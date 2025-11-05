#include "./../header/storeData.h"

void initEventQueue(EventQueue* queue)
{
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        queue->buffer[i] = NULL;
    }
    queue->head = 0;
    queue->tail = 0;
    queue->isFull = false;
}

void initCodeConverters(cJSON* json, OS os)
{
    osToRL = malloc(sizeof(int) * osKeyEntries);
    rlToOS = malloc(sizeof(int) * rlKeyEntries);
    for (int i = 0; i < osKeyEntries; i++)
    {
        osToRL[i] = NO_VALUE;
    }
    for (int i = 0; i < rlKeyEntries; i++)
    {
        rlToOS[i] = NO_VALUE;
    }
    
    char* osCodeString;
    if (os == MACOS)
    {
        osCodeString = "mac";
    }
    
    cJSON* cKeys = cJSON_GetObjectItem(json, "keysRL");
    if (!cKeys) exit(101);
    cJSON* cKey = NULL;
    cJSON* cKeyCode = NULL;
    cJSON* cRelayCode = NULL;
    cJSON* cOSCode = NULL;

    cJSON_ArrayForEach(cKey, cKeys) {
        cKeyCode = cJSON_GetObjectItem(cKey, "keyCode");
        if (!cKeyCode) exit(102);

        cRelayCode = cJSON_GetObjectItem(cKeyCode, "rl");
        if (!cRelayCode) exit(103);
        cRelayCode = cJSON_GetObjectItem(cRelayCode, "dec");
        if (!cRelayCode) exit(104);

        cOSCode = cJSON_GetObjectItem(cKeyCode, osCodeString);
        if (!cOSCode) exit(105);
        cOSCode = cJSON_GetObjectItem(cOSCode, "dec");
        if (!cOSCode) exit(106);

        int relayCode = cJSON_GetNumberValue(cRelayCode);
        int osCode = cJSON_GetNumberValue(cOSCode);

        rlToOS[relayCode] = osCode;
        osToRL[osCode] = relayCode;
    }
    printf("%d\n", osToRL[24]);
    printf("%d\n", rlToOS[24]);
}

void initRemapTable(cJSON* json)
{
    remapTable = malloc(sizeof(KeyInfo*) * layerEntries);
    for (int i = 0; i < layerEntries; i++)
    {
        remapTable[i] = malloc(sizeof(KeyInfo) * rlKeyEntries);
    }

    cJSON* cLayers = cJSON_GetObjectItem(json, "layers");
    if (!cLayers) exit(301);

    for (int i = 0; i < layerEntries; i++)
    {
        for (int j = 0; j < rlKeyEntries; j++)
        {
            remapTable[i][j] = (KeyInfo) {
                .code = NO_VALUE,
                .codeOnPress = NO_VALUE,
                .codeOnHold = NO_VALUE
            };
        }
    }

    cJSON* cLayer = NULL;
    cJSON* cLayerNr = NULL;
    cJSON* cCodes = NULL;
    cJSON* cCode = NULL;
    cJSON* cFrom = NULL;
    cJSON* cOnPress = NULL;
    cJSON* cOnHold = NULL;

    cJSON_ArrayForEach(cLayer, cLayers)
    {
        cLayerNr = cJSON_GetObjectItem(cLayer, "layerNr");
        if (!cLayerNr) exit(302);

        cCodes = cJSON_GetObjectItem(cLayer, "keyCode");
        if (!cCodes) exit(303);

        int layerNr = cJSON_GetNumberValue(cLayerNr);

        cJSON_ArrayForEach(cCode, cCodes)
        {
            cFrom = cJSON_GetObjectItem(cCode, "from");
            if (!cFrom) exit(304);
            cOnPress = cJSON_GetObjectItem(cCode, "toOnPress");
            if (!cOnPress) exit(305);
            cOnHold = cJSON_GetObjectItem(cCode, "toOnHold");
            if (!cOnHold) exit(306);

            int from = cJSON_GetNumberValue(cFrom);
            int onPress = cJSON_GetNumberValue(cOnPress);
            int onHold = cJSON_GetNumberValue(cOnHold);
            remapTable[layerNr][from] = (KeyInfo) {
                .code = from,
                .codeOnPress = onPress,
                .codeOnHold = onHold
            };
        }
    }
}

cJSON* readJSON(char* path) 
{
    FILE* f = fopen(path, "r");

    if (!f) 
    {
        // printf("Could not open json file! => ");
        return NULL;
    }

    char buffer[32768];
    fread(buffer, 1, sizeof(buffer), f);
    fclose(f);

    // parse the JSON data
    cJSON* json = cJSON_Parse(buffer);
    if (!json) 
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) 
        {
            // printf("Error: %s => ", error_ptr);
        }
        cJSON_Delete(json);
        return NULL;
    }
    return json;
}