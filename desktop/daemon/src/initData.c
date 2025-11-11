#include "./../header/initData.h"

static cJSON* MY_REMAPS_OBJ = NULL;
static cJSON* readJSON(char* path);

void initEventQueue()
{
    createEventQueue();
    printf("> event queue initialized!\n");
}

void initMyRemaps()
{
    cJSON* layerEntriesJSONObj = cJSON_GetObjectItem(MY_REMAPS_OBJ, "layerEntries");
    if (!layerEntriesJSONObj) exit(READ_KEY_CODE_ENTRIES);
    int layerEntries = cJSON_GetNumberValue(layerEntriesJSONObj);

    createRemapsArray(layerEntries);

    cJSON* layersJSONObj= cJSON_GetObjectItem(MY_REMAPS_OBJ, "layers");
    if (!layersJSONObj) exit(READ_LAYERS);


    cJSON* layerJSONObj = NULL;
    cJSON* layerNrJSONObj = NULL;
    cJSON* keyCodesJSONObj = NULL;
    cJSON* codeJSONObj = NULL;
    cJSON* fromJSONObj = NULL;
    cJSON* toOnPressJSONObj = NULL;
    cJSON* toOnHoldJSONObj = NULL;
    cJSON* keyCodeEntriesJSONObj = NULL;

    cJSON_ArrayForEach(layerJSONObj, layersJSONObj)
    {
        layerNrJSONObj= cJSON_GetObjectItem(layerJSONObj, "layerNr");
        if (!layerNrJSONObj) exit(READ_LAYER_NR);
        int layerNr = cJSON_GetNumberValue(layerNrJSONObj); 

        keyCodeEntriesJSONObj = cJSON_GetObjectItem(layerJSONObj, "keyCodeEntries");
        if (!keyCodeEntriesJSONObj) exit(READ_LAYER_NR);
        int keyCodeEntries = cJSON_GetNumberValue(keyCodeEntriesJSONObj);

        createRemapTableForLayer(layerNr, keyCodeEntries);

        keyCodesJSONObj= cJSON_GetObjectItem(layerJSONObj, "keyCode");
        if (!keyCodesJSONObj) exit(READ_KEYCODE);

        cJSON_ArrayForEach(codeJSONObj, keyCodesJSONObj)
        {
            fromJSONObj= cJSON_GetObjectItem(codeJSONObj, "from");
            if (!fromJSONObj) exit(READ_FROM);
            toOnPressJSONObj= cJSON_GetObjectItem(codeJSONObj, "toOnPress");
            if (!toOnPressJSONObj) exit(READ_TO_ON_PRESS);
            toOnHoldJSONObj= cJSON_GetObjectItem(codeJSONObj, "toOnHold");
            if (!toOnHoldJSONObj) exit(READ_TO_ON_HOLD);

            int from = cJSON_GetNumberValue(fromJSONObj);
            int onPress = cJSON_GetNumberValue(toOnPressJSONObj);
            int onHold = cJSON_GetNumberValue(toOnHoldJSONObj);
            
            addRemapTableEntry(layerNr, from, onPress, onHold);
        }
    }
    printf("> my remaps initialized!\n");
}

void initStatusTable()
{

    cJSON* layersJSONObj= cJSON_GetObjectItem(MY_REMAPS_OBJ, "layers");
    if (!layersJSONObj) exit(READ_LAYERS);

    cJSON* uniqueKeyCodeEntriesJSONObj = cJSON_GetObjectItem(MY_REMAPS_OBJ, "uniqueKeyCodeEntries");
    if (!uniqueKeyCodeEntriesJSONObj) exit(READ_KEY_CODE_ENTRIES);
    int uniqueKeyCodeEntries = cJSON_GetNumberValue(uniqueKeyCodeEntriesJSONObj);

    createStatusTable(uniqueKeyCodeEntries);

    cJSON* layerJSONObj = NULL;
    cJSON* keyCodesJSONObj = NULL;
    cJSON* codeJSONObj = NULL;
    cJSON* fromJSONObj = NULL;

    cJSON_ArrayForEach(layerJSONObj, layersJSONObj)
    {

        keyCodesJSONObj= cJSON_GetObjectItem(layerJSONObj, "keyCode");
        if (!keyCodesJSONObj) exit(READ_KEYCODE);

        cJSON_ArrayForEach(codeJSONObj, keyCodesJSONObj)
        {
            fromJSONObj= cJSON_GetObjectItem(codeJSONObj, "from");
            if (!fromJSONObj) exit(READ_FROM);

            int from = cJSON_GetNumberValue(fromJSONObj);
            addStatusTableEntry(from);
        }
    }
    printf("> status table initialized!\n");
}

void openJSON(char* path) 
{
    MY_REMAPS_OBJ = readJSON(path);
    cJSON* layersJSONObj = cJSON_GetObjectItem(MY_REMAPS_OBJ, "layers");
    cJSON* layerEntriesJSONObj= cJSON_GetObjectItem(MY_REMAPS_OBJ, "layerEntries");

    if (!layerEntriesJSONObj) exit(READ_LAYER_ENTRIES);
    setLayerEntries(cJSON_GetNumberValue(layerEntriesJSONObj));
    printf("> json successfully opened!\n");
}

void deleteJSON()
{
    cJSON_Delete(MY_REMAPS_OBJ);
    MY_REMAPS_OBJ = NULL;
    printf("> json deleted!\n");
}

static cJSON* readJSON(char* path) 
{
    FILE* f = fopen(path, "r");

    if (!f) 
    {
        // printf("Could not open json file! => ");
        return NULL;
    }

    char buffer[32768]; // hm look over and make dynamic based of the file size
    fread(buffer, 1, sizeof(buffer), f);
    fclose(f);

    // parse the JSON data
    cJSON* json = cJSON_Parse(buffer);
    if (!json) 
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) 
        {
            printf("Error: %s => ", error_ptr);
        }
        cJSON_Delete(json);
        return NULL;
    }
    return json;
}