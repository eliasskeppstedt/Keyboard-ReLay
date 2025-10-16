#ifndef _EVENTHANDLER_
#define _EVENTHANDLER_

#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGEventSource.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "dataStorage.h"

struct runLoopTimerCallBackData {
    CGEventRef event;
    CGEventType type;
    struct keyData* pKey;
};

void myRunLoopTimerCallBack(CFRunLoopTimerRef, void*);

void createRunLoopTimer(struct runLoopTimerCallBackData*);

CGEventRef createEventForKey();

CGEventRef handleMacEvent(CGEventType, CGEventRef, CFRunLoopRef, struct keyData*);
#endif // _EVENTHANDLER_