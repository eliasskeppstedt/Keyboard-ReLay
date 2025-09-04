#include "./../header/eventHandler.h"

void myRunLoopTimerCallBack(CFRunLoopTimerRef runLoopTimer, void* keyCode)
{
    printf("fire!\n");
    CFRunLoopTimerInvalidate(runLoopTimer);
    CFRelease(runLoopTimer);
    free(keyCode);
}

void createRunLoopTimer(int64_t* keyCode)
{    
    CFRunLoopTimerRef runLoopTimer = CFRunLoopTimerCreate(
        kCFAllocatorDefault, // allocator
        CFAbsoluteTimeGetCurrent() + 0.200, // fireDate
        0, // interval: <=0 just fires one time then gets invalidated
        0, // flags: ignored, 0 for future compatibility
        0, // order: runloop timers currently ignore order, thus set to 0 (3 sep 2025)
        myRunLoopTimerCallBack, // callout: callback func for when timer fires
        NULL //&runLoopTimer // context: data for callback func
    );
    CFRunLoopAddTimer(CFRunLoopGetCurrent(), runLoopTimer, kCFRunLoopCommonModes);
}

CGEventRef createEventForKey(CGEventType type, CGEventRef event, uint16_t keyCode)
{
    return NULL;
}

CGEventRef handleMacEvent(CGEventType type, CGEventRef event, CFRunLoopRef* pRunLoop, cJSON* remapTable) 
{
    cJSON* macRemapTable = cJSON_GetObjectItem(remapTable, "macMapping");
    int64_t isRepeat = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
    if (isRepeat) return NULL;

    int64_t keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keyCode == MAC_ESC) 
    { 
        CFRunLoopStop(*pRunLoop);
        deleteRemapTable(macRemapTable); 
        return NULL;
    }
    printf("\n");
    if (type == kCGEventKeyUp) 
    {
        cJSON* remapTableOnHold = cJSON_GetObjectItem(macRemapTable, "remapsOnHold");
        if (remapTableOnHold)
        {
            char* jsonKeyName = getJsonKeyNameFromMacKeyCode(keyCode);
            cJSON* value = cJSON_GetObjectItemCaseSensitive(remapTableOnHold, jsonKeyName);
            if (value)
            {
                int64_t* pKeyCode = malloc(sizeof(int64_t));
                *pKeyCode = keyCode;
                printf("value: %s, keycode: %i\n", cJSON_Print(value), keyCode);
                createRunLoopTimer(&pKeyCode);
            }
        }
        printf("key up, ");
    }
    else if (type == kCGEventKeyDown) 
    {
        printf("key down, ");
    } 
    else if (type == kCGEventFlagsChanged) 
    {
        printf("modifier key, ");
    } 
    if (keyCode == MAC_B)
    {
        //event = createEventForKey(type, event, MAC_LEFT_SHIFT);
    }
    printf("Callback func! Keycode: %lli\n", keyCode);

    return event;
}
