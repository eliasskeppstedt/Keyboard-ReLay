# Keyboard ReLay
A program developed to enable remapping and layering for your keyboard.

REMEMBER: CURRENTLY NEEDING TO EDIT THE ENTRY NUMBER IN keys.json IN ORDER TO ADD ALL KEYS TO ITS CORRESPONDING LOOKUP TABLE

toOnPressHold - custom string - ex skicka denna sträng vid detta event, kan ex vara för att skicka åäö på engelsk layout

instead of always allocating new space for new key events, i could have a cirkular buffer where all events go, so i pre allocate the spaces for x buffers from main, and then i just use them as i run the program. Since there are only so many key presses at a time, i could go for a high number, since the space taken by creating such buffer is not large, and could definitely be worth over allocation in this real time application

