#include "./../../header/os/quartzEventService.h"
#include "./../../header/interface/initData.h"
#include "./../../header/interface/relayEventHandler.h"

// -------- statics
/* 
 * @attention quartzEventService.c
 * @attention CFMutableDictionaryRef* remaps = (CFMutableDictionaryRef*) MY_RELAY.rem  aps;
 * @attention CFMutableDictionaryRef* statusTable = (CFMutableDictionaryRef*) MY_RELAY.statusTable;
 * 
 * @brief why are so many comment @ descriptions not working?! and i want struct member not param D:
 * 
 * @param void* remaps is an array of CFMutableDictionaryRef
 * @param void* statusTable is a CFMutableDictionaryRef  
 */
static MyReLay MY_RELAY;
static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
); // only masking keyboard events as of now
static uint64_t evSrcUserData;
/*
 * @brief kills program if it missbehaves and spams keys to fast
 */
static inline void watchdog_ping_or_die(void);
static void printMacEvent(CGEventRef macEvent);
static CFMutableDictionaryRef getRemapTable();
static CFMutableDictionaryRef getStatusTable();

// --------

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) 
{
    watchdog_ping_or_die(); // infinite feedback loop prevention. Should not be needed, just in case tho...
    evSrcUserData = CGEventGetIntegerValueField(event, kCGEventSourceUserData);
    //printf("ev src user data: %llu\n", evSrcUserData);
    bool returnNull = ( 
        CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat)
    );

    if (returnNull)/* fix better logic for this such that auto repeat is ok */
    {
        return NULL;
    }

    if (evSrcUserData == SIMULATED_EVENT)
    {
        return event;
    }

    //if (CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode) == 53) exit(1);

    RLEvent* rlEvent = RLEventCreate(
        CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode),
        type == kCGEventKeyDown,
        CGEventGetFlags(event),
        CGEventGetTimestamp(event),
        type == kCGEventFlagsChanged,
        evSrcUserData
    );
    
    eventCallBack(rlEvent);

    return NULL;    
}

