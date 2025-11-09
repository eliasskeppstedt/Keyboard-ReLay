#include "./../header/main.h"

int main()
{
    atexit(cleanup);

    cJSON* cKeys = readJSON("./config/keys.json");
    if (!cKeys) return ERROR_READ_JSON;
    cJSON* cRemaps = readJSON("./config/remaps.json");
    if (!cRemaps) return ERROR_READ_JSON;
    cJSON* cRLEntries = cJSON_GetObjectItemCaseSensitive(cKeys, "rlEntries");
    if (!cRLEntries) return 1;
    cJSON* cOSEntries = cJSON_GetObjectItemCaseSensitive(cKeys, "macEntries");
    if (!cOSEntries) return 1;
    cJSON* cLayerEntries = cJSON_GetObjectItemCaseSensitive(cRemaps, "layerEntries");
    if (!cLayerEntries) return 1;

    rlKeyEntries = cJSON_GetNumberValue(cRLEntries);
    osKeyEntries = cJSON_GetNumberValue(cOSEntries);
    layerEntries = cJSON_GetNumberValue(cLayerEntries);

    MyReLay myReLay;
    EventQueue eventQueue; 
    initEventQueue(&eventQueue);
    initCodeConverters(cKeys, MACOS);
    initRemapTable(cRemaps);
    KeyStatus statusTable[osKeyEntries];
    for (int i = 0; i < osKeyEntries; i++) 
    {
        statusTable[i] = (KeyStatus) {
            .code = NO_VALUE,
            .osCode = NO_VALUE,
            .keyDown = false,
            .keysDown = 0,
            .originalCodeDown = NO_VALUE
        };
    }

    myReLay = (MyReLay) {
        .statusTable = statusTable,
        .eventQueue = eventQueue,
        .activeLayer = 0
    };
    for (int i = 0; i < osKeyEntries; i++)
    {
        printf("osToRL: %d -> %d\n", i, osToRL[i]);
    }
    for (int i = 0; i < rlKeyEntries; i++)
    {
        printf("remapTable[0][%d].codeOnHold: %d \n", i, remapTable[0][i].codeOnHold);
    }
    

    cJSON_Delete(cKeys);
    cJSON_Delete(cRemaps);

    start(&myReLay);
}

void cleanup()
{
    printf("CLEANUP :D \n");
}