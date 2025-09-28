#ifndef _EVENTQUEUE_
#define _EVENTQUEUE_

#include <stdio.h>
#include "data.h"

#include <dispatch/dispatch.h>
#include <sys/time.h>
#include <stdlib.h>

/**
 * @param EventQueue* pEventQueue
 */
#define VIEW_FIRST(pEventQueue) pEventQueue->buffer[pEventQueue->head]

/**
 * GeneralizedEvent* pEvent
 * EventQueue* pEventQueue
 * UniversalKeyStatus* pStatusTable
 * @return EXIT_CODE_EVENT_QUEUE_FULL (999), exit program start debugging...
 */
int enqueue(GeneralizedEvent* pEvent, EventQueue* pEventQueue, UniversalKeyStatus* pStatusTable);

/**
 * EventQueue* pEventQueue
 * UniversalKeyStatus* pStatusTable
 * @return NULL if empty
 * @return dequeued event
 */
GeneralizedEvent* dequeue(EventQueue* pEventQueue, UniversalKeyStatus* pStatusTable);

#endif // _EVENTQUEUE_