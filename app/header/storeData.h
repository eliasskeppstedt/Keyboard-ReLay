#ifndef _STOREDATA_
#define _STOREDATA_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./../lib/cJSON.h"
#include "data.h"

int createLookUpTables(lookUpTables*, char*);
layers* createLayerEntries(int, char*);
keyData* createRemapTable(cJSON*, int);
cJSON* readJSON(char*);

#endif // _STOREDATA_