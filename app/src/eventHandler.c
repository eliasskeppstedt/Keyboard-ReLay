#include "./../header/eventHandler.h"

static int KEYS_PRESSED_DOWN_BY_KEY_CODE[5] = { BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE };
int MASK_CURRENT_MODIFIERS = 0;

CGEventRef handleMacEvent(CGEventType type, CGEventRef event, CFRunLoopRef runLoop, struct keyData* pRemapTable)
{
    /*CGEventRef event = pStaticData->pDynamicData->event;
    CFRunLoopRef runLoop = pStaticData->runLoop;
    CGEventType type = pStaticData->pDynamicData->type;
    struct keyData* pKey = pStaticData->pDynamicData->pKey;*/
    int64_t keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    struct keyData* pKey = &pRemapTable[keyCode];
    if (type == kCGEventFlagsChanged) 
    {
        // modify event or whatever
        return event;
    } 
    int64_t isRepeat = CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat);
    if (isRepeat) return NULL; // dont repeate presses on key hold
    if (keyCode == MAC_ESC) 
    { 
        CFRunLoopStop(runLoop);
        return NULL;
    }
    printf("- 1 from handle mac event: keycode: %i\n", pKey->macKeyCode); // data is lost between the call of cur fun and here
    struct runLoopTimerCallBackData* pData = malloc(sizeof(struct runLoopTimerCallBackData));
    pData->event = event;
    pData->pKey = pKey;
    pData->type = type;
    if (type == kCGEventKeyDown) 
    {
        pKey->isPressed = true;
        //printf("(1) keycode: %i, comboKey: %i\n", pRemapTable[keyCode].macKeyCode, pRemapTable[keyCode].comboKey);
        // lager 0 för nu. TODO: fix dynamic usage of layers
        //printf("keyDown: value: %s, keycode: %i\n", "keyX" /*add in remapTable keyData structure*/, pRemapTable[keyCode].pMacKeyCodeRemapOnPress[0]);
        createRunLoopTimer(pData);
    } 
    else if (type == kCGEventKeyUp) 
    {
        // lager 0 för nu. TODO: fix dynamic usage of layers
        //printf("keyUpp: value: %s, keycode: %i\n", "keyX" /*add in remapTable keyData structure*/, pKey->pMacKeyCodeRemapOnPress[0]);
        if (pKey->isPressed)
        {
        }
        pKey->isPressed = false;
    }
    if (keyCode == MAC_B)
    {
        //event = createEventForKey(type, event, MAC_LEFT_SHIFT);
    }

    return event;
}

CGEventRef modifyEvent(CGEventType type, CGEventRef pEvent, uint16_t keyCode)
{
    return NULL;
}

void createRunLoopTimer(struct runLoopTimerCallBackData* pData)
{   
    //printf();
    CFRunLoopTimerContext context;
    context.copyDescription = NULL;
    context.info = pData;
    context.release = NULL;
    context.retain = NULL;
    context.version = 0;
    //int eventFlags = CGEventGetFlags(pData->event);
    printf("- 1 from create timer: keycode: %i\n", pData->pKey->macKeyCode);
    CFRunLoopTimerRef runLoopTimer = CFRunLoopTimerCreate(
        kCFAllocatorDefault, // allocator
        CFAbsoluteTimeGetCurrent() + 0.200, // fireDate
        0, // interval: <=0 just fires one time then gets invalidated
        0, // flags: ignored, 0 for future compatibility
        0, // order: runloop timers currently ignore order, thus set to 0 (3 sep 2025)
        myRunLoopTimerCallBack, // callout: callback func for when timer fires
        &context // context: data for callback func
    );

    CFRunLoopAddTimer(CFRunLoopGetCurrent(), runLoopTimer, kCFRunLoopCommonModes);
}

void myRunLoopTimerCallBack(CFRunLoopTimerRef runLoopTimer, void* pInfo)
{
    // se till att vara på rätt lager, implementera senare //
    /*struct dynamicData* pData = pInfo;*/
    struct runLoopTimerCallBackData* pData = pInfo;
    CGEventRef event = pData->event;
    struct keyData* pKey = pData->pKey;
    printf("- 1 from timer callback: keycode: %i\n", pKey->macKeyCode);
    if (MASK_CURRENT_MODIFIERS)
    {
        //CGEventSetFlags(event, modifierFlags);
    }
    free(pData);
    CFRunLoopTimerInvalidate(runLoopTimer);
    CFRelease(runLoopTimer);
}
