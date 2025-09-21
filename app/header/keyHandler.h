#ifndef _KEYHANDLER_
#define _KEYHANDLER_

#include <stdio.h>
#include "data.h"
#include "converter.h"

typedef struct generalizedEvent {
    int keyCode;
    int eventFlagMask;
    bool isPressed;
} generalizedEvent;

int handleEvent(layers*, generalizedEvent*, int*);
/**
 * @param generalizedEvent* pMacEvent
 * @param int* pLookUpTables
 */
int handleMacEvent(layers*, generalizedEvent*, lookUpTables*);

#endif // _KEYHANDLER_