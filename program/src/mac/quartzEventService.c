#include <stdio.h>
#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp);

CGEventRef MyEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) 
{
    printf("Callback func!\n");
    return event;
}

int startMonitoring() 
{
    printf("2. Initializing event tap...\n");
    int testValue = 5;
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap, // tap; window server, login session, specific annotation
        kCGHeadInsertEventTap, // places; head, tail
        K_CG_EVENT_TAP_OPTION_DEFAULT, // options; default, listen only
        EVENT_MASK, // eventsOfInterest; mouse, keyboard, etc
        MyEventTapCallBack, // callback func called when a quartz event is triggered
        &testValue
    );
    if (!eventTap) {
        printf("\
            Could not initialize event tap. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
    }
    printf("\
        Event tap successfully created\n4. Creating run loop source\n");
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, 
        eventTap, 
        0
    );
    printf("\
        Run loop source created\n5. Adding run loop source\n");
    CFRunLoopAddSource(
        CFRunLoopGetCurrent(), 
        runLoopSource, 
        kCFRunLoopCommonModes
    );
    printf("\
        Run loop source created\n6. Enable event tap\n");
    CGEventTapEnable(
        eventTap, 
        true
    );
    printf("\
        Event tap enabled\n7. Run event loop\n");
    CFRunLoopRun();
    printf("\
        Event loop closed\n");
    CFMachPortInvalidate(eventTap);        // ensures no more messages are delivered - chatgpt
    CFRelease(runLoopSource);              // if you created one - chatgpt
    CFRelease(eventTap);
    printf("Event service closed, exiting program...");
    return 0;
}

int main()
{
    printf("1. Program started\n");
    if(!startMonitoring()) { printf("error"); return 1; }
    return 0;
}