#include "./../header/main.h"

int main()
{
    int e = macMain();
    if (e != 0)
    {
        printf("Handling error... exit main :D\n");
        return e;
    }
    printf("Exiting program\n");
    return 0;
}

int macMain()
{
    int e;
    lookUpTables lookUpTables = { NULL, NULL, NO_VALUE, NO_VALUE };
    e = createLookUpTables(&lookUpTables, JSON_LABEL_MACOS);
    if (e != 0)
    {
        printf("Could not create look up table");
        return e;
    }
    layers* pLayerEntries = createLayerEntries(lookUpTables.webEntries, "mac");
    // would it be better here to store the layerEntries struct and just pass a reference and free
    // the heap allocated struct?
    if (!pLayerEntries)
    {
        printf("Could not create layer entries");
        return -1;
    }
    printf("pWebToOS[8] should be 34, is: %d\n", lookUpTables.pWebToOS[8]);
    printf("pOSToMac[34] should be 8, is: %d\n", lookUpTables.pOSToWeb[34]);
    printf("pRemapTable[65].keyCode should be -1, is: %d\n", pLayerEntries->pRemapTable[65].keyCode);
    printf("pRemapTable[1].keyCode should be 1, is: %d\n", pLayerEntries->pRemapTable[1].keyCode);
    e = macStartMonitoring(pLayerEntries, &lookUpTables);
    return e;
}