#include "./../header/eventQueue.h"

// implemented as a ring buffer

GeneralizedEvent* getEvent(EventQueue* eventQueue, QueuePosition pos)
{
    //printf("  in getEvent\n    eventQueue: \n");
    //printf("      head:%d \n", eventQueue->head);
    //printf("      tail:%d \n", eventQueue->tail);
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
int enqueue(GeneralizedEvent* event, EventQueue* eventQueue, KRKeyStatus* statusTable)
{
    //printf("  in enqueue\n    eventQueue: \n");
    //printf("      head:%d \n", eventQueue->head);
    //printf("      tail:%d \n", eventQueue->tail);
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

GeneralizedEvent* dequeue(EventQueue* eventQueue, KRKeyStatus* statusTable)
{
    //printf("  in dequeue\n    eventQueue: \n");
    //printf("      head:%d \n", eventQueue->head);
    //printf("      tail:%d \n", eventQueue->tail);
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

int makeReadyForSend(GeneralizedEvent* event, EventQueue* eventQueue)
{
    return 0;
}