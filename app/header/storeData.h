#ifndef _STOREDATA_
#define _STOREDATA_

#include <stdio.h>
#include "data.h"
#include "./../lib/cJSON.h"

void initEventQueue(EventQueue* queue);

void initCodeConverters(cJSON* json, int* osToRL, int* rlToOS, int entries, OS os);

void initRemapTable(cJSON* json, KeyInfo*** pRemapTable, int layerEntries, int entries);

cJSON* readJSON(char* path);

#endif // _STOREDATA_