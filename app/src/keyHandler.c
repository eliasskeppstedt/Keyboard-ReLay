#include "./../header/keyHandler.h"

/**
 * 
 */
int handleEvent(Layers* pLayerEntries, GeneralizedEvent* pUniversalEvent, LookUpTables* pLookUpTables)
{ 
    if (!(pUniversalEvent->code >= 0 && pUniversalEvent->code < pLookUpTables->universalKeyEntries))
    {   
        printf("DEBUG [1] Index error\n"/*, e*/);
        return EXIT_CODE_INDEX_ERROR;
    };

    struct timeval timeStampOnPress;
    gettimeofday(&timeStampOnPress, NULL);
    uint32_t timeStampOnPressUSec = timeStampOnPress.tv_usec;
    
    UniversalKeyData* keyData = &pLayerEntries->pRemapTable[pUniversalEvent->code];
    if (pUniversalEvent->keyDown)
    {
        printf("DEBUGG Key \"%d\" was pressed\n", pUniversalEvent->code);
        printf("DEBUGG code %d\n", keyData->code);
        printf("DEBUGG onPress %d\n", keyData->codeOnPress);
        printf("DEBUGG onHold %d\n", keyData->codeOnHold);
    }
    for (int i = 0; i < pLookUpTables->universalKeyEntries; i++)
    {
        printf("%d ", pLayerEntries->pRemapTable[i].codeOnHold);
    }
    

    if (keyData[pUniversalEvent->code].codeOnPress != NO_VALUE) // there exist a on press remap code
    {
        pUniversalEvent->code = keyData->codeOnPress;
    }
    return 0;
}

int handleMacEvent(Layers* pLayerEntries, GeneralizedEvent* pMacEvent, LookUpTables* pLookUpTables)
{
    // different name just for clarity, use to indicate that the event info now has
    // the "universal" information about the OS key
    GeneralizedEvent* pUniversalEvent = pMacEvent;

    setFlagsFromMac(pMacEvent->eventFlagMask, &pUniversalEvent->eventFlagMask);
    setCodeFromMac(pMacEvent->code, &pUniversalEvent->code, pLookUpTables->pOSToUniversal);
    // printf("DEBUG pOSToUniversalLookUp[osCode] = universalCode\n");
    // printf("DEBUG                      ^%d       ^%d\nDEBUG\n", pUniversalEvent->code, pLookUpTables->pOSToUniversal[pUniversalEvent->code]);

    int e = handleEvent(pLayerEntries, pUniversalEvent, pLookUpTables);
    
    setFlagsToMac(pUniversalEvent->eventFlagMask, &pMacEvent->eventFlagMask);
    setCodeToMac(pUniversalEvent->code, &pMacEvent->code, pLookUpTables->pUniversalToOS);

    return e;
}