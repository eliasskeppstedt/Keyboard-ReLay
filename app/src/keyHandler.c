#include "./../header/keyHandler.h"

int handleEvent(Layer* layerList, GeneralizedEvent* event, LookUpTables* lookUpTables)
{   
    printf("  handleEvent:\n");
    if (!event) return kKREventSupress;
    printf("  kr code: %d\n", event->code);
    int e = 0;

    EventQueue*  eventQueue = lookUpTables->eventQueue;
    KRKeyStatus* statusTable = lookUpTables->statusTable;   
    KRKeyData*   remapTable = layerList->remapTable;

    // if (lookUpTables->statusTable[event->code].state == PENDING) return kKREventSupress; // logical error

    //statusTable[event->code].keyDown = event->keyDown;

    enqueue(event, eventQueue, statusTable);
    GeneralizedEvent* headEvent = getEvent(eventQueue, HEAD);
    KRKeyData* dataForPressedKey = &remapTable[headEvent->code];
    //printf("data for key: code: %d\n", dataForPressedKey->code);
    //printf("  Timestamp enqueued event: %llu\n", event->timeStampOnPress);
    //printf("  Timestamp dequeued event: %llu\n", event->timeStampOnPress);
    if (!headEvent->keyDown)
    {
        /*if (hasCodeOnHold(remapTable, headEvent)) // then CAN become hold event
        {
            printf("  keyUpp \n");
        }*/
        statusTable[headEvent->code].state = NORMAL;
        return kKREventTypeKeyUp;
    }
    if (hasCodeOnHold(remapTable, headEvent)) // then CAN become hold event
    {

        /*if (getTimeStamp() - headEvent->timeStampOnPress < U_SEC_FOR_ON_HOLD_EVENT)
        {
            statusTable[headEvent->code].state = PENDING;
            return kKREventSupress;
        }
        
        headEvent->code = remapTable[headEvent->code].codeOnPress;*/
    }
    else if (hasCodeOnPress(remapTable, headEvent)) // has onPress remap
    {
    }
    else if (statusTable[headEvent->code].state == NORMAL) // just pass as normal key press
    {
    }

    /*printf("  DEBUGG Key \"%d\" was pressed\n", event->code);
    printf("  DEBUGG code %d\n", remapTable[headEvent->code].code);
    printf("  DEBUGG onPress %d\n", remapTable[headEvent->code].codeOnPress);
    printf("  DEBUGG onHold %d\n", remapTable[headEvent->code].codeOnHold);*/
    return kKREventTypeKeyDown;
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
    setFlagsFromMac(macEvent->eventFlagMask, &krEvent->eventFlagMask);

    e = handleEvent(layerList, krEvent, lookUpTables);
    
    setCodeToMac(krEvent->code, &macEvent->code, lookUpTables->krToOS);
    setFlagsToMac(krEvent->eventFlagMask, &macEvent->eventFlagMask);

    return e;
}

uint64_t getTimeStamp()
{
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    return (uint64_t)timeStamp.tv_sec * 1000000ULL + (uint64_t)timeStamp.tv_usec; // ULL - unsigned long long (must do arethmetic in long,,, its good no)
}