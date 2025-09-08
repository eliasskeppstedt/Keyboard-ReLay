#include "./../header/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
);

CGEventRef MyEventTapCallBack(CGEventTapProxy pProxy, CGEventType type, CGEventRef pEvent, void* pRefcon) 
{
    struct staticData* pStaticData = (struct staticData*)pRefcon;
    pStaticData->pDynamicData->pEvent = pEvent;
    pStaticData->pDynamicData->type = type;
    int64_t keyCode = CGEventGetIntegerValueField(pEvent, kCGKeyboardEventKeycode);
    pStaticData->pDynamicData->pKey = &pStaticData->pRemapTable[keyCode];
    return handleMacEvent(pStaticData);
}

int macStartMonitoring(struct keyData* pRemapTable) 
{
    CFRunLoopRef pRunLoop = CFRunLoopGetMain();
    struct staticData* pStaticData = malloc(sizeof(struct staticData)); 
    struct dynamicData* pDynamicData = malloc(sizeof(struct dynamicData)); 
    pStaticData->pRunLoop = pRunLoop;
    pStaticData->pRemapTable = pRemapTable;    
    pStaticData->pDynamicData = pDynamicData;
    CFMachPortRef pEventTap = CGEventTapCreate(
        kCGHIDEventTap, // tap; window server, login session, specific annotation
        kCGHeadInsertEventTap, // places; head, tail
        K_CG_EVENT_TAP_OPTION_DEFAULT, // options; default, listen only
        EVENT_MASK, // eventsOfInterest; mouse, keyboard, etc
        MyEventTapCallBack, // callback func called when a quartz event is triggered
        pStaticData // userInfo, I pass pointer to the runLoop to be able to close it from within the callback
    );

    if (!pEventTap) 
    {
        printf("\
            Could not initialize event tap. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
    }

    CFRunLoopSourceRef pRunLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, 
        pEventTap, 
        0
    );

    CFRunLoopAddSource(
        pRunLoop, 
        pRunLoopSource, 
        kCFRunLoopCommonModes
    );

    CGEventTapEnable(
        pEventTap, 
        true
    );

    CFRunLoopRun();

    CFMachPortInvalidate(pEventTap);        // ensures no more messages are delivered - chatgpt
    CFRelease(pRunLoopSource);              // if you created one - chatgpt
    CFRelease(pEventTap);
    free(pStaticData);

    printf("Event service closed, exiting program...");
    return 1; // (return True for if statement in main)
}