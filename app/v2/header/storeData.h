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
int createLayerEntries(Layer* layerList, int krEntries);


/**
 * Create a remap table. Should be 1 for each layer
 */
int createRemapTable(KRKeyData* remapTable, cJSON* layer, int krKeyEntries);


/**
 * @internal
 */
cJSON* readJSON(char* path);

#endif // _STOREDATA_