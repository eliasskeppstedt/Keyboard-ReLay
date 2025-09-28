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

/**
 * CFRunLoopRef runLoop
 * struct layers* pLayerEntries
 * LookUpTables* pLookUpTables
 */
typedef struct EventTapCallBackData {
    CFRunLoopRef runLoop;
    Layers* pLayerEntries;
    LookUpTables* pLookUpTables;
} EventTapCallBackData;

#define MAC_ESCAPE 0x35

CGEventRef myEventTapCallBack(
    CGEventTapProxy proxy, 
    CGEventType type, 
    CGEventRef event,
    void* pRefcon
);

int macStartMonitoring(Layers* pLayerEntries, LookUpTables* pLookUpTables);

#endif // _QUARTZEVENTSYSTEM_