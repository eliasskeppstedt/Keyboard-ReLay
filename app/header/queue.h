#ifndef _QUEUE_
#define _QUEUE_

#include "dataStorage.h"

#define QUEUE_SIZE 20

struct keyData* createKeyQueue();
struct keyData* addKey(struct keyData*, struct keyData*);
struct keyData* removeKey(struct keyData*);

#endif // _QUEUE_