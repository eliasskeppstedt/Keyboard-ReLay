#ifndef _KEYHANDLER_
#define _KEYHANDLER_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "data.h"
#include "converter.h"
#include "eventQueue.h"

int handleEvent(
    Layers* pLayerEntries, 
    GeneralizedEvent* pUniversalEvent, 
    LookUpTables* lookUpTables
);

int handleMacEvent(
    Layers* pLayerEntries, 
    GeneralizedEvent* pMacEvent, 
    LookUpTables* pLookUpTables
);

#endif // _KEYHANDLER_