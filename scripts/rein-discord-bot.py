import asyncio
import functools
import itertools
import math
import random
import os

import discord
import youtube_dl
from async_timeout import timeout
from discord.ext import commands
from discord import guild
from discord_slash import SlashCommand, SlashContext,cog_ext
from discord_slash.utils.manage_commands import create_choice, create_option
from discordTogether import DiscordTogether
from replit import db
from webserver import keep_alive

# Silence useless bug reports messages
youtube_dl.utils.bug_reports_message = lambda: ''
gids = db["guilds"]


class VoiceError(Exception):
    pass


class YTDLError(Exception):
    pass


class YTDLSource(discord.PCMVolumeTransformer):
    YTDL_OPTIONS = {
        'format': 'bestaudio/best',
        'extractaudio': True,
        'audioformat': 'mp3',
        'outtmpl': '%(extractor)s-%(id)s-%(title)s.%(ext)s',
        'restrictfilenames': True,
        'noplaylist': True,
        'nocheckcertificate': True,
        'ignoreerrors': False,
        'logtostderr': False,
        'quiet': True,
        'no_warnings': True,
        'default_search': 'auto',
        'source_address': '0.0.0.0',
    }
    FFMPEG_OPTIONS = {
        'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5',
        'options': '-vn',
    }

    ytdl = youtube_dl.YoutubeDL(YTDL_OPTIONS)

    def __init__(self, ctx: commands.Context, source: discord.FFmpegPCMAudio, *, data: dict, volume: float = 0.5):
        super().__init__(source, volume)

        self.requester = ctx.author
        self.channel = ctx.channel
        self.data = data

        self.uploader = data.get('uploader')
        self.uploader_url = data.get('uploader_url')
        date = data.get('upload_date')
        self.upload_date = date[6:8] + '.' + date[4:6] + '.' + date[0:4]
        self.title = data.get('title')
        self.thumbnail = data.get('thumbnail')
        self.description = data.get('description')
        self.duration = self.parse_duration(int(data.get('duration')))
        self.tags = data.get('tags')
        self.url = data.get('webpage_url')
        self.views = data.get('view_count')
        self.likes = data.get('like_count')
        self.dislikes = data.get('dislike_count')
        self.stream_url = data.get('url')

    def __str__(self):
        return '**{0.title}** by **{0.uploader}**'.format(self)

    @classmethod
    async def create_source(cls, ctx: commands.Context, search: str, *, loop: asyncio.BaseEventLoop = None):
        loop = loop or asyncio.get_event_loop()

        partial = functools.partial(cls.ytdl.extract_info, search, download=False, process=False)
        data = await loop.run_in_executor(None, partial)

        if data is None:
            raise YTDLError('Couldn\'t find anything that matches `{}`'.format(search))

        if 'entries' not in data:
            process_info = data
        else:
            process_info = None
            for entry in data['entries']:
                if entry:
                    process_info = entry
                    break

            if process_info is None:
                raise YTDLError('Couldn\'t find anything that matches `{}`'.format(search))

        webpage_url = process_info['webpage_url']
        partial = functools.partial(cls.ytdl.extract_info, webpage_url, download=False)
        processed_info = await loop.run_in_executor(None, partial)

        if processed_info is None:
            raise YTDLError('Couldn\'t fetch `{}`'.format(webpage_url))

        if 'entries' not in processed_info:
            info = processed_info
        else:
            info = None
            while info is None:
                try:
                    info = processed_info['entries'].pop(0)
                except IndexError:
                    raise YTDLError('Couldn\'t retrieve any matches for `{}`'.format(webpage_url))

        return cls(ctx, discord.FFmpegPCMAudio(info['url'], **cls.FFMPEG_OPTIONS), data=info)

    @staticmethod
    def parse_duration(duration: int):
        minutes, seconds = divmod(duration, 60)
        hours, minutes = divmod(minutes, 60)
        days, hours = divmod(hours, 24)

        duration = []
        if days > 0:
            duration.append('{} days'.format(days))
        if hours > 0:
            duration.append('{} hours'.format(hours))
        if minutes > 0:
            duration.append('{} minutes'.format(minutes))
        if seconds > 0:
            duration.append('{} seconds'.format(seconds))

        return ', '.join(duration)


