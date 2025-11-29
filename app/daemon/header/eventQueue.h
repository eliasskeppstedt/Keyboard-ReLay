#ifndef _EVENTQUEUE_
#define _EVENTQUEUE_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "./constants.h"
#include "./interface/relayEventHandler.h"

/*
RLEvent* buffer[MAX_QUEUE_SIZE];
int head;
int tail;   
bool isFull;
*/
typedef struct EventQueue {
    RLEvent* buffer[MAX_QUEUE_SIZE];
    int head;
    int tail;   
    bool isFull;
} EventQueue;

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
 * @brief Dequeue the first arrived event
 * @return RLEvent* to the dequeued event
*/
RLEvent* dequeue();

/*
 * @brief used by initData
 */
void createEventQueue();

#endif // _EVENTQUEUE_