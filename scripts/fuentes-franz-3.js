//Description: Equinox was a discord bot project using discord.JS,
//it was SUPPOSED to be used to mute and unmute people in a discord voice chat on command which is useful for games like Among us,
//but with the creation of Crewlink and how easy it is to setup I have chosen to abandon the project and used Crewlink instead.
//this code MIGHT be though...


// require the discord.js module
const Discord = require('discord.js');

const { prefix, token } = require('./config.json');

// create a new Discord client
const client = new Discord.Client();

// when the client is ready, run this code
// this event will only trigger one time after logging in
client.once('ready', () => {
  console.log('Ready!');

  //client.user.setActivity('paint dry', { type: 'WATCHING' });

});

//listens for commands
client.on('message', message => {
	if (message.content === `${prefix}ping`) {
    // send back "Pong." to the channel the message was sent in
    message.channel.send('Pong.');
  }

  if (message.content === `${prefix}muteall`) {
    
    if (message.member.voice.channel) {
      //voiceChannel.join().then(connection => console.log('Connected!')).catch(console.error);
      message.channel.send('Let the Game Begin!');
      let channel = message.guild.channels.cache.get(message.member.voice.channel.id);
      for (const [memberID, member] of channel.members) {
	      
        member.voice.setMute(true); //this should mute all of the members in the voice channel
	      
      }
    } else {
      message.reply('You need to join a voice channel first!');
    }
  }

    if (message.content === `${prefix}unmuteall`) {
      
      if (message.member.voice.channel) {
        //voiceChannel.leave().then(connection => console.log('Disconnected!')).catch(console.error);
        message.channel.send('Speak up!');
        let channel = message.guild.channels.cache.get(message.member.voice.channel.id);
        for (const [memberID, member] of channel.members) {
		
          member.voice.setMute(false); //this should mute all of the members in the voice channel
		
        }
      } else {
        message.reply('You need to join a voice channel first!');
      }

  }
});

// login to Discord with your app's token
client.login(token);