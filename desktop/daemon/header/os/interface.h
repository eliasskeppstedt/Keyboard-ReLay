#ifndef _INTERFACE_
#define _INTERFACE_

#include <sys/time.h>
#include "../data.h"

void postEvent(RLEvent* event, int userDefinedData);
void startOnHoldTimer(void** eventTimer);
void invalidateTimer(void** eventTimer);
int initRunLoop(MyReLay* myReLay);
void closeRunLoop(void* ctx);

void eventCallBack(MyReLay* myReLay, RLEvent* rlEvent);
uint64_t getTimeStamp();

void printRLEvent(RLEvent* rlEvent);


#endif // _INTERFACE_