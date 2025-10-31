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
    int rlEntries = cJSON_GetNumberValue(cRLEntries);
    int osEntries = cJSON_GetNumberValue(cOSEntries);
    int layerEntries = cJSON_GetNumberValue(cLayerEntries);

    MyReLay myReLay;

    EventQueue eventQueue; 
    initEventQueue(&eventQueue);

    KeyStatus statusTable[rlEntries];
    for (int i = 0; i < rlEntries; i++) 
    {
        statusTable[i] = (KeyStatus) {
            .keyDown = false,
            .keysDown = 0
        };
    }

    int* osToRL = malloc(sizeof(int) * osEntries);
    int* rlToOS = malloc(sizeof(int) * rlEntries);
    initCodeConverters(cKeys, osToRL, rlToOS, osEntries, rlEntries, MACOS);

    KeyInfo** remapTable = NULL;
    initRemapTable(cRemaps, &remapTable, layerEntries, rlEntries);

    myReLay = (MyReLay) {
        .remapTable = remapTable,
        .activeLayer = 0,
        .statusTable = statusTable,
        .eventQueue = eventQueue,
        .osToRL = osToRL,
        .rlToOS = rlToOS,
        .osKeyEntries = osEntries,
        .rlKeyEntries = rlEntries
    };
    for (int i = 0; i < osEntries; i++)
    {
        printf("osToRL: %d -> %d\n", i, osToRL[i]);
    }

    cJSON_Delete(cKeys);
    cJSON_Delete(cRemaps);

    start(&myReLay);
}

void cleanup()
{
    printf("CLEANUP :D \n");
}