#include "./../header/main.h"

int main()
{
    atexit(cleanup);

    cJSON* cKeys = readJSON("./config/keys.json");
    if (!cKeys) return ERROR_READ_JSON;
    cJSON* cRemaps = readJSON("./config/remaps.json");
    if (!cRemaps) return ERROR_READ_JSON;
    cJSON* cEntries = cJSON_GetObjectItemCaseSensitive(cKeys, "rlEntries");
    if (!cEntries) return 1;
    cJSON* cLayerEntries = cJSON_GetObjectItemCaseSensitive(cKeys, "rlEntries");
    if (!cLayerEntries) return 1;
    int entries = cJSON_GetNumberValue(cEntries);
    int layerEntries = cJSON_GetNumberValue(cLayerEntries);

    MyReLay myReLay;

    EventQueue eventQueue; 
    initEventQueue(&eventQueue);

    bool keyDownTable[entries];
    for (int i = 0; i < entries; i++) keyDownTable[i] = false;

    int osToRL[entries];
    int rlToOS[entries];
    initCodeConverters(cKeys, &osToRL[0], &rlToOS[0], entries, MACOS);

    KeyInfo** remapTable = NULL;
    initRemapTable(cRemaps, &remapTable, layerEntries, entries);

    myReLay.remapTable = remapTable;
    myReLay.activeLayer = 0;
    myReLay.keyDownTable = keyDownTable;
    myReLay.eventQueue = eventQueue;
    myReLay.osToRL = osToRL;
    myReLay.rlToOS = rlToOS;
    myReLay.keyEntries = entries;

    cJSON_Delete(cKeys);
    cJSON_Delete(cRemaps);
}

void cleanup()
{
    printf("CLEANUP :D \n");
}