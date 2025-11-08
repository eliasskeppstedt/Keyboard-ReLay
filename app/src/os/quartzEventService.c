#include "../../header/os/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
); // only masking keyboard events as of now

static uint64_t evSrcUserData;

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) 
{
    watchdog_ping_or_die(); // infinite feedback loop prevention. Should not be needed, just in case tho...
    
    evSrcUserData = CGEventGetIntegerValueField(event, kCGEventSourceUserData);
    if (evSrcUserData == DONT_POST_DUE_TO_PENDING_EVENT ||
        CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat)/* fix better logic for this such that auto repeat is ok */) 
    {
        return NULL;
    }
    if (evSrcUserData == SIMULATED_EVENT)
    {
        return event;
    }

    MyReLay* myReLay = (MyReLay*)refcon;
    RLEvent* rlEvent = RLEventCreate(type, event);

    eventCallBack(myReLay, rlEvent);
    return NULL;    
    printRLEvent(rlEvent);
}

RLEvent* RLEventCreate(CGEventType type, CGEventRef macEvent)
{    
    RLEvent* rlEvent = malloc(sizeof(RLEvent));

    if (evSrcUserData == ON_HOLD_TIMER_EVENT)
    {
        rlEvent->code = NO_VALUE;
        return rlEvent;
    }

    int macCode = CGEventGetIntegerValueField(macEvent, kCGKeyboardEventKeycode);
    int rlCode = getCodeMacToRL(macCode);
    uint64_t macFlags = CGEventGetFlags(macEvent);
    bool isSupported = true;
    if (rlCode == NO_VALUE)
    {
        rlCode = macCode;
        isSupported = false;
    }
    printf("\n\nNEW EVENT original mac code: %d, rl code: %d %s\n",macCode, rlCode, isSupported ? "(supported)" : "(unsupported)");

    *rlEvent = (RLEvent) {
        .code = rlCode,
        .flagMask = getFlagsMacToRL(macFlags),
        .preservedOSFlagMask = macFlags,
        .timeStampOnPress = getTimeStamp(),
        .state = NORMAL,
        .isModifier = type == kCGEventFlagsChanged,
        .keyDown = type == kCGEventKeyDown,
        .isSupported = isSupported,
        .timer = NULL
    };
    return rlEvent;
}

// defined in interfaces.h
void postEvent(RLEvent* rlEvent, int userDefinedData)
{
    printRLEvent(rlEvent);
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    int code;
    if (rlEvent->isSupported)
    {
        code = getCodeRLToMac(rlEvent->code);
    }
    else
    {
        code = rlEvent->code;
    }
    
    uint64_t flags = NO_VALUE;
    setFlagsToMac(rlEvent->flagMask, &flags);
    flags |= rlEvent->preservedOSFlagMask;
    printf("keyDown? when posting event: %s\n", rlEvent->keyDown ? "true" : "false");
    CGEventRef macEvent = CGEventCreateKeyboardEvent(src, code, rlEvent->keyDown);
    
    CGEventSetIntegerValueField(macEvent, kCGEventSourceUserData, userDefinedData); // send some user defined data
    CGEventSetFlags(macEvent, flags);
    CGEventSetTimestamp(macEvent, rlEvent->timeStampOnPress);

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