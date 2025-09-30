#include "./../header/storeData.h"
// for reading json, im using the relative path, change later? then ig change to absolute path
// based on download location for each OS

int createLookUpTables(LookUpTables* lookUpTables, OS os)
{
    // printf("DEBUG storeData.c createLookUpTable\n");
    // printf("DEBUG cjson array for each to create look up table\n");
    // Create look up tables
    // works only if first universal code is 0 and increses by 1 untill universalKeyEntries - 1
    cJSON* json = readJSON("./config/keys.json");
    if (!json) 
    { 
        printf("[ ] !json keys\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }

    cJSON* verifyObj = cJSON_GetObjectItem(json, "universalEntries");
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

    verifyObj = cJSON_GetObjectItem(json, stringOSEntries);
    if (!verifyObj) 
    { 
        printf("1 !osKeyEntries\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    int osKeyEntries = cJSON_GetNumberValue(verifyObj);
    // if (!osKeyEntries) { printf("2 !osKeyEntries\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

    int* universalToOSLookUp = malloc(sizeof(int) * universalKeyEntries); 
    for (int i = 0; i < universalKeyEntries; i++)
    {
        universalToOSLookUp[i] = NO_VALUE;
    }
    
    int* osToUniversalLookUp = malloc(sizeof(int) * osKeyEntries); 
    for (int i = 0; i < osKeyEntries; i++)
    {
        osToUniversalLookUp[i] = NO_VALUE;
    }
    
    UniversalKeyStatus* statusTable = malloc(sizeof(UniversalKeyStatus) * universalKeyEntries);

    cJSON* keys = cJSON_GetObjectItem(json, "keys");
    if (!keys) { 
        printf("[ ] !keys\n"); 
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
    }
    cJSON* key = NULL; 
    cJSON* osKeyCodes = NULL;
    cJSON* universalKeyCode = NULL;
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

        universalKeyCode = cJSON_GetObjectItem(osKeyCode, "universal");
        if (!universalKeyCode) { 
            printf("[ ] !universalKeyCode\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(universalKeyCode));

        osKeyCodes = cJSON_GetObjectItem(osKeyCode, stringOS);
        if (!osKeyCodes) { 
            printf("[ ] !osKeyCodes\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(osKeyCodes));

        verifyObj = cJSON_GetObjectItemCaseSensitive(universalKeyCode, "dec");
        if (!verifyObj) { 
            printf("[ ] !dec universal 1\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        int universalCode = cJSON_GetNumberValue(verifyObj);
        //if (!universalCode) { // printf("[ ] !dec universal 2\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

        verifyObj = cJSON_GetObjectItemCaseSensitive(osKeyCodes, "dec");
        if (!verifyObj) { 
            printf("[ ] !dec os 1\n"); 
            return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED; 
        }
        // printf("%s\n", cJSON_Print(verifyObj));
        int osCode = cJSON_GetNumberValue(verifyObj);
        //if (!osCode) { // printf("[ ] !dec os 2\n"); return EXIT_CODE_LOOK_UP_TABLE_FAILED; }

        universalToOSLookUp[universalCode] = osCode; // *(universalToOSLookUp + universalCode) = osCode (since i work with pointers of size int, adding x to it will "move" the pointer forward to xth location)
        osToUniversalLookUp[osCode] = universalCode; // -1 sparat om mapping inte universell kod finns (än)
        // printf("DEBUG osToUniversalLookUp[osCode] = universalCode\n");
        // printf("DEBUG                      ^%d       ^%d\nDEBUG\n", osCode, osToUniversalLookUp[osCode]);
        statusTable[universalCode] = (UniversalKeyStatus) {
            universalCode, 
            NO_VALUE, // flag mask
            NO_VALUE, // time stamp on key down
            false, // keyDown
            NORMAL // state
        };
    }

    cJSON_free(json);
    cJSON_free(keys);
    cJSON_free(key);
    cJSON_free(osKeyCodes);
    cJSON_free(universalKeyCode);
    cJSON_free(verifyObj);
    if (!lookUpTables)
    {
        printf("look up table null\n");
        return EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED;
    }
    // printf("universalKeyEntries: %d\n", universalKeyEntries);
    // printf("osKeyEntries: %d\n", osKeyEntries);
    /**lookUpTables = (lookUpTables) {
        universalToOSLookUp,
        osToUniversalLookUp,
        statusTable,
        universalKeyEntries,
        osKeyEntries
    };*/
    *lookUpTables = (LookUpTables) {
        universalToOSLookUp,
        osToUniversalLookUp,
        universalKeyEntries,
        osKeyEntries, // osKeyEntries
        statusTable,
        NULL
    };
    // printf("DEBUG storeData.c createLookUpTable end\n"); return EXIT_CODE_DEBUG;
    return 0;
}

int createEventQueue(EventQueue* eventQueue)
{
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        eventQueue->buffer[i] = NULL;
        eventQueue->bufferReadyForDispatch[i] = NULL;
    }
    eventQueue->head = 0;
    eventQueue->tail = 0;
    eventQueue->isFull = false;
    return 0;
}

int createLayerEntries(Layer* layerList, int universalKeyEntries)
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
        UniversalKeyData* remapTable = malloc(sizeof(UniversalKeyData) * universalKeyEntries);
        createRemapTable(remapTable, layer, universalKeyEntries);
        if (!remapTable) 
        {  
            printf("[ ] !remapTable\n"); 
            return -1; 
        }
        layerList[layerNr].layerName = strdup(layerName); // deep copy of string
        layerList[layerNr].layerNr = layerNr;
        layerList[layerNr].remapTable = remapTable;
    }

    cJSON_free(json);
    cJSON_free(layer);
    cJSON_free(layers);
    cJSON_free(osKeyCodes);
    cJSON_free(verifyObj);

    return 0;
}

int createRemapTable(UniversalKeyData* remapTable, cJSON* layer, int universalKeyEntries)
{
    // printf("universal entries: %d\n", universalKeyEntries);
    for (int i = 0; i < universalKeyEntries; i++)
    {
        // compound literal initializer, C99 standard or later 
        // (compiler version can be manually set by flag -std=C[version nr])
        remapTable[i] = (UniversalKeyData) { 
            NO_VALUE, // code
            NO_VALUE, // codeOnPress
            NO_VALUE, // codeOnHold
            false, // keyDown
            NORMAL // state
        };
    }
    // printf("\n");
    cJSON* osKeyCodes = cJSON_GetObjectItem(layer, "keyCode");
    if (!osKeyCodes) 
    { 
        printf("1 !osKeyCodes\n"); 
        return -1; 
    }
    for (int i = 0; i < universalKeyEntries; i++)
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

        remapTable[from] = (UniversalKeyData) {
            from,
            toOnPress,
            toOnHold,
            false
        };
        PRINT_REMAP_TABLE_ENTRY(remapTable[from]);
    }
    for (int i = 0; i < universalKeyEntries; i++)
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