#include "./../header/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
);

CGEventRef MyEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* pRefcon) 
{
    struct staticData* pStaticData = (struct staticData*)pRefcon;
    pStaticData->pDynamicData->event = event;
    pStaticData->pDynamicData->type = type;
    int64_t keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    pStaticData->pDynamicData->pKey = &pStaticData->pRemapTable[keyCode];
    return handleMacEvent(pStaticData);
}

int macStartMonitoring(struct keyData* pRemapTable) 
{
    CFRunLoopRef runLoop = CFRunLoopGetMain();
    struct staticData* pStaticData = malloc(sizeof(struct staticData)); 
    struct dynamicData* pDynamicData = malloc(sizeof(struct dynamicData)); 
    pStaticData->runLoop = runLoop;
    pStaticData->pRemapTable = pRemapTable;    
    pStaticData->pDynamicData = pDynamicData;
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap, // tap; window server, login session, specific annotation
        kCGHeadInsertEventTap, // places; head, tail
        K_CG_EVENT_TAP_OPTION_DEFAULT, // options; default, listen only
        EVENT_MASK, // eventsOfInterest; mouse, keyboard, etc
        MyEventTapCallBack, // callback func called when a quartz event is triggered
        pStaticData // userInfo, I pass pointer to the runLoop to be able to close it from within the callback
    );

    if (!eventTap) 
    {
        printf("\
            Could not initialize event tap. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, 
        eventTap, 
        0
    );

    CFRunLoopAddSource(
        runLoop, 
        runLoopSource, 
        kCFRunLoopCommonModes
    );

    CGEventTapEnable(
        eventTap, 
        true
    );

    CFRunLoopRun();

    CFMachPortInvalidate(eventTap);        // ensures no more messages are delivered - chatgpt
    CFRelease(runLoopSource);              // if you created one - chatgpt
    CFRelease(eventTap);
    free(pStaticData);

    printf("Event service closed, exiting program...");
    return 1; // (return True for if statement in main)
}