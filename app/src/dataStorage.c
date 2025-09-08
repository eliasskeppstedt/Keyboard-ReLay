#include "./../header/dataStorage.h"

struct keyData* createRemapTable(enum USER_OS os)
{
    cJSON* remapTableObject = readJSON();   
    if (!remapTableObject) 
    { 
        printf("Could not create remap table (cjson object)"); 
        return NULL; 
    }
    struct keyData* pRemapTable = malloc(sizeof(struct keyData) * AMOUNT_OF_KEYCODES); 
    cJSON* layers = cJSON_GetObjectItem(remapTableObject, "layers"); 
    if (!layers)
    {
        printf("Could not locate layers");
        return NULL;
    }
    int layerCount = 0;
    cJSON* layer = NULL;
    cJSON_ArrayForEach(layer, layers)
    {
        layerCount++;
    }
    // initialize relevant remap table spaces //
    printf("Initializing remap table...\n");
    for (int i = 0; i < AMOUNT_OF_KEYCODES; i++)
    {
        pRemapTable[i].pMacKeyCodeRemapOnPress = malloc(sizeof(int) * layerCount);
        pRemapTable[i].pMacKeyCodeRemapOnHold = malloc(sizeof(int) * layerCount);
        char layerNameBuf[7]; // l a y e r X \n
        pRemapTable[i].layerName = layerNameBuf;
        pRemapTable[i].isPressed = false;
        pRemapTable[i].layerNr = NO_LAYER;
        pRemapTable[i].layerName = NULL;
        //pRemapTable[i].comboKey = COMBO_KEY_NOT_SET;
    }
    // populate remap table //
    printf("Populating remap table...\n");
    cJSON_ArrayForEach(layer, layers)
    {
        // names MUST match json file in order to work (maybe should implement error handling (if item is NULL))
        int layerNr = cJSON_GetObjectItemCaseSensitive(layer, "layerNr")->valueint;
        char* layerName = cJSON_GetObjectItemCaseSensitive(layer, "layerName")->string;
        cJSON* macMapping = cJSON_GetObjectItem(layer, "macMapping");
        cJSON* remapsOnPress = cJSON_GetObjectItem(macMapping, "remapsOnPress");
        cJSON* remapsOnHold = cJSON_GetObjectItem(macMapping, "remapsOnHold");
        updateForKeyInLayerRemap(ON_PRESS, remapsOnPress, pRemapTable, layerNr, layerName);
        updateForKeyInLayerRemap(ON_HOLD, remapsOnHold, pRemapTable, layerNr, layerName);
    } 
    return pRemapTable;
}

void updateForKeyInLayerRemap(enum REMAP_MODES remap, cJSON* remaps, struct keyData* pRemapTable, int layerNr, char* layerName)
{  
    int keyCodeIndex = NO_KEY;
    int keyCodeValue = NO_KEY;
    cJSON* key = NULL;
    cJSON_ArrayForEach(key, remaps)
    {
        keyCodeIndex = cJSON_GetObjectItemCaseSensitive(key, "from")->valueint;
        keyCodeValue = cJSON_GetObjectItemCaseSensitive(key, "to")->valueint;
        pRemapTable[keyCodeIndex].macKeyCode = keyCodeIndex;
        if (remap == ON_PRESS)
        {
            pRemapTable[keyCodeIndex].pMacKeyCodeRemapOnPress[layerNr] = keyCodeValue;
        }
        else if (remap == ON_HOLD)
        {
            pRemapTable[keyCodeIndex].pMacKeyCodeRemapOnPress[layerNr] = keyCodeValue;
        }
        pRemapTable[keyCodeIndex].layerNr = layerNr;
        pRemapTable[keyCodeIndex].layerName = layerName;
        printKey(pRemapTable, keyCodeIndex, layerNr); 
    }
}

void printKey(struct keyData* pRemapTable, int keyCodeIndex, int layerIndex)
{
    printf("key:\n\
        - layerNr: %i\n\
        - layerName: %s\n\
        - macKeyCode: %i\n\
        - macKeyCodeOnPress: %i\n\
        - macKeyCodeOnHold: %i\n\
        - isPressed: %s\n", 
        pRemapTable[keyCodeIndex].layerNr,
        pRemapTable[keyCodeIndex].layerName,
        pRemapTable[keyCodeIndex].macKeyCode,
        pRemapTable[keyCodeIndex].pMacKeyCodeRemapOnPress[layerIndex],
        pRemapTable[keyCodeIndex].pMacKeyCodeRemapOnHold[layerIndex],
        pRemapTable[keyCodeIndex].isPressed ? "true" :  "false"
    );
}

cJSON* readJSON() 
{
    FILE *pF = fopen("./mapping.json", "r");

    if (pF == NULL) 
    {
        printf("Could not open json file!");
        return NULL;
    }

    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), pF);
    fclose(pF);

    // parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) 
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) 
        {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return NULL;
    }
    return json;
}