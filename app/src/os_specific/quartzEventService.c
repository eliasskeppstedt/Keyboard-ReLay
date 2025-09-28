#include "./../../header/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
); // only masking keyboard events as of now

// safeguard in case of feedback loop ...
// Pick a high, unused bit for our private tag. 
static const CGEventFlags kMyInjectedFlag = (CGEventFlags)(1ULL << 50);
// (Optional) simple runaway detector
static inline void watchdog_ping_or_die(void) {
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

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* pRefcon) 
{
    watchdog_ping_or_die();

    printf("%s\n", type == kCGEventKeyDown ? "kew down" : "key up");

    EventTapCallBackData* pData = pRefcon;
    int keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    if (keyCode == MAC_ESCAPE)
    {
        CFRunLoopStop(pData->runLoop);
        return NULL;
    }
    if (CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat) != 0) 
    { 
        return NULL; 
    } 

    struct timeval timeOnPress;
    gettimeofday(&timeOnPress, NULL);

    GeneralizedEvent* pMacEvent = malloc(sizeof(GeneralizedEvent));
    pMacEvent->eventFlagMask = CGEventGetFlags(event);
    pMacEvent->code = keyCode;

    if (type == kCGEventKeyDown)
    {
        pMacEvent->keyDown = true;
    } 
    else if (type == kCGEventKeyUp)
    {
        pMacEvent->keyDown = false;
    }

    int e = handleMacEvent(pData->pLayerEntries, pMacEvent, pData->pLookUpTables);

    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    // in order to make every modifier as a press, pmacevent must have a modifier buffer, where we traverse over to send a key
    // down event for each of the modifier before the accuall key is presset.
    CGEventRef newEvent = CGEventCreateKeyboardEvent(source, pMacEvent->code, pMacEvent->keyDown);
    CGEventPost(kCGAnnotatedSessionEventTap, newEvent);
    CFRelease(newEvent);
    printf("\n");

    return NULL;
}

#include <stddef.h>
int macStartMonitoring(Layers* pLayerEntries, LookUpTables* pLookUpTables) 
{
    printf("Setting upp run loop... ");
    CFRunLoopRef runLoop = CFRunLoopGetMain();
    if (!runLoop) 
    {
        printf("Failed to set up run loop. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
    }
    printf("ok\n");

    EventTapCallBackData* pData = malloc(sizeof(EventTapCallBackData));
    *pData = (EventTapCallBackData) {
        runLoop,
        pLayerEntries,
        pLookUpTables
    };

    printf("Setting upp event tap... ");
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
        printf("Failed to set up event tap. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
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
        printf("Failed to set up run loop source. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
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
    free(pData);

    printf("\nEvent loop closed\n");
    return 0;
}