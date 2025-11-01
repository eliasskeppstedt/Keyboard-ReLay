#include "../../header/os/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
); // only masking keyboard events as of now

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) 
{
    watchdog_ping_or_die(); // infinite feedback loop prevention. Should not be needed, just in case tho...

    if (CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode) == 0x35) // esc, just in case i break something with the convertion. Else esc is the key to quit
    {
        CFRunLoopStop(CFRunLoopGetCurrent());
        return NULL;
    }
    
    uint64_t evSrcUserData = CGEventGetIntegerValueField(event, kCGEventSourceUserData);
    if (evSrcUserData == DONT_POST_DUE_TO_PENDING_EVENT ||
        CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat)/* fix better logic for this such that auto repeat is ok */) 
    {
        return NULL;
    }
    if (evSrcUserData == SIMULATED_EVENT)
    {
        return event;
    }

    RLEvent* rlEvent = malloc(sizeof(RLEvent));
    MyReLay* myReLay = (MyReLay*)refcon;

    if (evSrcUserData == ON_HOLD_TIMER_EVENT)
    {
        *rlEvent = (RLEvent) {
            .code = NO_VALUE,
            .flagMask = NO_VALUE,
            .timeStampOnPress = NO_VALUE,
            .preservedOSFlagMask = CGEventGetFlags(event),
            .state = NORMAL,
            .isModifier = false,
            .keyDown = false,
            .timer = NULL
        };
    }
    else if (eventOSToReLay(type, event, rlEvent, myReLay->osToRL) == 1)
    {
        return event;
    }
    eventCallBack(myReLay, rlEvent);
    return NULL;
}

int eventOSToReLay(CGEventType type, CGEventRef macEvent, RLEvent* rlEvent, int* macToRL)
{
    int macCode = CGEventGetIntegerValueField(macEvent, kCGKeyboardEventKeycode);
    if (setCodeFromMac(macCode, &rlEvent->code, macToRL) == 1)
    {
        return 1;
    }
    uint64_t macFlagMask = CGEventGetFlags(macEvent);
    setFlagsFromMac(macFlagMask, &rlEvent->flagMask);
    rlEvent->preservedOSFlagMask = macFlagMask;
    rlEvent->timeStampOnPress = getTimeStamp();
    rlEvent->state = NORMAL;
    rlEvent->isModifier = type == kCGEventFlagsChanged;
    rlEvent->keyDown = type == kCGEventKeyDown;

    printf("\n\nNEW EVENT original mac code: %d, rl code: %d\n", macCode, rlEvent->code);
    return 0;
}

// defined in interfaces.h
void postEvent(RLEvent* rlEvent, int* rlToOS, int userDefinedData)
{
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    int code = NO_VALUE;
    uint64_t flags = NO_VALUE;
    setCodeToMac(rlEvent->code, &code, rlToOS);
    setFlagsToMac(rlEvent->flagMask, &flags);
    flags |= rlEvent->preservedOSFlagMask;

    //printRLEvent(rlEvent);
    CGEventRef macEvent = CGEventCreateKeyboardEvent(src, code, rlEvent->keyDown);
    
    CGEventSetIntegerValueField(macEvent, kCGEventSourceUserData, userDefinedData); // send some user defined data
    CGEventSetFlags(macEvent, flags);
    CGEventSetTimestamp(macEvent, rlEvent->timeStampOnPress);
    //printMacEvent(&macEvent);

    CGEventPost(kCGHIDEventTap, macEvent);
    CFRelease(src);
    CFRelease(macEvent);
}

void timerCallBack(CFRunLoopTimerRef timer, void* info)
{
    void** eventTimer = info;
    CFRelease(*eventTimer);
    *eventTimer = NULL;

    CGEventSourceRef src =  CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventRef onHoldTimerEvent = CGEventCreateKeyboardEvent(src, 0, 0); // keycode and keydown does not matter, we will react to the eventsourceuserdata when this event arrives
    CGEventSetIntegerValueField(onHoldTimerEvent, kCGEventSourceUserData, ON_HOLD_TIMER_EVENT); // send some user defined data
    CGEventPost(kCGHIDEventTap, onHoldTimerEvent);
    CFRelease(src);
    CFRelease(onHoldTimerEvent);
}

void invalidateTimer(void** eventTimer)
{
    CFRunLoopTimerRef timer = *eventTimer;
    CFRunLoopTimerInvalidate(timer);
    CFRelease(timer);
    *eventTimer = NULL;
}

