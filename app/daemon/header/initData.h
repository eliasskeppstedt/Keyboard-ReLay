#ifndef _INITDATA_
#define _INITDATA_

#include <stdio.h>
#include "./../lib/cJSON.h"
#include "./constants.h"
#include "./eventQueue.h"

void initEventQueue();
void initMyRemaps();
void initStatusTable();
void openJSON(char* path);
void deleteJSON();

#endif // _INITDATA_