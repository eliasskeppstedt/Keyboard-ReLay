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
    }; 
    struct keyData* pRemapTable = malloc(sizeof(struct keyData) * AMOUNT_OF_KEYCODES);
    char* mapping = "layers";
    cJSON* layers = cJSON_GetObjectItem(remapTableObject, mapping);
    cJSON_Delete(remapTableObject);
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
    pRemapTable->pMacKeyCodeRemapOnPress = malloc(sizeof(int) * layerCount);
    pRemapTable->pMacKeyCodeRemapOnHold = malloc(sizeof(int) * layerCount);
    // initialize remap table //
    for (int i = 0; i < AMOUNT_OF_KEYCODES; i++)
    {
        pRemapTable[i].layerNr = i;
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
    cJSON_ArrayForEach(layer, layers)
    {
        cJSON* layerNr = cJSON_GetObjectItemCaseSensitive(remapTableObject, "layer");
        cJSON* layerName = cJSON_GetObjectItemCaseSensitive(remapTableObject, "name");
        cJSON* macMapping = cJSON_GetObjectItem(remapTableObject, "macMapping");
        cJSON* remapsOnPress = cJSON_GetObjectItem(remapTableObject, "remapsOnPress");
        cJSON* key = NULL; 
        cJSON_ArrayForEach(key, remapsOnPress)
        {
            printf("%i\n", key->valueint);
        }
        cJSON* remapsOnHold = cJSON_GetObjectItem(remapTableObject, "remapsOnHold");
    }
    return pRemapTable;
}