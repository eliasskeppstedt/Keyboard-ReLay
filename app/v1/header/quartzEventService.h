#ifndef _QUARTZEVENTSYSTEM_
#define _QUARTZEVENTSYSTEM_

//#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
//#include <CoreGraphics/CGEventTypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGEventSource.h>
#include <CoreFoundation/CFRunLoop.h>
#include "dataStorage.h"
#include "constants.h"
#include "eventHandler.h"

struct eventTapCallBackData {
    CFRunLoopRef runLoop;
    struct keyData* pRemapTable;
};

CGEventRef myEventTapCallBack(CGEventTapProxy, CGEventType, CGEventRef, void*);

int macStartMonitoring(struct keyData*);

#endif // _QUARTZEVENTSYSTEM_