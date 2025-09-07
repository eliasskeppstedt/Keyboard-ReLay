#ifndef _DATASTORAGE_
#define _DATASTORAGE_

#include <stdio.h>
#include <stdlib.h>
#include "./../lib/cJSON.h"
#include "constants.h"

typedef struct keyData {
    int layerNr;
    char* layerName;
    int* pMacKeyCodeRemapOnPress;
    int* pMacKeyCodeRemapOnHold;
    int isPressed;
} keyData;

#define AMOUNT_OF_KEYCODES 1024

cJSON* createRemapTable();

void deleteRemapTable(cJSON*);

void populateRemapTable(cJSON*, struct keyData*);

struct keyData* createRemap(int);

#endif // _DATASTORAGE_