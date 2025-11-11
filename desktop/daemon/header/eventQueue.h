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

/*
 * @param QueuePosition HEAD or TAIL event
 */
RLEvent* getEvent(QueuePosition pos);

/*
 * @param RLEvent* event
 */
int enqueue(RLEvent* event);

/*
 * @brief this will put the event at the first (head) position
 * @param RLEvent* event
*/
void enqueueSqueezeToFront(RLEvent* event);

/*
 * @brief Dequeue the first arrived event
 * @return RLEvent* to the dequeued event
*/
RLEvent* dequeue();

/*
 * @brief Dequeue the last arrived event
 * @return RLEvent* to the dequeued event
*/
RLEvent* dequeueFromTail();

/*
 * @brief used by initData
 */
void createEventQueue();

#endif // _EVENTQUEUE_