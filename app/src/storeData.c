#include "./../header/storeData.h"

int createLookUpTables(lookUpTables* pLookUpTables, char* OS)
{
    // Create look up tables
    // works only if first web code is 0 and increses by 1 untill webEntries - 1
    char* stringOSEntries;
    if (strcmp(OS, JSON_LABEL_MACOS) == 0)
    {
        stringOSEntries = JSON_LABEL_MAC_ENTRIES;
    }
    
    cJSON* pJSON = readJSON("./../config/keys.json");
    if (!pJSON) { printf("!pJSON keys\n"); return -1; }
    int webEntries = cJSON_GetNumberValue(cJSON_GetObjectItem(pJSON, JSON_LABEL_WEB_ENTRIES));
    int osEntries = cJSON_GetNumberValue(cJSON_GetObjectItem(pJSON, stringOSEntries));
    cJSON* pKeys = cJSON_GetObjectItem(pJSON, JSON_LABEL_KEYS);
    if (!pKeys) { printf("!pKeys\n"); return -1; }
    
    int debuggerNum = 0;
    int* pWebToOSLookUp = malloc(sizeof(int) * webEntries);    
    int* pOStoWebLookUp = malloc(sizeof(int) * osEntries); 
    cJSON* pKey = NULL; 
    cJSON* pKeyCodes = NULL;
    cJSON* pWebKeyCodes = NULL;
    cJSON_ArrayForEach(pKey, pKeys)
    {
        pKeyCodes = cJSON_GetObjectItem(pKey, JSON_LABEL_KEY_CODE);
        if (!pKeyCodes) { printf("%d: !pKeyCodes\n", debuggerNum); return -1; }
        pWebKeyCodes = cJSON_GetObjectItem(pKeyCodes, JSON_LABEL_WEB);
        if (!pWebKeyCodes) { printf("%d: !pWebKeyCodes\n", debuggerNum); return -1; }
        pKeyCodes = cJSON_GetObjectItem(pKeyCodes, OS);
        if (!pKeyCodes) { printf("%d: !pKeyCodes\n", debuggerNum); return -1; }
        int webCode = cJSON_GetNumberValue(cJSON_GetObjectItem(pWebKeyCodes, JSON_LABEL_DECIMAL));
        int osCode = cJSON_GetNumberValue(cJSON_GetObjectItem(pKeyCodes, JSON_LABEL_DECIMAL));
        pWebToOSLookUp[webCode] = osCode; // *(pWebToOSLookUp + webCode) = osCode (since i work with pointers of size int, adding x to it will "move" the pointer forward to xth location)
        pOStoWebLookUp[osCode] = webCode;
    }
    cJSON_free(pJSON);
    cJSON_free(pKeys);
    cJSON_free(pKey);
    cJSON_free(pKeyCodes);
    cJSON_free(pWebKeyCodes);
    if (!pLookUpTables)
    {
        printf("Lookuptable object NUUUULLLLL\n");
    }
    
    pLookUpTables->pWebToOS = pWebToOSLookUp; // SEGMENTATION FAULT 11!
    pLookUpTables->pOSToWeb = pOStoWebLookUp;
    printf("1\n");
    return 1;
}

layers* createLayerEntries(char* OS, int webEntries)
{
    cJSON* pJSON = readJSON("./../config/remaps.json");
    if (!pJSON) { printf("!pJSON remaps\n"); return NULL; }
    cJSON* pLayers = cJSON_GetObjectItem(pJSON, JSON_LABEL_LAYERS);
    if (!pLayers) { printf("!pLayers\n"); return NULL; }
    int layerEntries = cJSON_GetNumberValue(cJSON_GetObjectItem(pLayers, JSON_LABEL_LAYER_ENTRIES));
    layers* pLayerEntries = malloc(sizeof(layers) * layerEntries);
    for (int i = 0; i < layerEntries; i++)
    {
        pLayerEntries[i].layerName = NULL;
        pLayerEntries[i].layerNr = NO_VALUE; // -1
        pLayerEntries[i].pRemapTable = NULL;
    }
    
    cJSON* pKeyCodes = NULL;
    //cJSON* pOnHold = NULL;
    cJSON* pLayer = NULL;   
    int debuggerNum = 0;
    cJSON_ArrayForEach(pLayer, pLayers)
    {
        keyData* pRemapTable = createRemapTable(webEntries);
        char* layerName = cJSON_GetStringValue(cJSON_GetObjectItem(pLayer, JSON_LABEL_LAYER_NAME));
        int layerNr = cJSON_GetNumberValue(cJSON_GetObjectItem(pLayer, JSON_LABEL_LAYER_NAME));
        pKeyCodes = cJSON_GetObjectItem(pLayer, JSON_LABEL_KEY_CODE);
        if (!pKeyCodes) { printf("%d: !pKeyCodes\n", debuggerNum); return NULL; }
        int from = cJSON_GetNumberValue(cJSON_GetObjectItem(pKeyCodes, JSON_LABEL_FROM));
        int toOnPress = cJSON_GetNumberValue(cJSON_GetObjectItem(pKeyCodes, JSON_LABEL_TO_ON_PRESS));
        int toOnHold = cJSON_GetNumberValue(cJSON_GetObjectItem(pKeyCodes, JSON_LABEL_TO_ON_HOLD));
        pRemapTable[from].keyCode = from;
        pRemapTable[from].keyCodeOnPress = toOnPress;
        pRemapTable[from].keyCodeOnHold = toOnHold;
        pLayerEntries[layerNr].layerName = layerName;
        pLayerEntries[layerNr].layerNr = layerNr;
        pLayerEntries[layerNr].pRemapTable = pRemapTable;
    }

    cJSON_free(pJSON);
    cJSON_free(pLayer);
    cJSON_free(pLayers);
    cJSON_free(pKeyCodes);

    return pLayerEntries;
}

keyData* createRemapTable(int webEntries)
{
    keyData* pRemapTable = malloc(sizeof(keyData) * webEntries);
    for (int i = 0; i < webEntries; i++)
    {
        pRemapTable[i].isPressed = false;
        pRemapTable[i].keyCode = NO_VALUE;
        pRemapTable[i].keyCodeOnPress = NO_VALUE;
        pRemapTable[i].keyCodeOnHold = NO_VALUE;
    }
    
    return pRemapTable;
}

cJSON* readJSON(char* path) 
{
    FILE* pF = fopen(path, "r");

    if (!pF) 
    {
        printf("Could not open json file!\n");
        return NULL;
    }

    char buffer[32768];
    fread(buffer, 1, sizeof(buffer), pF);
    fclose(pF);

    // parse the JSON data
    cJSON* pJSON = cJSON_Parse(buffer);
    if (!pJSON) 
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) 
        {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(pJSON);
        return NULL;
    }
    return pJSON;
}