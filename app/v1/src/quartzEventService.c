#include "./../header/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
);

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* pRefcon) 
{   
    struct eventTapCallBackData* pData = pRefcon;
    return handleMacEvent(type, event, pData->runLoop, pData->pRemapTable);
}

int macStartMonitoring(struct keyData* pRemapTable) 
{
    CFRunLoopRef runLoop = CFRunLoopGetMain();
    struct eventTapCallBackData* pData = malloc(sizeof(struct eventTapCallBackData));
    pData->pRemapTable = pRemapTable;
    pData->runLoop = runLoop;
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap, // tap; window server, login session, specific annotation
        kCGHeadInsertEventTap, // places; head, tail
        K_CG_EVENT_TAP_OPTION_DEFAULT, // options; default, listen only
        EVENT_MASK, // eventsOfInterest; mouse, keyboard, etc
        myEventTapCallBack, // callback func called when a quartz event is triggered
        pData // userInfo, I pass pointer to the runLoop to be able to close it from within the callback
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
    free(pData);

    printf("Event service closed, exiting program...");
    return 1; // (return True for if statement in main)
}