#ifndef _INITDATA_
#define _INITDATA_

#include <stdio.h>
#include "data.h"
#include "../lib/cJSON.h"
#include "os/quartzEventService.h"
#include "eventQueue.h"

void initEventQueue();
void initMyRemaps();
void initStatusTable();
void openJSON(char* path);
void deleteJSON();

#endif // _INITDATA_