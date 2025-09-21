#include "./../header/keyHandler.h"

int handleMacEvent(layers* pLayerEntries, generalizedEvent* pMacEvent, lookUpTables* pLookUpTables)
{
    setFlagsFromMac(pMacEvent->eventFlagMask, &pMacEvent->eventFlagMask);
    setCodeFromMac(pMacEvent->keyCode, &pMacEvent->keyCode, pLookUpTables->pOSToWeb);
    int e = handleEvent(pLayerEntries, pMacEvent, pLookUpTables->pWebToOS);
    setFlagsToMac(pMacEvent->eventFlagMask, &pMacEvent->eventFlagMask);
    setCodeToMac(pMacEvent->keyCode, &pMacEvent->keyCode, pLookUpTables->pWebToOS);
    return e;
}

int handleEvent(layers* pLayerEntries, generalizedEvent* pWebEvent, int* pWebToOSLookUp)
{
    keyData* keyEntry = &pLayerEntries->pRemapTable[pWebEvent->keyCode];
    if (keyEntry->isPressed)
    {
        pWebEvent->keyCode = keyEntry->keyCodeOnPress;
        return 1;
    }
    return 0;
}