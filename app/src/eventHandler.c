#include "./../header/eventHandler.h"

static int KEYS_PRESSED_DOWN_BY_KEY_CODE[5] = { BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE };

void myRunLoopTimerCallBack(CFRunLoopTimerRef runLoopTimer, void* key_void)
{
    // se till att vara på rätt lager, implementera senare //
    struct keyData* key = (struct keyData*)key_void;
    if (key->isPressed)
    {
        int bitmaskModifiers = (
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_CONTROL] &&
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_OPTION_ALLT] &&
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_SHIFT] &&
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_META] &&
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_CAPS]
        );
    }
    CFRunLoopTimerInvalidate(runLoopTimer);
    CFRelease(runLoopTimer);
}

void createRunLoopTimer(struct keyData* pRemapTable, int keyCode)
{   
    CFRunLoopTimerContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.info = &pRemapTable[keyCode];

    CFRunLoopTimerRef runLoopTimer = CFRunLoopTimerCreate(
        kCFAllocatorDefault, // allocator
        CFAbsoluteTimeGetCurrent() + 0.200, // fireDate
        0, // interval: <=0 just fires one time then gets invalidated
        0, // flags: ignored, 0 for future compatibility
        0, // order: runloop timers currently ignore order, thus set to 0 (3 sep 2025)
        myRunLoopTimerCallBack, // callout: callback func for when timer fires
        &ctx //&runLoopTimer // context: data for callback func
    );

    CFRunLoopAddTimer(CFRunLoopGetCurrent(), runLoopTimer, kCFRunLoopCommonModes);
}

CGEventRef modifyEvent(CGEventType type, CGEventRef event, uint16_t keyCode)
{
    return NULL;
}

CGEventRef handleMacEvent(CGEventType type, CGEventRef event, CFRunLoopRef* pRunLoop, struct keyData* pRemapTable) 
{
    int64_t isRepeat = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
    if (isRepeat) return NULL;

    int64_t keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keyCode == MAC_ESC) 
    { 
        CFRunLoopStop(*pRunLoop);
        return NULL;
    }
    printf("\n");
    if (type == kCGEventKeyDown) 
    {
        pRemapTable[keyCode].isPressed = true;
        char* jsonKeyName = getJsonKeyNameFromMacKeyCode(keyCode);
        //printf("(1) keycode: %i, comboKey: %i\n", pRemapTable[keyCode].macKeyCode, pRemapTable[keyCode].comboKey);
        // lager 0 för nu. TODO: fix dynamic usage of layers
        //printf("keyDown: value: %s, keycode: %i\n", "keyX" /*add in remapTable keyData structure*/, pRemapTable[keyCode].pMacKeyCodeRemapOnPress[0]);
        createRunLoopTimer(pRemapTable, keyCode);
    } 
    else if (type == kCGEventKeyUp) 
    {
        char* jsonKeyName = getJsonKeyNameFromMacKeyCode(keyCode);
        // lager 0 för nu. TODO: fix dynamic usage of layers
        printf("keyUpp: value: %s, keycode: %i\n", "keyX" /*add in remapTable keyData structure*/, pRemapTable[keyCode].pMacKeyCodeRemapOnPress[0]);
        if (pRemapTable[keyCode].isPressed)
        {
        }
        pRemapTable[keyCode].isPressed = false;
    }
    else if (type == kCGEventFlagsChanged) 
    {
        printf("modifier key up or down: \n");
    } 
    if (keyCode == MAC_B)
    {
        //event = createEventForKey(type, event, MAC_LEFT_SHIFT);
    }

    return event;
}
