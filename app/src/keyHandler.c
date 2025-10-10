#include "./../header/keyHandler.h"

int handleEvent(Layer* layerList, GeneralizedEvent* event, LookUpTables* lookUpTables)
{   
    if (!event) return kKREventSupress;
    printf("  pressed kr code: %d\n", event->code);
    int e = 0;

    EventQueue*  eventQueue = lookUpTables->eventQueue;
    KRKeyData*   remapTable = layerList->remapTable;
    KRKeyStatusTable* statusTable = lookUpTables->statusTable;
    enqueue(event, eventQueue);
    GeneralizedEvent* headEvent = getEvent(eventQueue, HEAD);
    
    // handle inoming event //
    
    if (event->keyDown)
    {
        if (remapTable[event->code].codeOnHold != NO_VALUE)
        {
            event->state = PENDING;
        }
    }    

    // handle outgoing event //

    if (headEvent->state == PENDING &&
        getTimeStamp() - headEvent->timeStampOnPress > U_SEC_FOR_ON_HOLD_EVENT)
    {
        headEvent->code = remapTable[headEvent->code].codeOnHold;
        headEvent->state = SEND;
    }
    else
    {
        if (remapTable[headEvent->code].codeOnPress != NO_VALUE)
        {
            headEvent->code = remapTable[headEvent->code].codeOnPress;
        }
    }

    if (headEvent->isModifier)
    {
        if (headEvent->flagMask != 0) // keyDown
        {
            statusTable->activeFlags |= headEvent->flagMask;
        }
        else
        {
            statusTable->activeFlags &= headEvent->flagMask;
        }
    }
    
    headEvent->flagMask = statusTable->activeFlags;
    return 0;
}

int handleMacEvent(Layer* layerList, GeneralizedEvent* macEvent, LookUpTables* lookUpTables)
{
    enqueue(macEvent, lookUpTables->eventQueue);
    return 0;
    printf("  flag %lld\n", macEvent->flagMask);
    int e = 0;

    if ((e = setCodeFromMac(macEvent->code, &macEvent->code, lookUpTables->osToKR) == 1)) 
    {
        printf("No remap for this key\n");
        return 0;
    }
    setFlagsFromMac(macEvent->flagMask, &macEvent->flagMask);
    
    printf("  macEvent flag %lld\n", macEvent->flagMask);
    e = handleEvent(layerList, macEvent, lookUpTables);

    GeneralizedEvent* headEvent = getEvent(lookUpTables->eventQueue, HEAD);
    printf("  headEvent flag %lld\n", headEvent->flagMask);
    setCodeToMac(headEvent->code, &macEvent->code, lookUpTables->krToOS);
    setFlagsToMac(headEvent->flagMask, &macEvent->flagMask);

    return e;
}

uint64_t getTimeStamp()
{
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    return (uint64_t)timeStamp.tv_sec * 1000000ULL + (uint64_t)timeStamp.tv_usec; // ULL - unsigned long long (must do arethmetic in long,,, its good no)
}