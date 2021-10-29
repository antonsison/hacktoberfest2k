import random

choices = list(range(100))
random.shuffle(choices)

print(choices.pop())

while choices:
    if input('Get another number? (Y/N)' ).lower() == 'n':
        break
    print(choices.pop())