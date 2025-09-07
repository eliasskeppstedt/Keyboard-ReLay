#include "./../header/dataStorage.h"

cJSON* createRemapTable() 
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

void deleteRemapTable(cJSON* json) 
{
    cJSON_Delete(json);
}

void populateRemapTable(cJSON* remapTableObject, struct keyData* pRemapTable)
{

}

struct keyData* createRemap(int os)
{
    cJSON* remapTableObject = createRemapTable();   
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
    // initialize remap table //
    printf("Initializing remap table...\n");
    for (int i = 0; i < AMOUNT_OF_KEYCODES; i++)
    { // hmmm det enda här som behöver göras är väll att allokera plats för remapOnPress/Hold på heapen,
        // kolla igenom igen
        // one remap for each layer (for now)
        pRemapTable[i].pMacKeyCodeRemapOnPress = malloc(sizeof(int) * layerCount);
        pRemapTable[i].pMacKeyCodeRemapOnHold = malloc(sizeof(int) * layerCount);
        //pRemapTable[i].layerNr = ;
        char layerNameBuf[7]; // l a y e r X \n
        snprintf(layerNameBuf, layerCount, "layer%i\n", i); // TODO make user define layout name
        pRemapTable[i].layerName = layerNameBuf;
        for (int j = 0; j < layerCount; j++)
        {
            pRemapTable[i].pMacKeyCodeRemapOnPress[j] =  NO_KEY;
            pRemapTable[i].pMacKeyCodeRemapOnHold[j] = NO_KEY;
        }
        pRemapTable[i].isPressed = 0;
    }
    // populate remap table //
    printf("Populating remap table...\n");
    if (layers) { printf("layers: %s\n", cJSON_Print(layers)); }
    cJSON_ArrayForEach(layer, layers) // hmm why sgementation fault here!?
    {
        printf("1 ");
        int layerNr = cJSON_GetObjectItemCaseSensitive(remapTableObject, "layer")->valueint;
        printf("2 ");
        char* layerName = cJSON_GetObjectItemCaseSensitive(remapTableObject, "name")->string;
        printf("3 ");
        cJSON* macMapping = cJSON_GetObjectItem(remapTableObject, "macMapping");
        printf("4 ");
        cJSON* remapsOnPress = cJSON_GetObjectItem(macMapping, "remapsOnPress");
        printf("5\n");
        cJSON* key = NULL;
        cJSON_ArrayForEach(key, remapsOnPress)
        {
            int keyCodeIndex = cJSON_GetObjectItemCaseSensitive(remapsOnPress, "from")->valueint;
            int keyCodeValue = cJSON_GetObjectItemCaseSensitive(remapsOnPress, "to")->valueint;
            printf("onPress: from: %i, to: %i\n", keyCodeIndex, keyCodeValue);
            pRemapTable[keyCodeIndex].pMacKeyCodeRemapOnPress[layerNr] = keyCodeValue;
        }
        cJSON* remapsOnHold = cJSON_GetObjectItem(macMapping, "remapsOnHold");
        cJSON_ArrayForEach(key, remapsOnHold)
        {
            int keyCodeIndex = cJSON_GetObjectItemCaseSensitive(remapsOnPress, "from")->valueint;
            int keyCodeValue = cJSON_GetObjectItemCaseSensitive(remapsOnPress, "to")->valueint;
            printf("onPress: from: %i, to: %i\n", keyCodeIndex, keyCodeValue);
            pRemapTable[keyCodeIndex].pMacKeyCodeRemapOnPress[layerNr] = keyCodeValue;
        }
    } 
    return pRemapTable;
}