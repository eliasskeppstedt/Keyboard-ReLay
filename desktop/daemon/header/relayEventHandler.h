#ifndef _RELAYEVENTHANDLER_
#define _RELAYEVENTHANDLER_

#include "os/interface.h"
#include "eventQueue.h"
#include "converter.h"

#define RL_ESCAPE 0x30

int start(MyReLay* myReLay);
uint64_t getTimeStamp();

#endif // _RELAYEVENTHANDLER_