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
#include <CoreFoundation/CoreFoundation.h> // umm for CFAbsoluteTimeGetCurrent() i think
#include "data.h"
#include "keyHandler.h"
#include "converter.h"

/**
 * CFRunLoopRef runLoop
 * struct layer* pLayerEntries
 * LookUpTables* pLookUpTables
 */
typedef struct EventTapCallBackData {
    CFRunLoopRef runLoop;
    Layer* layerList;
    LookUpTables* lookUpTables;
} EventTapCallBackData;

#define MAC_ESCAPE 0x35

CGEventRef myEventTapCallBack(
    CGEventTapProxy proxy, 
    CGEventType type, 
    CGEventRef event,
    void* refcon
);

int macStartMonitoring(Layer* layerList, LookUpTables* lookUpTables);

#endif // _QUARTZEVENTSYSTEM_