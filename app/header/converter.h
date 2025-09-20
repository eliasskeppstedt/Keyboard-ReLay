#ifndef _KEYCODECONVERTER_
#define _KEYCODECONVERTER_

#include <stdio.h>
#include <string.h>

typedef struct keyEntry {
    char* name;
    int webCode;
    int osCode;
} keyEntry;

int webStringToCode(char*);
int macToWeb(int);

#endif // _KEYCODECONVERTER_