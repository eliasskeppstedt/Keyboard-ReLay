#include "./../header/eventQueue.h"

// implemented as a ring buffer

RLEvent* getEvent(EventQueue* eventQueue, QueuePosition pos)
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
    int tail = (eventQueue->tail - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    return eventQueue->buffer[tail];   
}

int enqueue(RLEvent* event, EventQueue* eventQueue)
{
    if (eventQueue->isFull)
    {
        printf("    uuuuh this should not happen but event queue is full somehow.... ig exit program for debugging...\n");
        exit(1);
    }
    eventQueue->buffer[eventQueue->tail] = event; 
    eventQueue->tail = (eventQueue->tail + 1) % MAX_QUEUE_SIZE;
    
    eventQueue->isFull = eventQueue->tail == eventQueue->head;

    // TODO implement double press support
    return 0;

}

void enqueueSqueezeToFront(RLEvent* event, EventQueue* eventQueue)
{
    if (eventQueue->isFull)
    {
        printf("    uuuuh this should not happen but event queue is full somehow.... could not sqeeze in front\nig exit program for debugging...\n");
        exit(1);
    }
    eventQueue->head = (eventQueue->head - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    eventQueue->buffer[eventQueue->head] = event;

    eventQueue->isFull = eventQueue->head == eventQueue->tail;
}

RLEvent* dequeue(EventQueue* eventQueue)
{
    bool isEmpty = !eventQueue->isFull && eventQueue->head == eventQueue->tail;
    if (isEmpty)
    {
        //printf("    Queue is empty!\n");
        return NULL;
    }

    RLEvent* headEvent = eventQueue->buffer[eventQueue->head];
    eventQueue->buffer[eventQueue->head] = NULL;
    eventQueue->isFull = false;
    eventQueue->head = (eventQueue->head + 1) % MAX_QUEUE_SIZE;
    // TODO implement double press support
    return headEvent;
}

RLEvent* dequeueFromTail(EventQueue* eventQueue)
{
    bool isEmpty = !eventQueue->isFull && eventQueue->head == eventQueue->tail;
    if (isEmpty)
    {
        //printf("    Queue is empty!\n");
        return NULL;
    }
    eventQueue->tail = (eventQueue->tail - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    RLEvent* tailEvent = eventQueue->buffer[eventQueue->tail];
    eventQueue->buffer[eventQueue->tail] = NULL;
    eventQueue->isFull = false;
    return tailEvent;
}