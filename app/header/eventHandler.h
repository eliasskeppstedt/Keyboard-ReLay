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

void myRunLoopTimerCallBack(CFRunLoopTimerRef, void*);

void createRunLoopTimer(struct dynamicData*);

CGEventRef createEventForKey();

CGEventRef handleMacEvent(struct staticData*);

#endif // _EVENTHANDLER_