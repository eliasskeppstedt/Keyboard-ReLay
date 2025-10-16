#ifndef _DATASTORAGE_
#define _DATASTORAGE_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./../lib/cJSON.h"
#include "constants.h"

#define AMOUNT_OF_KEYCODES 1024

cJSON* readJSON();
void populateRemapTable(cJSON*, struct keyData*);
void updateForKeyInLayerRemap(
    enum REMAP_MODES, 
    cJSON*, 
    struct keyData*, 
    int, 
    char*
);
void printKey(struct keyData*, int, int);
struct keyData* createRemapTable(enum USER_OS);

#endif // _DATASTORAGE_