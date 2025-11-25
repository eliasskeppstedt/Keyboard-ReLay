#include "./../header/eventQueue.h"

/* @attention eventQueue.c */
static EventQueue* EVENT_QUEUE;

// implemented as a ring buffer

RLEvent* getEvent(QueuePosition pos)
{
    if (!EVENT_QUEUE->isFull && EVENT_QUEUE->head == EVENT_QUEUE->tail)
    {
        //printf("    queue empty\n");
        return NULL;
    }
    
    if (pos == HEAD)
    {
        return EVENT_QUEUE->buffer[EVENT_QUEUE->head];
    }
    int tail = (EVENT_QUEUE->tail - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    return EVENT_QUEUE->buffer[tail];   
}

int enqueue(RLEvent* event)
{
    if (EVENT_QUEUE->isFull)
    {
        printf("    uuuuh this should not happen but event queue is full somehow.... ig exit program for debugging...\n");
        exit(1);
    }
    EVENT_QUEUE->buffer[EVENT_QUEUE->tail] = event; 
    EVENT_QUEUE->tail = (EVENT_QUEUE->tail + 1) % MAX_QUEUE_SIZE;
    
    EVENT_QUEUE->isFull = EVENT_QUEUE->tail == EVENT_QUEUE->head;

    // TODO implement double press support
    return 0;
}

RLEvent* dequeue()
{
    bool isEmpty = !EVENT_QUEUE->isFull && EVENT_QUEUE->head == EVENT_QUEUE->tail;
    if (isEmpty)
    {
        //printf("    Queue is empty!\n");
        return NULL;
    }

    RLEvent* headEvent = EVENT_QUEUE->buffer[EVENT_QUEUE->head];
    EVENT_QUEUE->buffer[EVENT_QUEUE->head] = NULL;
    EVENT_QUEUE->isFull = false;
    EVENT_QUEUE->head = (EVENT_QUEUE->head + 1) % MAX_QUEUE_SIZE;
    // TODO implement double press support
    return headEvent;
}

void createEventQueue() 
{
    EVENT_QUEUE = malloc(sizeof(EventQueue));
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        EVENT_QUEUE->buffer[i] = NULL;
    }
    EVENT_QUEUE->head = 0;
    EVENT_QUEUE->tail = 0;
    EVENT_QUEUE->isFull = false;
}