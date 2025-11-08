# Current state
The daemon is kinda working, altho sometimes it seems to not change from the option flag, resulting in a bunch of presses that is interpreted as opt + the key press.

# ToDo ish
## app
[✅] instead of always allocating new space for new key events, i could have a cirkular buffer where all events go, so i pre allocate the spaces for x buffers from main, and then i just use them as i run the program. Since there are only so many key presses at a time, i could go for a high number, since the space taken by creating such buffer is not large, and could definitely be worth over allocation in this real time application

[⭐️] 

[❓] there should be a counter for keys held down in the status array, such that for example command is always pressed even tho i change the command key i hold down (if the transition is overlapping). Why tho? ... idk atm ...maybe this would be good if there are multiple mod keys for example?

## web app

# Notes
REMEMBER: CURRENTLY NEEDING TO EDIT THE ENTRY NUMBER IN keys.json IN ORDER TO ADD ALL KEYS TO ITS CORRESPONDING LOOKUP TABLE

toOnPressHold - custom string - ex skicka denna sträng vid detta event, kan ex vara för att skicka åäö på engelsk layout

to turn off the mac accent popup on hold: defaults write -g ApplePressAndHoldEnabled -bool false

when headEvent is modifier, ALWAYS change the status event flag