void startOnHoldTimer(void** eventTimer)
{
    CFRunLoopTimerContext context = (CFRunLoopTimerContext) {
        .version = 0,
        .info = eventTimer,
        .retain = NULL,
        .release = NULL,
        .copyDescription = NULL
    };

    CFRunLoopTimerRef timer = CFRunLoopTimerCreate(
        kCFAllocatorDefault, //CFAllocatorRef allocator, 
        CFAbsoluteTimeGetCurrent() + CF_ON_HOLD_THRESHOLD, //CFAbsoluteTime fireDate, 
        0, //CFTimeInterval interval, // no interval => system will free this timer when it has fired
        0, //CFOptionFlags flags, 
        0, //CFIndex order, 
        timerCallBack, //CFRunLoopTimerCallBack callout, 
        &context //CFRunLoopTimerContext * context);
    );
    *eventTimer = timer;
    CFRunLoopRef rl = CFRunLoopGetCurrent();
    CFRunLoopAddTimer(rl, timer, kCFRunLoopCommonModes); // constant make this timer visible for all run loops, we only have one so no concern there 
}

void closeRunLoop(void* ctx)
{
    CFRunLoopStop(CFRunLoopGetCurrent());
}

int initRunLoop(MyReLay* myReLay) 
{
    printf("Setting upp run loop... ");
    CFRunLoopRef runLoop = CFRunLoopGetMain();
    if (!runLoop) 
    {
        printf("Failed to set up run loop. [Write suggestions on why this may happen, else contact support blablabla] ");
        exit(21);
    }
    printf("ok\n");

    EventTapCallBackData* callbackData = malloc(sizeof(EventTapCallBackData));
    *callbackData = (EventTapCallBackData) {
    };

    printf("Setting upp event tap... ");
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap, // tap; window server, login session, specific annotation
        kCGHeadInsertEventTap, // places; head, tail
        K_CG_EVENT_TAP_OPTION_DEFAULT, // options; default, listen only
        EVENT_MASK, // eventsOfInterest; mouse, keyboard, etc
        myEventTapCallBack, // callback func called when a quartz event is triggered
        myReLay // userInfo, I pass pointer to the runLoop to be able to close it from within the callback
    );
    if (!eventTap) 
    {
        printf("Failed to set up event tap. [Write suggestions on why this may happen, else contact support blablabla] ");
        exit(22); // exit program
    }
    printf("ok\n");

    printf("Setting upp run loop source... ");
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, 
        eventTap, 
        0
    );
    if (!runLoopSource) 
    {
        printf("Failed to set up run loop source. [Write suggestions on why this may happen, else contact support blablabla] ");
        exit(23); // exit program
    }
    printf("ok\n");

    printf("Adding source to run loop... \n");
    CFRunLoopAddSource(
        runLoop, 
        runLoopSource, 
        kCFRunLoopCommonModes
    );

    printf("Enabeling event tap... \n");
    CGEventTapEnable(
        eventTap, 
        true
    );

    printf("Starting run loop...\n\n");
    CFRunLoopRun();

    CFMachPortInvalidate(eventTap);        // ensures no more messages are delivered - chatgpt
    CFRelease(runLoopSource);              // if you created one - chatgpt
    CFRelease(eventTap);
    free(callbackData);

    printf("\nEvent loop closed\n");
    return 0;
}

void printMacEvent(CGEventRef* macEvent)
{
    printf(">  macEvent {\n");
    printf(">   code: %lld\n", CGEventGetIntegerValueField(*macEvent, kCGKeyboardEventKeycode));
    printf(">   flagMask: %llu\n", CGEventGetFlags(*macEvent));
    printf(">   timeStampOnPress: %llu\n", CGEventGetTimestamp(*macEvent));
    printf(">   isModifier: %s\n", CGEventGetType(*macEvent) == kCGEventFlagsChanged ? "true" : "false");
    printf(">   keyDown: %s\n", CGEventGetType(*macEvent) == kCGEventKeyDown ? "true" : "false");
    printf("> }\n");
}

static inline void watchdog_ping_or_die(void) 
{
    static struct timeval last = {0};
    static int burst = 0;

    struct timeval now; gettimeofday(&now, NULL);
    double dt = (now.tv_sec - last.tv_sec) + (now.tv_usec - last.tv_usec) / 1e6;

    if (dt < 0.002) { // >500 events/sec implies runaway
        if (++burst > 2000) { // ~4 seconds continuous
            fprintf(stderr, "Infinite loop detected — exiting.\n");
            _Exit(1);
        }
    } else {
        burst = 0;
    }
    last = now;
}