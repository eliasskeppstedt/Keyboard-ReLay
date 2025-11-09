#ifndef _EVENTQUEUE_
#define _EVENTQUEUE_

#include <stdio.h>
#include <stdlib.h>
#include <dispatch/dispatch.h>
#include <sys/time.h>
#include "data.h"

typedef enum QueuePosition {
    HEAD, TAIL
} QueuePosition;

#define isEmpty(queue) !queue->isFull && queue->head == queue->tail

#define queueForEach(event, queue) for(element = (queue != NULL) ? (queue)->buffer[(queue)->head] : NULL; element != NULL; element = dequeue(queue))
/*
@param EventQueue* eventQueue
*/
RLEvent* getEvent(EventQueue* eventQueue, QueuePosition pos);

/*
RLEvent* event, 
EventQueue* eventQueue, 
KRKeyStatus* statusTable
return EXIT_CODE_EVENT_QUEUE_FULL (999), exit program start debugging...
*/
int enqueue(RLEvent* event, EventQueue* eventQueue);

/*
RLEvent* event,
EventQueue* eventQueue
*/
void enqueueSqueezeToFront(RLEvent* event, EventQueue* eventQueue);

/*
EventQueue* eventQueue
return NULL if empty
return dequeued event
*/
RLEvent* dequeue(EventQueue* eventQueue);

RLEvent* dequeueFromTail(EventQueue* eventQueue);

#endif // _EVENTQUEUE_