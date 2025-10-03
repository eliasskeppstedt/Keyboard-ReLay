#include "./../header/keyHandler.h"

int handleEvent(Layer* layerList, GeneralizedEvent* event, LookUpTables* lookUpTables)
{   
    if (!event) return kKREventSupress;
    printf("pressed kr code: %d\n", event->code);
    int e = 0;

    EventQueue*  eventQueue = lookUpTables->eventQueue;
    KRKeyData*   remapTable = layerList->remapTable;
    KRKeyStatusTable* statusTable = lookUpTables->statusTable;
    int oldActiveFlags = statusTable->activeFlags;
    //event->flagMask |= oldActiveFlags;
    GeneralizedEvent* headEvent = getEvent(eventQueue, HEAD);

    if (headEvent) goto notEmpty; // !isEmpty(eventQueue), will always be empty if no pending event
    printf("  is empty\n");
    // if it was empty, this event is a keyDown event
    enqueue(event, eventQueue);
    if (hasCodeOnHold(remapTable, event))
    {   
        event->state = event->state == NORMAL ? PENDING : SEND;
    }
    else if (hasCodeOnPress(remapTable, event))
    {
        event->code = remapTable[event->code].codeOnPress;
    }
    else if (event->isModifier) // key down, ADD the mask to the status, key up REMOVE mask from status
    {
        if (event->keyDown)
        {
            statusTable->activeFlags |= event->flagMask;
        }
        else
        {
            statusTable->activeFlags &= event->flagMask;
        }
        return 0;//kKREventFlagsChanged;
    }
    event->flagMask |= statusTable->activeFlags;
    return 0;

    notEmpty:
    printf("  queue was not empty\n");
    enqueue(event, eventQueue);
    headEvent = getEvent(eventQueue, HEAD);
    
    printf("  isHoldEvent: %s, %lld\n", getTimeStamp() - headEvent->timeStampOnPress > U_SEC_FOR_ON_HOLD_EVENT ? "true" : "false", getTimeStamp() - headEvent->timeStampOnPress);
    
    // NO SUPPORT FOR MULTIPLE KEYS FOR SAME CODES
    if (headEvent->state != PENDING) goto notPending;
    printf("  is pending %llu\n", headEvent->flagMask);

    bool isHoldEvent = getTimeStamp() - headEvent->timeStampOnPress > U_SEC_FOR_ON_HOLD_EVENT;
    if (isHoldEvent)
    {   // headEvent already in queue, just modify its code
        headEvent->code = remapTable[headEvent->code].codeOnHold;
        headEvent->flagMask = statusTable->activeFlags;
        headEvent->state = SEND;
    }
    else
    {
        if (headEvent->isModifier) // hmmmmmmmmmmmm
        {
            statusTable->activeFlags &= headEvent->flagMask;
            return 0;//kKREventFlagsChanged;
        }

        if (headEvent->code == event->code)
        {   // since this happens before the threshold is met, event will be keyUp eqvivalent to the 
            // headEvent keyDown, send this combo as first when dequeuing
            if (hasCodeOnPress(remapTable, event))
            {
                headEvent->code = remapTable[headEvent->code].codeOnPress;
                event->code = remapTable[event->code].codeOnPress;
            }

            headEvent = dequeue(eventQueue);
            event = dequeueFromTail(eventQueue);
            headEvent->state = SEND;
            event->state = SEND;
            headEvent->flagMask = statusTable->activeFlags;
            event->flagMask = statusTable->activeFlags;
            enqueueSqueezeToFront(event, eventQueue);
            enqueueSqueezeToFront(headEvent, eventQueue);
        }
    }
    return 0;

    notPending:

    if (event->keyDown) 
    {
        if (hasCodeOnHold(remapTable, event))
        {   
            event->state = event->state == NORMAL ? PENDING : SEND;
        }
        else if (hasCodeOnPress(remapTable, event))
        {
            event->code = remapTable[event->code].codeOnPress;
        }
        event->flagMask = statusTable->activeFlags;
        return 0;
    }
    if (headEvent->isModifier) // key down, hence switch OFF the modifier events mask from status
    {
        statusTable->activeFlags &= headEvent->flagMask;
        return 0;//kKREventFlagsChanged;
    }
    return 0;
}

int handleMacEvent(Layer* layerList, GeneralizedEvent* macEvent, LookUpTables* lookUpTables)
{
    printf("  flag %lld\n", macEvent->flagMask);
    int e = 0;
    // different name just for clarity, use to indicate that the event info now has
    // the "kr" information about the OS key
    GeneralizedEvent* krEvent = macEvent;
    if ((e = setCodeFromMac(macEvent->code, &krEvent->code, lookUpTables->osToKR) == 1)) 
    {
        printf("No remap for this key\n");
        return 0;
    }
    setFlagsFromMac(macEvent->flagMask, &krEvent->flagMask);
    
    printf("  flag %lld\n", krEvent->flagMask);
    e = handleEvent(layerList, krEvent, lookUpTables);
    krEvent = getEvent(lookUpTables->eventQueue, HEAD);
    printf("  flag %lld\n", krEvent->flagMask);
    setCodeToMac(krEvent->code, &macEvent->code, lookUpTables->krToOS);
    setFlagsToMac(krEvent->flagMask, &macEvent->flagMask);

    return e;
}

uint64_t getTimeStamp()
{
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    return (uint64_t)timeStamp.tv_sec * 1000000ULL + (uint64_t)timeStamp.tv_usec; // ULL - unsigned long long (must do arethmetic in long,,, its good no)
}