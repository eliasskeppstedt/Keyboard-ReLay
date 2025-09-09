#include "./../header/eventHandler.h"

static int KEYS_PRESSED_DOWN_BY_KEY_CODE[5] = { BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE, BITMASK_NOT_ACTIVE };

void myRunLoopTimerCallBack(CFRunLoopTimerRef runLoopTimer, void* pContext)
{
    // se till att vara på rätt lager, implementera senare //
    struct dynamicData* pData = (struct dynamicData*) pContext;
    CGEventRef event = pData->event;
    struct keyData* pKey = pData->pKey;
    if (pKey->isPressed)
    {
        int modifierFlags = (
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_CONTROL] ||
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_OPTION_ALLT] ||
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_SHIFT] ||
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_META] ||
            KEYS_PRESSED_DOWN_BY_KEY_CODE[INDEX_CAPS]
        );
        //CGEventSetFlags(event, modifierFlags);
    }
    free(pData);
    CFRunLoopTimerInvalidate(runLoopTimer);
    CFRelease(runLoopTimer);
}

void createRunLoopTimer(struct dynamicData* pData)
{   
    //printf();
    CFRunLoopTimerContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.info = pData;

    CFRunLoopTimerRef pRunLoopTimer = CFRunLoopTimerCreate(
        kCFAllocatorDefault, // allocator
        CFAbsoluteTimeGetCurrent() + 0.200, // fireDate
        0, // interval: <=0 just fires one time then gets invalidated
        0, // flags: ignored, 0 for future compatibility
        0, // order: runloop timers currently ignore order, thus set to 0 (3 sep 2025)
        myRunLoopTimerCallBack, // callout: callback func for when timer fires
        &ctx //&runLoopTimer // context: data for callback func
    );

    CFRunLoopAddTimer(CFRunLoopGetCurrent(), pRunLoopTimer, kCFRunLoopCommonModes);
}

CGEventRef modifyEvent(CGEventType type, CGEventRef pEvent, uint16_t keyCode)
{
    return NULL;
}

CGEventRef handleMacEvent(struct staticData* pStaticData) 
{
    CGEventRef event = pStaticData->pDynamicData->event;
    CFRunLoopRef runLoop = pStaticData->runLoop;
    int64_t isRepeat = CGEventGetIntegerValueField (event, kCGKeyboardEventAutorepeat);
    if (isRepeat) return NULL; // dont repeate presses on key hold
    int64_t keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    if (keyCode == MAC_ESC) 
    { 
        CFRunLoopStop(runLoop);
        return NULL;
    }
    CGEventType type = pStaticData->pDynamicData->type;
    struct keyData* pKey = pStaticData->pDynamicData->pKey;
    struct dynamicData* pData = malloc(sizeof(struct dynamicData));
    pData->event = event;
    pData->pKey = pKey;
    pData->type = type;
    printf("\n");
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
        printf("keyUpp: value: %s, keycode: %i\n", "keyX" /*add in remapTable keyData structure*/, pKey->pMacKeyCodeRemapOnPress[0]);
        if (pKey->isPressed)
        {
        }
        pKey->isPressed = false;
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