void postEvent(uint64_t code, bool keyDown, uint64_t flagMask, uint64_t timeStamp, DataCodes userDefinedData)
{

    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    CGEventRef macEvent = CGEventCreateKeyboardEvent(src, code, keyDown);
    CGEventSetIntegerValueField(macEvent, kCGEventSourceUserData, userDefinedData); // send some user defined data
    CGEventSetFlags(macEvent, flagMask);
    CGEventSetTimestamp(macEvent, timeStamp);
    //printRLEvent(rlEvent);
    //printMacEvent(macEvent);
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

int initRunLoop() 
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
        NULL // userInfo, I pass pointer to the runLoop to be able to close it from within the callback
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

void printMacEvent(CGEventRef event)
{
    printf(">  macEvent {\n");
    printf(">   code: %lld\n", CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
    printf(">   flagMask: %llu\n", CGEventGetFlags(event));
    printf(">   timeStampOnPress: %llu\n", CGEventGetTimestamp(event));
    printf(">   isModifier: %s\n", CGEventGetType(event) == kCGEventFlagsChanged ? "true" : "false");
    printf(">   keyDown: %s\n", CGEventGetType(event) == kCGEventKeyDown ? "true" : "false");
    printf("> }\n");
}

void setModFlags(ModKeys* modKey)
{
    *modKey = (ModKeys) {
        .CAPS_LOCK_MASK     = kCGEventFlagMaskAlphaShift,
        .SHIFT_MASK         = kCGEventFlagMaskShift,
        .CONTROL_MASK       = kCGEventFlagMaskControl,
        .ALTERNATE_MASK     = kCGEventFlagMaskAlternate,
        .META_MASK          = kCGEventFlagMaskCommand,
        .CAPS_LOCK_CODE     = 57,
        .L_SHIFT_CODE       = 56,
        .R_SHIFT_CODE       = 60,
        .L_CONTROL_CODE     = 59,
        .R_CONTROL_CODE     = 62,
        .L_ALTERNATE_CODE   = 58,
        .R_ALTERNATE_CODE   = 61,
        .L_META_CODE        = 55,
        .R_META_CODE        = 54,
    };
}

//////////////////////////////// MY_RELAY

//////////////////////////////////////
////////////// remaps ////////////////
//////////////////////////////////////

void createRemapsArray(int layers)
{
    MY_RELAY.remaps = calloc(layers, sizeof(CFMutableDictionaryRef));
    MY_RELAY.layerEntries = layers;
    MY_RELAY.activeLayer = 0;
}

void createRemapTableForLayer(int layer, int capacity)
{
    CFMutableDictionaryRef* remaps = (CFMutableDictionaryRef*)MY_RELAY.remaps;
    remaps[layer] = CFDictionaryCreateMutable(
        kCFAllocatorDefault, 
        capacity, // capacity - hint about how many key-value pairs will be in the dic. It may optimize various operations.
        NULL, 
        NULL
    );
}

void addRemapTableEntry(int layer, uint64_t from, uint64_t toOnPress, uint64_t toOnHold)
{
    if (from == NO_VALUE) exit(NO_VALUE_FROM);
    KeyInfo* keyInfo = malloc(sizeof(KeyInfo));
    *keyInfo = (KeyInfo) {
        .code = from,
        .codeOnPress = toOnPress,
        .codeOnHold = toOnHold
    };

    // key/value fields take raw pointers since i created the dic with their callback value as NULL.
    // cast int into an integer of the size as an integer pointer (intptr_t), then and cast an int into a constant void pointer. This is effectively an address to invalid memory
    // and as such, NEVER dereference
    CFDictionaryAddValue(getRemapTable(), CF_DIC_KEY(from), keyInfo); 
}

KeyInfo* getKeyInfo(uint64_t code)
{
    KeyInfo* keyInfo = CFDictionaryGetValue(getRemapTable(), CF_DIC_KEY(code));
    if (!keyInfo)
    {
        return NULL;
    }
    
    return keyInfo;
}

static CFMutableDictionaryRef getRemapTable()
{
    CFMutableDictionaryRef remapTable = ((CFMutableDictionaryRef*) MY_RELAY.remaps)[MY_RELAY.activeLayer];
    return remapTable;
}


//////////////////////////////////////
////////////// status ////////////////
//////////////////////////////////////

void createStatusTable(int uniqueKeyCodeEntries)
{
    MY_RELAY.statusTable = malloc(sizeof(CFMutableDictionaryRef));
    CFMutableDictionaryRef* statusTable = (CFMutableDictionaryRef*)MY_RELAY.statusTable;
    *statusTable = CFDictionaryCreateMutable(
        kCFAllocatorDefault, 
        uniqueKeyCodeEntries, // capacity - hint about how many key-value pairs will be in the dic. It may optimize various operations.
        NULL, 
        NULL
    );
}

void addStatusTableEntry(uint64_t from)
{
    if (from == NO_VALUE) exit(NO_VALUE_FROM);

    KeyStatus* keyStatus = malloc(sizeof(KeyStatus));
    *keyStatus = (KeyStatus) {
        .code = NO_VALUE,
        .keyDown = false,
        .onHold = false,
    };

    CFDictionaryAddValue(getStatusTable(), CF_DIC_KEY(from), keyStatus); 
}

KeyStatus* getKeyStatus(uint64_t code)
{
    return (KeyStatus*) CFDictionaryGetValue(getStatusTable(), CF_DIC_KEY(code));
}

static CFMutableDictionaryRef getStatusTable()
{
    CFMutableDictionaryRef statusTable = *(CFMutableDictionaryRef*) MY_RELAY.statusTable;
    return statusTable;
}


//////////////////////////////////////
///////////// layer //////////////////
//////////////////////////////////////

void changeToLayer(int layer)
{
    if (layer >= MY_RELAY.layerEntries) return;
    
    MY_RELAY.activeLayer = layer;
}

void setLayerEntries(int layerEntries)
{
    MY_RELAY.layerEntries = layerEntries;
}

int getLayerEntries()
{
    return MY_RELAY.layerEntries;
}

int getActiveLayer()
{
    return MY_RELAY.activeLayer;
}

void deleteMyReMap()
{
    CFMutableDictionaryRef* remaps = (CFMutableDictionaryRef*)MY_RELAY.remaps;
    for (int i = 0; i < MY_RELAY.layerEntries; i++) {
        if (!remaps[i]) continue;
        int count = CFDictionaryGetCount(remaps[i]);
        const void **layer = malloc(count * sizeof(void*)); // must be used for cf dic function
        const void **remapTable = malloc(count * sizeof(void*));
        CFDictionaryGetKeysAndValues(remaps[i], layer, remapTable);
        for (int j = 0; j < count; j++) {
            if (!remapTable[j]) continue;
            free(remapTable[j]); 
        }
        free(layer);
        free(remapTable);

        CFDictionaryRemoveAllValues(remaps[i]);
        CFRelease(remaps[i]);
    }
    free(MY_RELAY.remaps);
    MY_RELAY.remaps = NULL;

    CFMutableDictionaryRef statusTable = *(CFMutableDictionaryRef*)MY_RELAY.statusTable;
    CFDictionaryRemoveAllValues(statusTable);
    CFRelease(statusTable);
    MY_RELAY.statusTable = NULL;
}

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////

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