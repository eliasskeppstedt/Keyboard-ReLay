#ifndef _STOREDATA_
#define _STOREDATA_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./../lib/cJSON.h"
#include "data.h"

#include "./../header/storeData.h"

/**
 * @return 
 */
int createLookUpTables(LookUpTables* lookUpTables, OS os);

int createEventQueue(EventQueue* eventQueue);

/**
 * @return 
 */
int createLayerEntries(Layer* layerList, int universalEntries);


/**
 * Create a remap table. Should be 1 for each layer
 */
int createRemapTable(UniversalKeyData* remapTable, cJSON* layer, int universalKeyEntries);


/**
 * @internal
 */
cJSON* readJSON(char* path);

#endif // _STOREDATA_