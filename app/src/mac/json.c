#include "./../../lib/cJSON.h"
#include "./../../lib/cJSON.c"

cJSON* createRemapTable() 
{
    FILE *pF = fopen("./src/mapping.json", "r");

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

cJSON* createRemapTableForMac(cJSON* json) 
{
    return json;
}

cJSON* getRemapOnPress(cJSON* remapTable)
{
    cJSON* remaps = cJSON_GetObjectItem(remapTable, "remapsOnPress");
    return remapTable;
}

cJSON* getRemapOnHold(cJSON* remapTable)
{
    cJSON* remaps = cJSON_GetObjectItem(remapTable, "remapsOnHold");
    return remapTable;
}

cJSON* getLayer(cJSON* remapTable)
{
    return remapTable;
}

void deleteRemapTable(cJSON* json) 
{
    cJSON_Delete(json);
}