#include "./../header/keyHandler.h"

int handleEvent(Layer* layerList, GeneralizedEvent* event, LookUpTables* lookUpTables)
{   
    if (!event) return kKREventSupress;
    printf("kr code: %d\n", event->code);
    
    int e = 0;

    EventQueue*  eventQueue = lookUpTables->eventQueue;
    KRKeyData*   remapTable = layerList->remapTable;

    enqueue(event, eventQueue);
    GeneralizedEvent* headEvent = getEvent(eventQueue, HEAD);
    printf("  isHoldEvent: %s, %llu\n", getTimeStamp() - headEvent->timeStampOnPress > U_SEC_FOR_ON_HOLD_EVENT ? "true" : "false", getTimeStamp() - headEvent->timeStampOnPress);
    
    // NO SUPPORT FOR MULTIPLE KEYS FOR SAME CODES
    if (headEvent->state != PENDING) goto notPending;

    bool isHoldEvent = getTimeStamp() - headEvent->timeStampOnPress > U_SEC_FOR_ON_HOLD_EVENT;
    if (isHoldEvent)
    {   // headEvent already in queue, just modify its code
        headEvent->code = remapTable[headEvent->code].codeOnHold;
        headEvent->state = SEND;
    }
    else
    {
        if (headEvent->code == event->code)
        {   // since this happens before the threshold is met, event will be keyUp eqvivalent to the 
            // headEvent keyDown, send this combo as first when dequeuing
            headEvent = dequeue(eventQueue);
            event = dequeueFromTail(eventQueue);
            event->state = SEND;
            headEvent->state = SEND;
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
    }
    
    return 0;
}

int handleMacEvent(Layer* layerList, GeneralizedEvent* macEvent, LookUpTables* lookUpTables)
{
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

    e = handleEvent(layerList, krEvent, lookUpTables);
    
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