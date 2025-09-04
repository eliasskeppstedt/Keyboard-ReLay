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
#include "./../lib/cJSON.h"
#include "constants.h"
#include "eventHandler.h"

struct two_pointers 
{
    CFRunLoopRef* pRunLoop;
    cJSON* remapTable;
};

CGEventRef MyEventTapCallBack(CGEventTapProxy, CGEventType, CGEventRef, void*);

int macStartMonitoring(cJSON*);

#endif // _QUARTZEVENTSYSTEM_