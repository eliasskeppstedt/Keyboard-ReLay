#include "./../header/storeData.h"
// for reading json, im using the relative path, change later? then ig change to absolute path
// based on download location for each OS

int createLookUpTables(LookUpTables* pLookUpTables, OS os)
{
    // printf("DEBUG storeData.c createLookUpTable\n");
    // printf("DEBUG cjson array for each to create look up table\n");
    // Create look up tables
    // works only if first universal code is 0 and increses by 1 untill universalKeyEntries - 1
    cJSON* pJSON = readJSON("./config/keys.json");
    if (!pJSON) 
    { 
        printf("[ ] !pJSON keys\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }

    cJSON* verifyObj = cJSON_GetObjectItem(pJSON, "universalEntries");
    if (!verifyObj) 
    { 
        printf("1 !universalKeyEntries\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    int universalKeyEntries = cJSON_GetNumberValue(verifyObj);
    // if (!universalKeyEntries) { printf("2 !universalKeyEntries\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }
    
    char* stringOSEntries;
    char* stringOS;
    if (os == MACOS)
    {
        stringOSEntries = "macEntries";
        stringOS = "mac";
    }

    verifyObj = cJSON_GetObjectItem(pJSON, stringOSEntries);
    if (!verifyObj) 
    { 
        printf("1 !osKeyEntries\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    int osKeyEntries = cJSON_GetNumberValue(verifyObj);
    // if (!osKeyEntries) { printf("2 !osKeyEntries\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

    int* pUniversalToOSLookUp = malloc(sizeof(int) * universalKeyEntries); 
    for (int i = 0; i < universalKeyEntries; i++)
    {
        pUniversalToOSLookUp[i] = NO_VALUE;
    }
    
    int* pOSToUniversalLookUp = malloc(sizeof(int) * osKeyEntries); 
    for (int i = 0; i < osKeyEntries; i++)
    {
        pOSToUniversalLookUp[i] = NO_VALUE;
    }
    
    UniversalKeyStatus* pStatusTable = malloc(sizeof(UniversalKeyStatus) * universalKeyEntries);

    cJSON* pKeys = cJSON_GetObjectItem(pJSON, "keys");
    if (!pKeys) { 
        printf("[ ] !pKeys\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    cJSON* pKey = NULL; 
    cJSON* pOSKeyCode = NULL;
    cJSON* pUniversalKeyCode = NULL;
    cJSON* pKeyCode = NULL;

    cJSON_ArrayForEach(pKey, pKeys)
    {
        // printf("New key:\n");
        pKeyCode = cJSON_GetObjectItem(pKey, "keyCode");
        if (!pKeyCode) {  
            printf("[ ] !pOSKeyCode\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(pKeyCode));

        pUniversalKeyCode = cJSON_GetObjectItem(pKeyCode, "universal");
        if (!pUniversalKeyCode) { 
            printf("[ ] !pUniversalKeyCode\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(pUniversalKeyCode));

        pOSKeyCode = cJSON_GetObjectItem(pKeyCode, stringOS);
        if (!pOSKeyCode) { 
            printf("[ ] !pOSKeyCode\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(pOSKeyCode));

        verifyObj = cJSON_GetObjectItemCaseSensitive(pUniversalKeyCode, "dec");
        if (!verifyObj) { 
            printf("[ ] !dec universal 1\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        int universalCode = cJSON_GetNumberValue(verifyObj);
        //if (!universalCode) { // printf("[ ] !dec universal 2\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(pOSKeyCode, "dec");
        if (!verifyObj) { 
            printf("[ ] !dec os 1\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(verifyObj));
        int osCode = cJSON_GetNumberValue(verifyObj);
        //if (!osCode) { // printf("[ ] !dec os 2\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

        pUniversalToOSLookUp[universalCode] = osCode; // *(pUniversalToOSLookUp + universalCode) = osCode (since i work with pointers of size int, adding x to it will "move" the pointer forward to xth location)
        pOSToUniversalLookUp[osCode] = universalCode; // -1 sparat om mapping inte universell kod finns (än)
        // printf("DEBUG pOSToUniversalLookUp[osCode] = universalCode\n");
        // printf("DEBUG                      ^%d       ^%d\nDEBUG\n", osCode, pOSToUniversalLookUp[osCode]);
        pStatusTable[universalCode] = (UniversalKeyStatus) {
            universalCode, 
            NO_VALUE, // flag mask
            NO_VALUE, // time stamp on key down
            false // is pressed
        };
    }

    cJSON_free(pJSON);
    cJSON_free(pKeys);
    cJSON_free(pKey);
    cJSON_free(pOSKeyCode);
    cJSON_free(pUniversalKeyCode);
    cJSON_free(verifyObj);
    if (!pLookUpTables)
    {
        printf("look up table null\n");
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }
    // printf("universalKeyEntries: %d\n", universalKeyEntries);
    // printf("osKeyEntries: %d\n", osKeyEntries);
    /**pLookUpTables = (lookUpTables) {
        pUniversalToOSLookUp,
        pOSToUniversalLookUp,
        pStatusTable,
        universalKeyEntries,
        osKeyEntries
    };*/
    *pLookUpTables = (LookUpTables) {
        pUniversalToOSLookUp,
        pOSToUniversalLookUp,
        universalKeyEntries,
        osKeyEntries, // osKeyEntries
        pStatusTable,
        NULL
    };
    // printf("DEBUG storeData.c createLookUpTable end\n"); return EXIT_CODE_DEBUG;
    return 0;
}

int createEventQueue(EventQueue* pEventQueue)
{
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        pEventQueue->buffer[i] = NULL;
    }
    pEventQueue->head = 0;
    pEventQueue->tail = 0;
    pEventQueue->isFull = false;
    return 0;
}

int createLayerEntries(Layers* pLayerEntries, int universalKeyEntries)
{
    cJSON* pJSON = readJSON("./config/remaps.json");
    if (!pJSON) 
    { 
        printf("[ ] !pJSON remaps\n"); 
        return -1; 
    }

    cJSON* verifyObj = cJSON_GetObjectItemCaseSensitive(pJSON, "layerEntries");
    if (!verifyObj)
    { 
        printf("[ ] !layerEntries 1\n"); 
        return -1; 
    }
    int layerEntries = cJSON_GetNumberValue(verifyObj);
    // if (!layerEntries) { // printf("[ ] !layerEntries 2\n"); return -1; }
    
    for (int i = 0; i < layerEntries; i++)
    {
        pLayerEntries[i] = (Layers) {
            NULL,       // layer name
            NO_VALUE,   // layer nr
            NULL        // remapTable
        };
    }
    

    cJSON* pLayers = cJSON_GetObjectItem(pJSON, "layers");
    if (!pLayers) 
    { 
        printf("[ ] !pLayers\n"); 
        return -1; 
    }
    cJSON* pOSKeyCode = NULL;
    //cJSON* pOnHold = NULL;
    cJSON* pLayer = NULL;   
    cJSON_ArrayForEach(pLayer, pLayers)
    {
        verifyObj = cJSON_GetObjectItemCaseSensitive(pLayer, "layerName");
        if (!verifyObj) 
        { 
            printf("1 !layerName\n"); 
            return -1; 
        }
        char* layerName = cJSON_GetStringValue(verifyObj);
        if (!layerName) 
        { 
            printf("2 !layerName\n"); 
            return -1; 
        }

        verifyObj = cJSON_GetObjectItemCaseSensitive(pLayer, "layerNr");
        if (!verifyObj) 
        { 
            printf("1 !layerNr\n"); 
            return -1; 
        }
        int layerNr = cJSON_GetNumberValue(verifyObj);
        // if (!layerNr) { // printf("2 !layerNr\n"); return -1; }
        UniversalKeyData* pRemapTable = malloc(sizeof(UniversalKeyData) * universalKeyEntries);
        createRemapTable(pRemapTable, pLayer, universalKeyEntries);
        if (!pRemapTable) 
        {  
            printf("[ ] !pRemapTable\n"); 
            return -1; 
        }
        pLayerEntries[layerNr].layerName = strdup(layerName); // deep copy of string
        pLayerEntries[layerNr].layerNr = layerNr;
        pLayerEntries[layerNr].pRemapTable = pRemapTable;
    }

    cJSON_free(pJSON);
    cJSON_free(pLayer);
    cJSON_free(pLayers);
    cJSON_free(pOSKeyCode);
    cJSON_free(verifyObj);

    return 0;
}

int createRemapTable(UniversalKeyData* pRemapTable, cJSON* pLayer, int universalKeyEntries)
{
    // printf("universal entries: %d\n", universalKeyEntries);
    for (int i = 0; i < universalKeyEntries; i++)
    {
        // compound literal initializer, C99 standard or later 
        // (compiler version can be manually set by flag -std=C[version nr])
        pRemapTable[i] = (UniversalKeyData) { 
            NO_VALUE, // code
            NO_VALUE, // codeOnPress
            NO_VALUE, // codeOnHold
            false // keyDown
        };
    }
    // printf("\n");
    cJSON* pOSKeyCode = cJSON_GetObjectItem(pLayer, "keyCode");
    cJSON* pKeyCode = NULL;
    cJSON_ArrayForEach(pKeyCode, pOSKeyCode)
    {
        cJSON* verifyObj = cJSON_GetObjectItemCaseSensitive(pKeyCode, "from");
        if (!verifyObj) 
        { 
            printf("[ ] !from 1\n"); 
            return 1; 
        }
        int from = cJSON_GetNumberValue(verifyObj);
        // if (!from) { // printf("[ ] !from 2\n"); return -1; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(pKeyCode, "toOnPress");
        if (!verifyObj) { 
            printf("[ ] !toOnPress 1\n"); 
            return 1; 
        }
        int toOnPress = cJSON_GetNumberValue(verifyObj);
        // if (!toOnPress) { // printf("[ ] !toOnPress 2\n"); return -1; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(pKeyCode, "toOnHold");
        if (!verifyObj) 
        { 
            printf("[ ] !toOnHold 1\n"); 
            return 1; 
        }
        int toOnHold = cJSON_GetNumberValue(verifyObj);
        // if (!toOnHold) { // printf("[ ] !toOnHold 2\n"); return -1; }

        pRemapTable[from] = (UniversalKeyData) {
            from,
            toOnPress,
            toOnHold,
            false
        };
        PRINT_REMAP_TABLE_ENTRY(pRemapTable[from]);
    }
    return 0;
}

cJSON* readJSON(char* path) 
{
    FILE* pF = fopen(path, "r");

    if (!pF) 
    {
        // printf("Could not open json file! => ");
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
            // printf("Error: %s => ", error_ptr);
        }
        cJSON_Delete(pJSON);
        return NULL;
    }
    return pJSON;
}