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
#include "data.h"
#include "keyHandler.h"
#include "converter.h"

typedef struct eventTapCallBackData {
    CFRunLoopRef runLoop;
    struct layers* pLayerEntries;
    lookUpTables* pLookUpTables;
} eventTapCallBackData;

#define MAC_ESCAPE 0x35

CGEventRef myEventTapCallBack(CGEventTapProxy, CGEventType, CGEventRef, void*);

int macStartMonitoring(layers*, lookUpTables*);

#endif // _QUARTZEVENTSYSTEM_