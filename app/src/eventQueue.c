#include "./../header/eventQueue.h"

// implemented as a ring buffer

/**
 * Does not enqueue if the key is already activated somewhere else 
 */
int enqueue(GeneralizedEvent* pEvent, EventQueue* pEventQueue, UniversalKeyStatus* pStatusTable)
{
    if (pStatusTable[pEvent->code].keyDown)
    {
        return MOD_ALREADY_ACTIVE;
    }
    if (pEventQueue->isFull)
    {
        printf("uuuuh this should not happen but event queue is full somehow.... ig exit program for debugging...\n");
        return EXIT_CODE_EVENT_QUEUE_FULL;
    }
    pEventQueue->buffer[pEventQueue->tail] = pEvent;
    pEventQueue->tail = (pEventQueue->tail + 1) % MAX_QUEUE_SIZE;
    if (pEventQueue->tail == pEventQueue->head)
    {
        pEventQueue->isFull = true;
    }
    // TODO implement double press support
    return 0;
}

GeneralizedEvent* dequeue(EventQueue* pEventQueue, UniversalKeyStatus* pStatusTable)
{
    bool isEmpty = !pEventQueue->isFull && pEventQueue->head == pEventQueue->tail;
    if (isEmpty)
    {
        printf("Queue is empty!\n");
        return NULL;
    }

    GeneralizedEvent* pEvent = pEventQueue->buffer[pEventQueue->head];
    pEventQueue->buffer[pEventQueue->head] = NULL;
    pEventQueue->isFull = false;
    pEventQueue->head = (pEventQueue->head + 1) % MAX_QUEUE_SIZE;
    // TODO implement double press support
    return pEvent;
}