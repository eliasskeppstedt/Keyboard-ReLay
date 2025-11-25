#ifndef _QUARTZEVENTSERVICE_
#define _QUARTZEVENTSERVICE_

#include <stdio.h>
#include <stdlib.h>
#include <CoreGraphics/CGEvent.h> // For ex: everything within CFMachPortRef
#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGEventSource.h>
#include <CoreFoundation/CFRunLoop.h>
#include <CoreFoundation/CoreFoundation.h> // umm for CFAbsoluteTimeGetCurrent() i think
#include "./../interface/initData.h"
#include "./../constants.h"

// int code
#define CF_DIC_KEY(code) (const void*)(uintptr_t) code

typedef struct EventTapCallBackData {

} EventTapCallBackData;

/*
 * @attention quartzEventService.h
 * @brief refcon is currently nothing
 */
CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon);

/*
 * @attention quartzEventService.h
 * @brief callback for a timer invoked by a on hold press. info pointer should be pointer to the event´s timer
 */
void timerCallBack(CFRunLoopTimerRef timer, void* info);

#endif // _QUARTZEVENTSERVICE_