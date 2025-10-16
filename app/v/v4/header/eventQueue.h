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

#define isEmpty(queue) !queue->isFull && queue->head == queue->tail

#define queueForEach(event, queue) for(element = (queue != NULL) ? (queue)->buffer[(queue)->head] : NULL; element != NULL; element = dequeue(queue))
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
int enqueue(GeneralizedEvent* event, EventQueue* eventQueue);

/*
GeneralizedEvent* event,
EventQueue* eventQueue
*/
void enqueueSqueezeToFront(GeneralizedEvent* event, EventQueue* eventQueue);

/*
EventQueue* eventQueue
return NULL if empty
return dequeued event
*/
GeneralizedEvent* dequeue(EventQueue* eventQueue);

GeneralizedEvent* dequeueFromTail(EventQueue* eventQueue);

#endif // _EVENTQUEUE_