#include "./../header/eventQueue.h"

// implemented as a ring buffer

GeneralizedEvent* getEvent(EventQueue* eventQueue, QueuePosition pos)
{
    if (!eventQueue->isFull && eventQueue->head == eventQueue->tail)
    {
        //printf("    queue empty\n");
        return NULL;
    }
    
    if (pos == HEAD)
    {
        return eventQueue->buffer[eventQueue->head];
    }
    int tail = eventQueue->tail - 1;
    if (tail == -1)
    {
        tail = MAX_QUEUE_SIZE - 1;
    }
    return eventQueue->buffer[tail];   
}

/**
 * Does not enqueue if the key is already activated somewhere else 
 */
int enqueue(GeneralizedEvent* event, EventQueue* eventQueue)
{
    if (eventQueue->isFull)
    {
        printf("    uuuuh this should not happen but event queue is full somehow.... ig exit program for debugging...\n");
        exit(1);
    }
    eventQueue->buffer[eventQueue->tail] = event; 
    eventQueue->tail = (eventQueue->tail + 1) % MAX_QUEUE_SIZE;
    if (eventQueue->tail == eventQueue->head)
    {
        eventQueue->isFull = true;
    }
    // TODO implement double press support
    return 0;
}

void enqueueForHoldException(GeneralizedEvent* eventKeyDown, GeneralizedEvent* eventKeyUp, EventQueue* eventQueue)
{
    enqueue(eventKeyDown, eventQueue);
    enqueue(eventKeyUp, eventQueue);
    int tail = eventQueue->tail;
    while (eventQueue->head != tail)
    {
        enqueue(dequeue(eventQueue), eventQueue);
    }
    
}

GeneralizedEvent* dequeue(EventQueue* eventQueue)
{
    bool isEmpty = !eventQueue->isFull && eventQueue->head == eventQueue->tail;
    if (isEmpty)
    {
        //printf("    Queue is empty!\n");
        return NULL;
    }

    GeneralizedEvent* event = eventQueue->buffer[eventQueue->head];
    eventQueue->buffer[eventQueue->head] = NULL;
    eventQueue->isFull = false;
    eventQueue->head = (eventQueue->head + 1) % MAX_QUEUE_SIZE;
    // TODO implement double press support
    return event;
}