class Song:
    __slots__ = ('source', 'requester')

    def __init__(self, source: YTDLSource):
        self.source = source
        self.requester = source.requester

    def create_embed(self):
        embed = (discord.Embed(title='Now playing',
                               description='[{0.source.title}]({0.source.url})'.format(self,self),
                               color=discord.Color.blurple())
                 .add_field(name='Duration', value=self.source.duration)
                 .add_field(name='Uploader', value='[{0.source.uploader}]({0.source.uploader_url})'.format(self))
                 .set_footer(text="Requested by {0}".format(self.requester), icon_url=self.requester.avatar_url)
                 .set_thumbnail(url=self.source.thumbnail))
        return embed


class SongQueue(asyncio.Queue):
    def __getitem__(self, item):
        if isinstance(item, slice):
            return list(itertools.islice(self._queue, item.start, item.stop, item.step))
        else:
            return self._queue[item]

    def __iter__(self):
        return self._queue.__iter__()

    def __len__(self):
        return self.qsize()

    def clear(self):
        self._queue.clear()

    def shuffle(self):
        random.shuffle(self._queue)

    def remove(self, index: int):
        del self._queue[index]


class VoiceState:
    def __init__(self, bot: commands.Bot, ctx: commands.Context):
        self.bot = bot
        self._ctx = ctx

        self.current = None
        self.voice = None
        self.next = asyncio.Event()
        self.songs = SongQueue()

        self._loop = False
        self._volume = 0.5
        self.skip_votes = set()

        self.audio_player = bot.loop.create_task(self.audio_player_task())

    def __del__(self):
        self.audio_player.cancel()

    @property
    def loop(self):
        return self._loop

    @loop.setter
    def loop(self, value: bool):
        self._loop = value

    @property
    def volume(self):
        return self._volume

    @volume.setter
    def volume(self, value: float):
        self._volume = value

    @property
    def is_playing(self):
        return self.voice and self.current

    async def audio_player_task(self):
        while True:
            self.next.clear()

            if not self.loop:
                # Try to get the next song within 3 minutes.
                # If no song will be added to the queue in time,
                # the player will disconnect due to performance
                # reasons.
                try:
                    async with timeout(180):  # 3 minutes
                        self.current = await self.songs.get()
                except asyncio.TimeoutError:
                    self.bot.loop.create_task(self.stop())
                    return

            self.current.source.volume = self._volume
            self.voice.play(self.current.source, after=self.play_next_song)
            await self.current.source.channel.send(embed=self.current.create_embed())

            await self.next.wait()

    def play_next_song(self, error=None):
        if error:
            raise VoiceError(str(error))

        self.next.set()

    def skip(self):
        self.skip_votes.clear()

        if self.is_playing:
            self.voice.stop()

    async def stop(self):
        self.songs.clear()

        if self.voice:
            await self.voice.disconnect()
            self.voice = None


