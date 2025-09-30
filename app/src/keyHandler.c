#include "./../header/keyHandler.h"

int lastDetectedKeyCode = NO_VALUE;

int handleEvent(Layer* layerList, GeneralizedEvent* event, LookUpTables* lookUpTables)
{   
    int e = 0;
    EventQueue*         eventQueue = lookUpTables->eventQueue;
    UniversalKeyStatus* statusTable = lookUpTables->statusTable;   
    UniversalKeyData*   remapTable = layerList->remapTable;
    GeneralizedEvent*   lastEvent = getEvent(eventQueue, eventQueue->tail); 
    if (statusTable[event->code].keyWasDown &&
        event->timeStampOnPress - statusTable[event->code].timeStampOnPress < TIME_FOR_AUTOREPEAT_DETECTION)
    {
        return kKRSimulatedEventAutorepeat;
    }
    statusTable[event->code].keyWasDown = event->keyDown;

    
    enqueue(event, eventQueue, statusTable);
    
    /*if (lastEvent)
    { 
        printf("  in here plz...");
        uint64_t timeSinceLastEvent = event->timeStampOnPress - lastEvent->timeStampOnPress;
        bool sameKey = event->code == lastEvent->code;
        if (timeSinceLastEvent < TIME_FOR_AUTOREPEAT_DETECTION && sameKey)
        {
            return kKRSimulatedEventAutorepeat;
        }
    }*/
    //statusTable[event->code].keyDown = event->keyDown;
    //statusTable[event->code].timeStampOnPress = event->timeStampOnPress;

    GeneralizedEvent* firstEvent = getEvent(eventQueue, HEAD);
    lastEvent = getEvent(eventQueue, eventQueue->tail);
    printf("  Timestamp enqueued event: %llu\n", event->timeStampOnPress);
    event = dequeue(eventQueue, statusTable);
    printf("  Timestamp dequeued event: %llu\n", event->timeStampOnPress);
    
    if (!lastEvent->keyDown) goto keyUpp;
    if (remapTable[firstEvent->code].codeOnHold != NO_VALUE) // then CAN become hold event
    {
        if (lastEvent->timeStampOnPress - getTimeStamp() < TIME_FOR_ON_HOLD_EVENT_U_SEC)
        {
            statusTable[event->code].state = PENDING;
            return kKRSimulatedEventAutorepeat; // change later, currently makes for no simulated presses
        }
        statusTable[event->code].state = ACTIVE;
        event->code = remapTable[event->code].codeOnHold;
        return 0;
    }

    UniversalKeyData* dataForPressedKey = &layerList->remapTable[event->code];
    lookUpTables->statusTable[event->code].timeStampOnPress = event->timeStampOnPress;
    
    
    if (dataForPressedKey->codeOnPress != NO_VALUE) // there exist a on press remap code
    {    

        if (event->keyDown)
        {
            printf("  DEBUGG Key \"%d\" was pressed\n", event->code);
            printf("  DEBUGG code %d\n", dataForPressedKey->code);
            printf("  DEBUGG onPress %d\n", dataForPressedKey->codeOnPress);
            printf("  DEBUGG onHold %d\n", dataForPressedKey->codeOnHold);
        }
        event->code = dataForPressedKey->codeOnPress;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    keyUpp:
    // key on hold logic here ...
    return 0;
}

int handleMacEvent(Layer* layerList, GeneralizedEvent* macEvent, LookUpTables* lookUpTables)
{
    int e = 0;
    // different name just for clarity, use to indicate that the event info now has
    // the "universal" information about the OS key
    GeneralizedEvent* pUniversalEvent = macEvent;
    if ((e = setCodeFromMac(macEvent->code, &pUniversalEvent->code, lookUpTables->osToUniversal) == 1)) 
    {
        printf("No remap for this key\n");
        return 0;
    }
    setFlagsFromMac(macEvent->eventFlagMask, &pUniversalEvent->eventFlagMask);

    e = handleEvent(layerList, pUniversalEvent, lookUpTables);
    
    setCodeToMac(pUniversalEvent->code, &macEvent->code, lookUpTables->universalToOS);
    setFlagsToMac(pUniversalEvent->eventFlagMask, &macEvent->eventFlagMask);

    return e;
}

uint64_t getTimeStamp()
{
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    return (uint64_t)timeStamp.tv_sec * 1000000ULL + (uint64_t)timeStamp.tv_usec; // ULL - unsigned long long (must do arethmetic in long,,, its good no)
}