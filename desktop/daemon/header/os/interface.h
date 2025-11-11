#ifndef _INTERFACE_
#define _INTERFACE_

#include <sys/time.h>
#include "../data.h"

/*
 * @attention interface.h
 * @param RLEvent* relay event to post
 * @param int some user defined data
 */
void postEvent(RLEvent* rlEvent, UserDefinedData userDefinedData);

/*
 * @attention interface.h
 */
void startOnHoldTimer(void** eventTimer);

/*
 * @attention interface.h
 */
void invalidateTimer(void** eventTimer);

/*
 * @attention interface.h
 */
int initRunLoop();

/*
 * @attention interface.h
 */
void closeRunLoop(void* ctx);

/*
 * @attention interface.h
 */
void eventCallBack(RLEvent* incomingEvent);

/*
 * @attention interface.h
 */
uint64_t getTimeStamp();

/*
 * @attention interface.h
 */
void printRLEvent(RLEvent* rlEvent);

/*
 * @attention interface.h
 * @brief set the ModFlag struct´s values to be the specific os´s flags
 */
void setModFlags(ModKeys* flagMask);

/*
 * @attention interface.h
 * @param int code from which to retrieve the key info
 * @return KeyInfo* keyInfo for given code if exists else NULL  
 */
KeyInfo* getKeyInfo(int code);

/*
 * @attention interface.h
 * @param int int code from which to retreive the key status
 * @return KeyStatus* keyStatus for given code if exists else NULL  
 */
KeyStatus* getKeyStatus(int code);

/*
 * @attention interface.h
 * @brief if possible update to new layer, else stay on current layer
 * @param int update to this layer
 */
void changeToLayer(int layer);

int getLayerEntries();

int getActiveLayer();

void deleteMyReMap();


#endif // _INTERFACE_
