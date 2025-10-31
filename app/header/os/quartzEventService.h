#ifndef _QUARTZEVENTSERVICE_
#define _QUARTZEVENTSERVICE_

#include <stdio.h>
#include <stdlib.h>
#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGEventSource.h>
#include <CoreFoundation/CFRunLoop.h>
#include <CoreFoundation/CoreFoundation.h> // umm for CFAbsoluteTimeGetCurrent() i think
#include "../data.h"
#include "../converter.h"
#include "interface.h"

typedef struct EventTapCallBackData {

} EventTapCallBackData;

CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon);
int eventOSToReLay(CGEventType type, CGEventRef macEvent, RLEvent* rlEvent, int* macToRL);
void printMacEvent(CGEventRef* macEvent);
void timerCallBack(CFRunLoopTimerRef timer, void* info);
static inline void watchdog_ping_or_die(void);

#endif // _QUARTZEVENTSERVICE_