#ifndef _STOREDATA_
#define _STOREDATA_

#include <stdio.h>
#include "data.h"
#include "./../lib/cJSON.h"

void initEventQueue(EventQueue* queue);

void initCodeConverters(cJSON* json, OS os);

void initRemapTable(cJSON* json);

cJSON* readJSON(char* path);

#endif // _STOREDATA_