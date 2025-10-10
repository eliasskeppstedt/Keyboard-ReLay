#include "./../header/storeData.h"
// for reading json, im using the relative path, change later? then ig change to absolute path
// based on download location for each OS

int createLookUpTables(LookUpTables* lookUpTables, OS os)
{
    // printf("DEBUG storeData.c createLookUpTable\n");
    // printf("DEBUG cjson array for each to create look up table\n");
    // Create look up tables
    // works only if first kr code is 0 and increses by 1 untill krKeyEntries - 1
    cJSON* json = readJSON("./config/keys.json");
    if (!json) 
    { 
        printf("[ ] !json keys\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }

    cJSON* verifyObj = cJSON_GetObjectItem(json, "krEntries");
    if (!verifyObj) 
    { 
        printf("1 !krKeyEntries\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    int krKeyEntries = cJSON_GetNumberValue(verifyObj);
    // if (!krKeyEntries) { printf("2 !krKeyEntries\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }
    
    char* stringOSEntries;
    char* stringOS;
    if (os == MACOS)
    {
        stringOSEntries = "macEntries";
        stringOS = "mac";
    }

    verifyObj = cJSON_GetObjectItem(json, stringOSEntries);
    if (!verifyObj) 
    { 
        printf("1 !osKeyEntries\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    int osKeyEntries = cJSON_GetNumberValue(verifyObj);
    // if (!osKeyEntries) { printf("2 !osKeyEntries\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

    int* krToOSLookUp = malloc(sizeof(int) * krKeyEntries); 
    KRKeyStatusTable* statusTable = malloc(sizeof(KRKeyStatusTable));
    KRKeyStatus* status = malloc(sizeof(KRKeyStatus) * krKeyEntries);
    *statusTable = (KRKeyStatusTable) {
        status,
        0
    };
    for (int i = 0; i < krKeyEntries; i++)
    {
        krToOSLookUp[i] = NO_VALUE;
        *status = (KRKeyStatus) {
            NO_VALUE,
            false
        };
    }

    int* osToKRLookUp = malloc(sizeof(int) * osKeyEntries); 
    for (int i = 0; i < osKeyEntries; i++)
    {
        osToKRLookUp[i] = NO_VALUE;
    }
    
    //KRKeyStatusTable* statusTable = malloc(sizeof(KRKeyStatus) * krKeyEntries);

    cJSON* keys = cJSON_GetObjectItem(json, "keys");
    if (!keys) { 
        printf("[ ] !keys\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    cJSON* key = NULL; 
    cJSON* osKeyCodes = NULL;
    cJSON* krKeyCode = NULL;
    cJSON* osKeyCode = NULL;
    int test = 0;
    cJSON_ArrayForEach(key, keys)
    {
        printf("%d ", test++);
        // printf("New key:\n");
        osKeyCode = cJSON_GetObjectItem(key, "keyCode");
        if (!osKeyCode) {  
            printf("[ ] !osKeyCodes\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(osKeyCode));

        krKeyCode = cJSON_GetObjectItem(osKeyCode, "kr");
        if (!krKeyCode) { 
            printf("[ ] !krKeyCode\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(krKeyCode));

        osKeyCodes = cJSON_GetObjectItem(osKeyCode, stringOS);
        if (!osKeyCodes) { 
            printf("[ ] !osKeyCodes\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(osKeyCodes));

        verifyObj = cJSON_GetObjectItemCaseSensitive(krKeyCode, "dec");
        if (!verifyObj) { 
            printf("[ ] !dec kr 1\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        int krCode = cJSON_GetNumberValue(verifyObj);
        //if (!krCode) { // printf("[ ] !dec kr 2\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(osKeyCodes, "dec");
        if (!verifyObj) { 
            printf("[ ] !dec os 1\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(verifyObj));
        int osCode = cJSON_GetNumberValue(verifyObj);
        //if (!osCode) { // printf("[ ] !dec os 2\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

        krToOSLookUp[krCode] = osCode; // *(krToOSLookUp + krCode) = osCode (since i work with pointers of size int, adding x to it will "move" the pointer forward to xth location)
        osToKRLookUp[osCode] = krCode; // -1 sparat om mapping inte universell kod finns (än)
        // printf("DEBUG osToKRLookUp[osCode] = krCode\n");
        // printf("DEBUG                      ^%d       ^%d\nDEBUG\n", osCode, osToKRLookUp[osCode]);
        status[krCode] = (KRKeyStatus) {
            krCode, 
            false // keyDown
        };
    }

    cJSON_free(json);
    cJSON_free(keys);
    cJSON_free(key);
    cJSON_free(osKeyCodes);
    cJSON_free(krKeyCode);
    cJSON_free(verifyObj);
    if (!lookUpTables)
    {
        printf("look up table null\n");
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }
    *lookUpTables = (LookUpTables) {
        krToOSLookUp,
        osToKRLookUp,
        krKeyEntries,
        osKeyEntries, // osKeyEntries
        statusTable, //statusTable,
        NULL
    };
    return 0;
}

int createEventQueue(EventQueue* eventQueue)
{
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        eventQueue->buffer[i] = NULL;
    }
    eventQueue->head = 0;
    eventQueue->tail = 0;
    eventQueue->isFull = false;
    return 0;
}

int createLayerEntries(Layer* layerList, int krKeyEntries)
{
    cJSON* json = readJSON("./config/remaps.json");
    if (!json) 
    { 
        printf("[ ] !json remaps\n"); 
        return -1; 
    }

    cJSON* verifyObj = cJSON_GetObjectItemCaseSensitive(json, "layerEntries");
    if (!verifyObj)
    { 
        printf("[ ] !layerEntries 1\n"); 
        return -1; 
    }
    int layerEntries = cJSON_GetNumberValue(verifyObj);
    // if (!layerEntries) { // printf("[ ] !layerEntries 2\n"); return -1; }
    
    for (int i = 0; i < layerEntries; i++)
    {
        layerList[i] = (Layer) {
            NULL,       // layer name
            NO_VALUE,   // layer nr
            NULL        // remapTable
        };
    }
    

    cJSON* layers = cJSON_GetObjectItem(json, "layers");
    if (!layers) 
    { 
        printf("[ ] !layers\n"); 
        return -1; 
    }
    cJSON* osKeyCodes = NULL;
    cJSON* layer = NULL;   
    cJSON_ArrayForEach(layer, layers)
    {
        verifyObj = cJSON_GetObjectItemCaseSensitive(layer, "layerName");
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

        verifyObj = cJSON_GetObjectItemCaseSensitive(layer, "layerNr");
        if (!verifyObj) 
        { 
            printf("1 !layerNr\n"); 
            return -1; 
        }
        int layerNr = cJSON_GetNumberValue(verifyObj);
        // if (!layerNr) { // printf("2 !layerNr\n"); return -1; }
        KRKeyData* remapTable = malloc(sizeof(KRKeyData) * krKeyEntries);
        createRemapTable(remapTable, layer, krKeyEntries);
        if (!remapTable) 
        {  
            printf("[ ] !remapTable\n"); 
            return -1; 
        }
        layerList[layerNr].name = strdup(layerName); // deep copy of string
        layerList[layerNr].nr = layerNr;
        layerList[layerNr].remapTable = remapTable;
    }

    cJSON_free(json);
    cJSON_free(layer);
    cJSON_free(layers);
    cJSON_free(osKeyCodes);
    cJSON_free(verifyObj);

    return 0;
}

int createRemapTable(KRKeyData* remapTable, cJSON* layer, int krKeyEntries)
{
    // printf("kr entries: %d\n", krKeyEntries);
    for (int i = 0; i < krKeyEntries; i++)
    {
        // compound literal initializer, C99 standard or later 
        // (compiler version can be manually set by flag -std=C[version nr])
        remapTable[i] = (KRKeyData) { 
            NO_VALUE, // code
            NO_VALUE, // codeOnPress
            NO_VALUE, // codeOnHold
            false, // keyDown
        };
    }
    // printf("\n");
    cJSON* osKeyCodes = cJSON_GetObjectItem(layer, "keyCode");
    if (!osKeyCodes) 
    { 
        printf("1 !osKeyCodes\n"); 
        return -1; 
    }
    for (int i = 0; i < krKeyEntries; i++)
    {
        printf("%d", remapTable[i].code);
    } printf("\n");
    
    
    cJSON* osKeyCode = NULL;
    cJSON_ArrayForEach(osKeyCode, osKeyCodes)
    {
        cJSON* verifyObj = cJSON_GetObjectItemCaseSensitive(osKeyCode, "from");
        if (!verifyObj) 
        { 
            printf("[ ] !from 1\n"); 
            return 1; 
        }
        int from = cJSON_GetNumberValue(verifyObj);
        // if (!from) { // printf("[ ] !from 2\n"); return -1; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(osKeyCode, "toOnPress");
        if (!verifyObj) { 
            printf("[ ] !toOnPress 1\n"); 
            return 1; 
        }
        int toOnPress = cJSON_GetNumberValue(verifyObj);
        // if (!toOnPress) { // printf("[ ] !toOnPress 2\n"); return -1; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(osKeyCode, "toOnHold");
        if (!verifyObj) 
        { 
            printf("[ ] !toOnHold 1\n"); 
            return 1; 
        }
        int toOnHold = cJSON_GetNumberValue(verifyObj);
        // if (!toOnHold) { // printf("[ ] !toOnHold 2\n"); return -1; }

        remapTable[from] = (KRKeyData) {
            from,
            toOnPress,
            toOnHold,
            false
        };
        PRINT_REMAP_TABLE_ENTRY(remapTable[from]);
    }
    for (int i = 0; i < krKeyEntries; i++)
    {
        printf("%s%d", remapTable[i].code == NO_VALUE ? "" : " ", remapTable[i].code);
    } printf("\n");
    return 0;
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