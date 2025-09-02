#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGEventSource.h>
#include "./mac/keycodes.c"
#include "./mac/json.c"

CGEventRef handleMacEvent(CGEventType type, CGEventRef event, CFRunLoopRef* pRunLoop, cJSON* macRemapTable) 
{
    int64_t keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    if (keyCode == MAC_ESC) 
    { 
        CFRunLoopStop(*pRunLoop); 
        deleteRemapTable(macRemapTable); 
        return NULL;
    }

    if (type == kCGEventKeyDown) 
    {
        printf("key down");
    } 
    else if (type == kCGEventKeyUp) 
    {
        printf("key up");
    }
    else if (type == kCGEventFlagsChanged) 
    {
        printf("modifier key, ");
    } 
    printf("Callback func! Keycode: %lli\n", keyCode);

    return event;
}