#include "./../header/dataStorage.h"

cJSON* createRemapTable() 
{
    FILE *pF = fopen("./../mapping.json", "r");

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
    for (int i = 0; i < AMOUNT_OF_KEYCODES; i++)
    {
        pRemapTable[i].isPressed = 0;
        pRemapTable[i].macRemapOnHold = NO_KEY;
        pRemapTable[i].macRemapOnPress = NO_KEY;
    }
    
    cJSON* layer = NULL;
    cJSON* layers = cJSON_GetObjectItem(remapTableObject, "layer1");
    if (layers)
    {
        cJSON_ArrayForEach(layer, layers)
        {
            cJSON* remapsOnPress = cJSON_GetObjectItem(remapTableObject, "remapsOnPress");
            

            cJSON* remapsOnHold = cJSON_GetObjectItem(remapTableObject, "remapsOnHold");
        }
    }
}

struct keyData* createRemap(int os)
{
    char* mapping;
    cJSON* remapTableObject = createRemapTable();
    struct keyData* pRemapTable = malloc(sizeof(struct keyData) * AMOUNT_OF_KEYCODES);
    if (os == 1)
    {
        mapping = "macMapping";
    }
    
    if (remapTableObject)
    {
        remapTableObject = cJSON_GetObjectItem(remapTableObject, mapping);
        if (remapTableObject)
        {
            populateRemapTable(remapTableObject, pRemapTable);
        }
        else
        {
            printf("Could hnot find objecs '%s'\n", mapping);
        }
    } 
    else
    {
        printf("Could not create remap table\n");
    }
}