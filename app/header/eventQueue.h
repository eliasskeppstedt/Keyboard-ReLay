#ifndef _EVENTQUEUE_
#define _EVENTQUEUE_

#include <stdio.h>
#include "data.h"

#include <dispatch/dispatch.h>
#include <sys/time.h>
#include <stdlib.h>

typedef enum QueuePosition {
    HEAD, TAIL
} QueuePosition;
/*
@param EventQueue* eventQueue
*/
GeneralizedEvent* getEvent(EventQueue* eventQueue, QueuePosition pos);

/*
GeneralizedEvent* event, 
EventQueue* eventQueue, 
KRKeyStatus* statusTable
return EXIT_CODE_EVENT_QUEUE_FULL (999), exit program start debugging...
*/
int enqueue(GeneralizedEvent* event, EventQueue* eventQueue, KRKeyStatus* statusTable);

/*
EventQueue* eventQueue
KRKeyStatus* statusTable
return NULL if empty
return dequeued event
*/
GeneralizedEvent* dequeue(EventQueue* eventQueue, KRKeyStatus* statusTable);

#endif // _EVENTQUEUE_