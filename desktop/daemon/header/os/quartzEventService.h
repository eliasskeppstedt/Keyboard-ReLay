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
#include "interface.h"

// int code
#define KEY(code) (const void*)(uintptr_t) code

typedef struct EventTapCallBackData {

} EventTapCallBackData;

/*
 * @attention quartzEventService.h
 * @brief refcon is currently nothing
 */
CGEventRef myEventTapCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon);

/*
 * @attention quartzEventService.h
 * @return RLEvent* created rl event, must be freed when posted!
 */
RLEvent* RLEventCreate(CGEventType type, CGEventRef macEvent);

/*
 * @attention quartzEventService.h
 * @brief callback for a timer invoked by a on hold press. info pointer should be pointer to the event´s timer
 */
void timerCallBack(CFRunLoopTimerRef timer, void* info);

/*
 * @attention quartzEventService.h
 * @brief used by initData, create the M
 * @param int layers in total, which then will hold the remap table for that layer (a CF dict)
 */
void createRemapsArray(int size);

/*
 * @attention quartzEventService.h
 * @brief used by initData,
 */
void createRemapTableForLayer(int layer, int capacity);

/*
 * @attention quartzEventService.h
 * @brief used by initData
 */
void addRemapTableEntry(int layer, int from, int toOnPress, int toOnHold);

/*
 * @attention quartzEventService.h
 * @brief used by initData
 */
void createStatusTable(int uniqueKeyCodeEntries);

/*
 * @attention quartzEventService.h
 * @brief used by initData
 */
void addStatusTableEntry(int from);

/*
 * @attention quartzEventService.h
 * @brief used by initData
 */
void setLayerEntries(int layerEntries);

#endif // _QUARTZEVENTSERVICE_