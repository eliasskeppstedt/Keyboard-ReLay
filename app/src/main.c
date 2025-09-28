#include "./../header/main.h"

int main()
{
    int os = MACOS;
    int e = 0;
    if (os == MACOS)
    {
        e = macMain();
    }
    if (e == EXIT_CODE_DEBUG)
    {
        printf("found the bug yet?!?!?!?!?!?\n");
    }
    else if (e == EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED)
    {
        printf("Could not create look up table");
    }
    else if (e == EXIT_CODE_CREATE_LAYER_ENTRIES_FAILED)
    {
        printf("Could not create layer entries");
    }
    else if (e != 0)
    {
        printf("oops error :D\n");
    }
    printf("Exiting program...\n");
    return 0;
}

int macMain()
{
    LookUpTables lookUpTables;
    if ((createLookUpTables(&lookUpTables, MACOS)) != 0) 
    {
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }
    EventQueue eventQueue;
    if ((createEventQueue(&eventQueue)) != 0) 
    {
        return -1;// EXIT_CODE_CREATE_EVENT_QUEUE_FAILED;
    }
    lookUpTables.pEventQueue = &eventQueue;

    Layers* pLayerEntries = malloc(sizeof(Layers) * lookUpTables.universalKeyEntries);
    if ((createLayerEntries(pLayerEntries, lookUpTables.universalKeyEntries)) != 0) 
    {
        return EXIT_CODE_CREATE_LAYER_ENTRIES_FAILED;
    }
    int entries = lookUpTables.universalKeyEntries;
    printf("Key lookup tables");
    for (int i = 0; i < lookUpTables.universalKeyEntries; i++)
    {
        printf("    pUniversalToOSLookUp[%d]: %d, pOSToUniversal[%d]: %d\n", i, lookUpTables.pUniversalToOS[i], i, lookUpTables.pOSToUniversal[i]);
    }
    
    int e = macStartMonitoring(pLayerEntries, &lookUpTables);
    exit:
    return e;
}