class Music(commands.Cog):
    def __init__(self, bot: commands.Bot):
        self.bot = bot
        self.voice_states = {}

    def get_voice_state(self, ctx: commands.Context):
        state = self.voice_states.get(ctx.guild.id)
        if not state:
            state = VoiceState(self.bot, ctx)
            self.voice_states[ctx.guild.id] = state

        return state

    def cog_unload(self):
        for state in self.voice_states.values():
            self.bot.loop.create_task(state.stop())

    def cog_check(self, ctx: commands.Context):
        if not ctx.guild:
            raise commands.NoPrivateMessage('This command can\'t be used in DM channels.')

        return True

    async def cog_before_invoke(self, ctx: commands.Context):
        ctx.voice_state = self.get_voice_state(ctx)

    async def cog_command_error(self, ctx: commands.Context, error: commands.CommandError):
        await ctx.send('{}'.format(str(error)))

    @commands.command(name='join', invoke_without_subcommand=True)
    async def _join(self, ctx: commands.Context):
        """Joins a voice channel."""

        destination = ctx.author.voice.channel
        if ctx.voice_state.voice:
            await ctx.voice_state.voice.move_to(destination)
            return

        ctx.voice_state.voice = await destination.connect()

    @commands.command(name='comeagain')
    async def _summon(self, ctx: commands.Context, *, channel: discord.VoiceChannel = None):
        """Summons the bot to a voice channel.

        If no channel was specified, it joins your channel.
        """

        if not channel and not ctx.author.voice:
            raise VoiceError('You are neither connected to a voice channel nor specified a channel to join.')

        destination = channel or ctx.author.voice.channel
        if ctx.voice_state.voice:
            await ctx.voice_state.voice.move_to(destination)
            return

        ctx.voice_state.voice = await destination.connect()

    @commands.command(name='goaway', aliases=['disconnect'])
    async def _leave(self, ctx: commands.Context):
        """Clears the queue and leaves the voice channel."""

        if not ctx.voice_state.voice:
            return await ctx.send('Not connected to any voice channel.')

        await ctx.voice_state.stop()
        del self.voice_states[ctx.guild.id]

    @commands.command(name='volume')
    async def _volume(self, ctx: commands.Context, *, volume: int):
        """Sets the volume of the player."""

        if not ctx.voice_state.is_playing:
            return await ctx.send('Nothing being played at the moment.')

        if 0 > volume > 100:
            return await ctx.send('Volume must be between 0 and 100')

        ctx.voice_state.volume = volume / 100
        await ctx.send('Volume of the player set to {}%'.format(volume))

    @commands.command(name='now', aliases=['current', 'playing'])
    async def _now(self, ctx: commands.Context):
        """Displays the currently playing song."""

        await ctx.send(embed=ctx.voice_state.current.create_embed())

    @commands.command(name='pause')
    async def _pause(self, ctx: commands.Context):
        """Pauses the currently playing song."""

        if ctx.voice_state.is_playing and ctx.voice_state.voice.is_playing():
            ctx.voice_state.voice.pause()
            await ctx.message.add_reaction('⏯')

    @commands.command(name='resume')
    async def _resume(self, ctx: commands.Context):
        """Resumes a currently paused song."""

        if ctx.voice_state.is_playing and ctx.voice_state.voice.is_paused():
            ctx.voice_state.voice.resume()
            await ctx.message.add_reaction('⏯')

    @commands.command(name='stop')
    async def _stop(self, ctx: commands.Context):
        """Stops playing song and clears the queue."""

        ctx.voice_state.songs.clear()

        if ctx.voice_state.is_playing:
            ctx.voice_state.voice.stop()
            await ctx.message.add_reaction('⏹')

    @commands.command(name='skip')
    async def _skip(self, ctx: commands.Context):
        """Vote to skip a song. The requester can automatically skip.
        3 skip votes are needed for the song to be skipped.
        """

        if not ctx.voice_state.is_playing:
            return await ctx.send('Not playing any music right now...')

        await ctx.message.add_reaction('⏭')
        ctx.voice_state.skip()

       
    @commands.command(name='queue')
    async def _queue(self, ctx: commands.Context, *, page: int = 1):
        """Shows the player's queue.

        You can optionally specify the page to show. Each page contains 10 elements.
        """

        if len(ctx.voice_state.songs) == 0:
            return await ctx.send('Empty queue.')

        items_per_page = 10
        pages = math.ceil(len(ctx.voice_state.songs) / items_per_page)

        start = (page - 1) * items_per_page
        end = start + items_per_page

        queue = ''
        for i, song in enumerate(ctx.voice_state.songs[start:end], start=start):
            queue += '`{0}.` [**{1.source.title}**]({1.source.url})\n'.format(i + 1, song)

        embed = (discord.Embed(description='**{} tracks:**\n\n{}'.format(len(ctx.voice_state.songs), queue))
                 .set_footer(text='Viewing page {}/{}'.format(page, pages)))
        await ctx.send(embed=embed)

    @commands.command(name='shuffle')
    async def _shuffle(self, ctx: commands.Context):
        """Shuffles the queue."""

        if len(ctx.voice_state.songs) == 0:
            return await ctx.send('Empty queue.')

        ctx.voice_state.songs.shuffle()
        await ctx.message.add_reaction('✅')

    @commands.command(name='remove')
    async def _remove(self, ctx: commands.Context, index: int):
        """Removes a song from the queue at a given index."""

        if len(ctx.voice_state.songs) == 0:
            return await ctx.send('Empty queue.')

        ctx.voice_state.songs.remove(index - 1)
        await ctx.message.add_reaction('✅')
        await ctx.invoke(self.bot.get_command('queue'))

    @commands.command(name='loop')
    async def _loop(self, ctx: commands.Context):
        """Loops the currently playing song.

        Invoke this command again to unloop the song.
        """

        if not ctx.voice_state.is_playing:
            return await ctx.send('Nothing being played at the moment.')

        # Inverse boolean value to loop and unloop.
        ctx.voice_state.loop = not ctx.voice_state.loop
        await ctx.message.add_reaction('✅')

    @commands.command(name='help')
    async def _help(self, ctx: commands.Context):
      if ctx.author.guild.id not in db["guilds"]:
          guilds = db["guilds"]
          guilds.append(ctx.author.guild.id )
          db["guilds"] = guilds

      embed = (discord.Embed(title='List of commands',
                          color=discord.Color.blurple())
            .add_field(name="** **",value="`rein!comeagain` - Make some room for me",inline=False)
            .add_field(name="** **",value="`rein!goaway` - I will go away and come again another day",inline=False)
            .add_field(name="** **",value="`rein!play` - I will sing a song from the bottom of my heart",inline=False)
            .add_field(name="** **",value="`rein!now` - I will show you what i'm currently singing",inline=False)
            .add_field(name="** **",value="`rein!pause` - I will pause and catch my breath",inline=False)
            .add_field(name="** **",value="`rein!resume` - The show must go on",inline=False)
            .add_field(name="** **",value="`rein!stop` - From the top make it Stop",inline=False)
            .add_field(name="** **",value="`rein!skip` - Thank you, next",inline=False)
            .add_field(name="** **",value="`rein!loop` - I will sing your favorite song forever",inline=False)
            .add_field(name="** **",value="`rein!shuffle` - Everyday i'm shuffling",inline=False)
            .add_field(name="** **",value="`rein!remove` - include the number please, i will remove a song the queue",inline=False)
            .add_field(name="** **",value="`rein!queue` -I will show you what's next (Optional:`rein!queue`+ page number )",inline=False)
            .add_field(name="** **",value="`rein!volume` - How loud should i sing? 1-100 please",inline=False)
            .add_field(name="** **",value="`rein!watchtogether` - Let's watch youtube together",inline=False)
            .add_field(name="** **",value="`rein!playpoker` - No gambling please",inline=False)
            .add_field(name="** **",value="`rein!playbetrayal` - It's like among us",inline=False)
            .add_field(name="** **",value="`rein!playfishing` - Just keep swimming",inline=False)
            .add_field(name="** **",value="`rein!playchess` - Practice your queen's gambit",inline=False) 
            .set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url))
     
      await ctx.send(embed=embed)

    @commands.command(name='playpoker')
    async def _playpoker(self, ctx: commands.Context):
       if ctx.author.voice.channel:
         link = await togetherControl.create_link(ctx.author.voice.channel.id, 'poker')
         embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
         embed.set_author(name="Poker ")
         embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
         await ctx.send(embed=embed)
       else:
         await ctx.send("Please join a voice channel 😑")

    @commands.command(name='playbetrayal')
    async def _playbetrayal(self, ctx: commands.Context):
      if ctx.author.voice.channel:
        link = await togetherControl.create_link(ctx.author.voice.channel.id, 'betrayal')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Betrayal.io")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")
   
    @commands.command(name='watchtogether')
    async def _watchtogether(self, ctx: commands.Context):
      if ctx.author.voice.channel:
        link = await togetherControl.create_link(ctx.author.voice.channel.id, 'youtube')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Watch Together")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")

    @commands.command(name='playfishing')
    async def _playfishing(self, ctx: commands.Context):
      if ctx.author.voice.channel:
        link = await togetherControl.create_link(ctx.author.voice.channel.id, 'fishing')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Fishington.io")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑") 

    @commands.command(name='playchess')
    async def _playchess(self, ctx: commands.Context):
      if ctx.author.voice.channel:
        link = await togetherControl.create_link(ctx.author.voice.channel.id, 'chess')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Chess in the Park ")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
         await ctx.send("Please join a voice channel 😑")   
  
    @commands.command(name='play')
    async def _play(self, ctx: commands.Context, *, search: str):
        """Plays a song.

        If there are songs in the queue, this will be queued until the
        other songs finished playing.

        This command automatically searches from various sites if no URL is provided.
        A list of these sites can be found here: https://rg3.github.io/youtube-dl/supportedsites.html
        """

        if not ctx.voice_state.voice:
            await ctx.invoke(self._join)

        await ctx.message.add_reaction('👌')

        async with ctx.typing():
            try:
                source = await YTDLSource.create_source(ctx, search, loop=self.bot.loop)
            except YTDLError as e:
                await ctx.send('An error occurred while processing this request: {}'.format(str(e)))
            else:
                song = Song(source)

                await ctx.voice_state.songs.put(song)

                if ctx.voice_state.is_playing and ctx.voice_state.voice.is_playing():
                  await ctx.send('Added to queue {}'.format(str(source)))
                  await ctx.invoke(self.bot.get_command('queue'))
                else:
                  await ctx.send('{}'.format(str(source)))

    @_join.before_invoke
    @_play.before_invoke
    async def ensure_voice_state(self, ctx: commands.Context):
        if not ctx.author.voice or not ctx.author.voice.channel:
            raise commands.CommandError('You are not connected to any voice channel.')

        if ctx.voice_client:
            if ctx.voice_client.channel != ctx.author.voice.channel:
                raise commands.CommandError('Bot is already in a voice channel.')
    
    @cog_ext.cog_slash(name="music", description="List of music commands", guild_ids=gids)
    async def play(self, ctx:SlashContext):

      if ctx.author.guild.id not in db["guilds"]:
          guilds = db["guilds"]
          guilds.append(ctx.author.guild.id )
          db["guilds"] = guilds

      ctx.voice_state = self.get_voice_state(ctx)
      embed = (discord.Embed(title='Music commands',
                            color=discord.Color.blurple())
              .add_field(name="** **",value="`rein!comeagain` - Make some room for me",inline=False)
              .add_field(name="** **",value="`rein!goaway` - I will go away and come again another day",inline=False)
              .add_field(name="** **",value="`rein!play` - I will sing a song from the bottom of my heart",inline=False)
              .add_field(name="** **",value="`rein!now` - I will show you what i'm currently singing",inline=False)
              .add_field(name="** **",value="`rein!pause` - I will pause and catch my breath",inline=False)
              .add_field(name="** **",value="`rein!resume` - The show must go on",inline=False)
              .add_field(name="** **",value="`rein!stop` - From the top make it Stop",inline=False)
              .add_field(name="** **",value="`rein!skip` - Thank you, next",inline=False)
              .add_field(name="** **",value="`rein!loop` - I will sing your favorite song forever",inline=False)
              .add_field(name="** **",value="`rein!shuffle` - Everyday i'm shuffling",inline=False)
              .add_field(name="** **",value="`rein!remove` - include the number please, i will remove a song the queue",inline=False)
              .add_field(name="** **",value="`rein!queue` -I will show what's next (Optional:`rein!queue`+ page number )",inline=False)
              .add_field(name="** **",value="`rein!volume` - How loud should i sing? 1-100 please",inline=False)
              .set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url))

      await ctx.send(embed=embed)
    
    @cog_ext.cog_slash(
    name="watchtogether", description="Watch youtube together!", guild_ids=gids,)
    async def watchtogether(self,ctx:SlashContext):
      if ctx.member.voice:
        link = await togetherControl.create_link(ctx.voice_state.voice.channel.id, 'youtube')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Watch Together")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")
      
    @cog_ext.cog_slash(
      name="playpoker",
      description="Play poker on discord!",
      guild_ids=gids,
    )
    async def playpoker(self,ctx:SlashContext):
      if ctx.member.voice:
        link = await togetherControl.create_link(ctx.voice_state.voice.channel.id, 'poker')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Poker ")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")

    @cog_ext.cog_slash(
      name="playbetrayal",
      description="Play betrayal on discord!",
      guild_ids=gids,
    )
    async def playbetrayal(self,ctx:SlashContext):
      if ctx.member.voice:
        link = await togetherControl.create_link(ctx.voice_state.voice.channel.id, 'betrayal')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Betrayal.io")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")

    @cog_ext.cog_slash(
      name="playfishing",
      description="Play fishing on discord!",
      guild_ids=gids,
    )
    async def playfishing(self,ctx:SlashContext):
      if ctx.member.voice:
        link = await togetherControl.create_link(ctx.voice_state.voice.channel.id, 'fishing')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Fishington.io")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")

    @cog_ext.cog_slash(
      name="playchess",
      description="Play chess on discord!",
      guild_ids=gids,
    )
    async def playchess(self,ctx:SlashContext):
      if ctx.member.voice:
        link = await togetherControl.create_link(ctx.author.voice.channel.id, 'chess')
        embed=discord.Embed(title="Click Here to join!", url=link, description="_Note: Please Close any active sharescreen in the voice channel_")
        embed.set_author(name="Chess in the Park ")
        embed.set_footer(text="Requested by {0}".format(ctx.author), icon_url=ctx.author.avatar_url)
        await ctx.send(embed=embed)
      else:
        await ctx.send("Please join a voice channel 😑")

bot = commands.Bot(command_prefix='rein!')
slash =  SlashCommand(bot, sync_commands=True)
bot.remove_command('help')
bot.add_cog(Music(bot))
togetherControl = DiscordTogether(bot)

@bot.event
async def on_ready():
    print('Logged in as:\n{0.user.name}\n{0.user.id}'.format(bot))

keep_alive()
bot.run(os.environ['discord_token'])