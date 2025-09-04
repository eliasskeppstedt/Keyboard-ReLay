#ifndef _EVENTHANDLING_
#define _EVENTHANDLING_

#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGEventSource.h>
#include <CoreFoundation/CFRunLoop.h>
#include "constants.h"
#include "dataStorage.h"

void myRunLoopTimerCallBack(CFRunLoopTimerRef, void*);

void createRunLoopTimer(int64_t*);

CGEventRef createEventForKey(CGEventType, CGEventRef, uint16_t);

CGEventRef handleMacEvent(CGEventType, CGEventRef, CFRunLoopRef*, cJSON*);

#endif // _EVENTHANDLING_