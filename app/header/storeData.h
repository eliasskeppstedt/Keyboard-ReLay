#ifndef _STOREDATA_
#define _STOREDATA_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./../lib/cJSON.h"
#include "data.h"

typedef struct lookUpTables {
    int* pWebToOS;
    int* pOSToWeb;
} lookUpTables;

int createLookUpTables(lookUpTables*, char*);
layers* createLayerEntries(char*, int);
keyData* createRemapTable(int);
cJSON* readJSON(char*);

#endif // _STOREDATA_