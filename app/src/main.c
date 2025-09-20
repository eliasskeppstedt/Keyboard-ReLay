#include "./../header/main.h"

int main()
{
    if (macMain() == -1)
    {
        printf("error :D");
        return -1;
    }
    return 1;
}

int macMain()
{
    lookUpTables lookUpTables = { NULL, NULL };
    // OS.MACOS
    if (createLookUpTables(&lookUpTables, JSON_LABEL_MACOS) == -1)
    {
        printf("error :D\n");
        return -1;
    }
    int* pWebToOsCodeLookUp = lookUpTables.pWebToOS;
    int* pOSToWebCodeLookUp = lookUpTables.pOSToWeb;
    int webEntries = sizeof(pWebToOsCodeLookUp);
    layers* pLayerEntries = createLayerEntries("mac", webEntries);
    if (!pLayerEntries)
    {
        printf("error :D\n");
        return -1;
    }
    if(!macStartMonitoring(pLayerEntries, pWebToOsCodeLookUp, pOSToWebCodeLookUp)) { printf("error\n"); return 1; }
    return 1;
}