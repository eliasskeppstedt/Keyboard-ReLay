#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>
#include "../eventHandler.c"

struct two_pointers 
{
    CFRunLoopRef* pRunLoop;
    cJSON* remapTable;
};

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
);

CGEventRef MyEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) 
{
    struct two_pointers* data = (struct two_pointers*)refcon;
    CFRunLoopRef* pRunLoop = data->pRunLoop;
    cJSON* remapTable = data->remapTable;
    return handleMacEvent(type, event, pRunLoop, remapTable);
}

int macStartMonitoring(cJSON* remapTable) 
{
    CFRunLoopRef runLoop = CFRunLoopGetMain();
    struct two_pointers* data = malloc(sizeof(struct two_pointers));
    data->pRunLoop = &runLoop;
    data->remapTable = remapTable;

    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap, // tap; window server, login session, specific annotation
        kCGHeadInsertEventTap, // places; head, tail
        K_CG_EVENT_TAP_OPTION_DEFAULT, // options; default, listen only
        EVENT_MASK, // eventsOfInterest; mouse, keyboard, etc
        MyEventTapCallBack, // callback func called when a quartz event is triggered
        data // userInfo, I pass pointer to the runLoop to be able to close it from within the callback
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
    free(data);

    printf("Event service closed, exiting program...");
    return 1; // (return True for if statement in main)
}