#ifndef _KEYHANDLER_
#define _KEYHANDLER_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "data.h"
#include "converter.h"
#include "eventQueue.h"


/*
Layer* layerList, 
GeneralizedEvent* eventvent, 
LookUpTables* lookUpTables
*/
int handleEvent(
    Layer* layerList, 
    GeneralizedEvent* event, 
    LookUpTables* lookUpTables
);

/*
Layer* layerList, 
GeneralizedEvent* macEvent, 
LookUpTables* lookUpTables
*/
int handleMacEvent(
    Layer* layerList, 
    GeneralizedEvent* macEvent, 
    LookUpTables* lookUpTables
);


uint64_t getTimeStamp();

#endif // _KEYHANDLER_