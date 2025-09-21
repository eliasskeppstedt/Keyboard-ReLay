#include "./../../header/quartzEventService.h"

static uint32_t K_CG_EVENT_TAP_OPTION_DEFAULT = 0x00000000; // for Mac OS X v10.4 support
static uint32_t EVENT_MASK = (
    CGEventMaskBit(kCGEventKeyDown) | 
    CGEventMaskBit(kCGEventKeyUp) |
    CGEventMaskBit(kCGEventFlagsChanged)
); // only masking keyboard events as of now

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* pRefcon) 
{
    eventTapCallBackData* pData = pRefcon;
    int keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keyCode == MAC_ESCAPE)
    {
        CFRunLoopStop(pData->runLoop);
        return NULL;
    }
    // suppress keys repeated by holding a key
    if (CGEventGetIntegerValueField(event, kCGKeyboardEventAutorepeat) != 0) 
    { 
        return NULL; 
    } 
    //printf("pWebToOS[8] should be 34, is: %d\n", pData->pLookUpTables->pWebToOS[8]);
    //printf("pOSToMac[34] should be 8, is: %d\n", pData->pLookUpTables->pOSToWeb[34]);

    generalizedEvent* pMacEvent = malloc(sizeof(generalizedEvent));
    pMacEvent->eventFlagMask = CGEventGetFlags(event);
    pMacEvent->keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    
    if (type == kCGEventKeyDown)
    {
        pMacEvent->isPressed = true;
    } 
    else if (type == kCGEventKeyUp)
    {
        pMacEvent->isPressed = false;
    }

    int e = handleMacEvent(pData->pLayerEntries, pMacEvent, pData->pLookUpTables);

    CGEventSetFlags(event, pMacEvent->eventFlagMask);  
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, pMacEvent->keyCode);
    free(pMacEvent); // a new generalized event is heap allocated for each event
    printf("\n");
    return event;
}

#include <stddef.h>
int macStartMonitoring(layers* pLayerEntries, lookUpTables* pLookUpTables) 
{
    printf("\nSetting upp run loop... ");
    CFRunLoopRef runLoop = CFRunLoopGetMain();
    printf("ok\n");
    printf("pRemapTable[1].keyCode should be 1, is: %i\n", pLayerEntries->pRemapTable[1].keyCode); // this does not work wtf? ig this is fucking up my remap table
    printf("pRemapTable[1].keyCode should be 1, is: %i\n", pLayerEntries->pRemapTable[1].keyCode);
    eventTapCallBackData* pData = malloc(sizeof(eventTapCallBackData));
    pData->pLayerEntries = pLayerEntries;
    pData->runLoop = runLoop;
    pData->pLookUpTables = pLookUpTables;
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
        printf("Could not initialize event tap. [Write suggestions on why this may happen, else contact support blablabla]");
        return 1; // exit program
    }
    printf("ok\n");

    printf("Setting upp run loop source... ");
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
        kCFAllocatorDefault, 
        eventTap, 
        0
    );
    printf("ok\n");

    printf("Adding source to run loop... ");
    CFRunLoopAddSource(
        runLoop, 
        runLoopSource, 
        kCFRunLoopCommonModes
    );
    printf("ok\n");

    printf("Enabeling event tap... ");
    CGEventTapEnable(
        eventTap, 
        true
    );
    printf("ok\n");

    printf("Starting event loop...\n\n");
    CFRunLoopRun();

    CFMachPortInvalidate(eventTap);        // ensures no more messages are delivered - chatgpt
    CFRelease(runLoopSource);              // if you created one - chatgpt
    CFRelease(eventTap);
    free(pData);

    printf("\nEvent loop closed\n");
    return 0;
}