# ToDo ish
> [✅] FIFO queue 
> 
> instead of always allocating new space for new key events, i could have a cirkular buffer where all events go, so i pre allocate the spaces for x buffers from main, and then i just use them as i run the program. Since there are only so many key presses at a time, i could go for a high number, since the space taken by creating such buffer is not large, and could definitely be worth over allocation in this real time application

> [⭐️] Look over code to comply with new keymap system
> 
> While vibe coding the web site, i was thinking about how to logically "connect" the web key codes to my relay codes. My thought originaly was to create a website that will create code convert tables with custom (my relay). Then in my app, i know about these codes and then can translate my os code into that custom code. So to access the code for each key type, i used 2 arrays, one for os to my custom code and vice verse. Since i hardcoded the json file first i think i missed these issues, which is about scaling. Since i hardcoded in the relay codes to match the other codes meant that a change somewhere in the json file created one empty slot in the array. This is a very huge problem since breaking the incremental by 1 standard would break the indexation into the convert table So if i want to change something in the middle, i have to change all the relay codes for the subsequent entries. This lead me to use AI in order to change the json file, however this is very inefficiant and annoying to do. What i noticed while thinking about this problem was how i would even sync the different codes. For the previous problem i could have used CFDictionary to create key value pairs, in order to avoid the index dependency. It was also a problem with syncing the codes. Idealy, there should only be 1 place where a code is defined, so a change in one set of data does not requires a change in another set. So i thought about ways to avoid double defenitions, and ended up with ditching my custom codes for os codes. It felt quite simple when i thougth of it but i needed to start with the web site to get the idea i think. So now, the app will not even have a keymap over all keys, since that will be handled while creating the keymap on the website. While it can retrieve codes from the web API, the website has access to the keymap which makes it possible to directly store the correct OS code into the json file that im going to download. So the remap table i have done for this application is still valid, but now there will be the OS´s codes in it, so no need to convert the code. The website will just see "oh the user is using a macbook" and then add the web code´s corresponding mac keycodes to the remap table. SO what needs to be done is to look over the parts where i do stuff with the key code since it will just stay as the os code all the time.

> [⭐️] Change the remaptable from struct array to a [CFDictionary](https://developer.apple.com/documentation/corefoundation/cfdictionary?language=objc)
> 
> The scaling problem is also an issue, or more of an unnecessaryness. It just takes the highest valued remap code and creates an array that long times amount of layers is used. So if i have 3 layers and uses 4 mappings on each i only need 12 spaces. But if i then have support for 150 keys, i would end up with taking 450 KeyInfo struct sized memories. Not a huge deal but not ideal. So here i think the CFDictionary alternative can be a great method, and again then i dont need a specific length on an array and the codes must not necessarely be incremented by 1, i just need the codes to access the correct KeyInfo.

> [❓] there should be a counter for keys held down in the status array, such that for example command is always pressed even tho i change the command key i hold down (if the transition is overlapping). Why tho? ... idk atm ...maybe this would be good if there are multiple mod keys for example?

# Notes
REMEMBER: CURRENTLY NEEDING TO EDIT THE ENTRY NUMBER IN keys.json IN ORDER TO ADD ALL KEYS TO ITS CORRESPONDING LOOKUP TABLE

toOnPressHold - custom string - ex skicka denna sträng vid detta event, kan ex vara för att skicka åäö på engelsk layout

to turn off the mac accent popup on hold: defaults write -g ApplePressAndHoldEnabled -bool false

when headEvent is modifier, ALWAYS change the status event flag

Mac virtual key codes:
Events.h