#include "./../header/keyHandler.h"

int handleEvent(Layer* layerList, GeneralizedEvent* event, LookUpTables* lookUpTables)
{   
    printf("  uni code:%d\n", event->code);
    int e = 0;
    EventQueue*         eventQueue = lookUpTables->eventQueue;
    UniversalKeyStatus* statusTable = lookUpTables->statusTable;   
    UniversalKeyData*   remapTable = layerList->remapTable;
    if (statusTable[event->code].keyDown &&
        event->timeStampOnPress - statusTable[event->code].timeStampOnPress < TIME_FOR_AUTOREPEAT_DETECTION)
    {
        return kKRSimulatedEventAutorepeat;
    }
    printf("  head: %d\n", eventQueue->head);
    printf("  tail: %d\n", eventQueue->tail);
    statusTable[event->code].keyDown = event->keyDown;
    statusTable[event->code].timeStampOnPress = event->timeStampOnPress;
    statusTable[event->code].flagMask = event->eventFlagMask;
    enqueue(event, eventQueue, statusTable);
    GeneralizedEvent* headEvent = getEvent(eventQueue, HEAD);
    UniversalKeyData* dataForPressedKey = &remapTable[headEvent->code];
    printf("data for key: code: %d\n", dataForPressedKey->code);
    GeneralizedEvent* tailEvent = getEvent(eventQueue, eventQueue->tail);
    printf("  Timestamp enqueued event: %llu\n", event->timeStampOnPress);
    printf("  Timestamp dequeued event: %llu\n", event->timeStampOnPress);
    
    if (!headEvent->keyDown)
    {
        // skip hold logic for now
        if (statusTable[headEvent->code].state == REMAP_ON_HOLD_POTENTIAL) return kKREventTypeKeyUp;
        event = dequeue(eventQueue, statusTable);
        statusTable[event->code].state = NORMAL;

        /*if (statusTable[dequeuedEvent->code].state == PENDING) 
        {
            //remove modifers from dequeuedEvent
        }*/
        return kKREventTypeKeyUp;
    }
    if (statusTable[headEvent->code].state == REMAP_ON_HOLD_POTENTIAL) // then CAN become hold event
    {
        /*
        printf("  1.       REMAP_ON_HOLD_POTENTIAL\n");
        if (headEvent->timeStampOnPress - getTimeStamp() < TIME_FOR_ON_HOLD_EVENT_U_SEC)
        {
        printf("  2.       REMAP_ON_HOLD_POTENTIAL\n");
            statusTable[headEvent->code].state = NORMAL;
            // TODO set flags
            return kKRSimulatedEventAutorepeat; // change later, currently makes for no simulated presses
        }
        printf("  3.       REMAP_ON_HOLD_POTENTIAL\n");
        event = dequeue(eventQueue, statusTable);
        //statusTable[headEvent->code].state = ACTIVE;
        //dequeuedEvent->code = dataForPressedKey->codeOnHold;
        */
    }
    else if (remapTable[headEvent->code].codeOnPress != NO_VALUE) // has onPress remap
    {
        printf("  4.       codeOnPress\n");
        event = dequeue(eventQueue, statusTable);
        event->code = dataForPressedKey->codeOnPress;
    }
    else if (statusTable[headEvent->code].state == NORMAL) // just pass as normal key press
    {
        event = dequeue(eventQueue, statusTable);
    }

    printf("  DEBUGG Key \"%d\" was pressed\n", event->code);
    printf("  DEBUGG code %d\n", dataForPressedKey->code);
    printf("  DEBUGG onPress %d\n", dataForPressedKey->codeOnPress);
    printf("  DEBUGG onHold %d\n", dataForPressedKey->codeOnHold);
    return kKREventTypeKeyDown;
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