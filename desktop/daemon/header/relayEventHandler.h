#ifndef _RELAYEVENTHANDLER_
#define _RELAYEVENTHANDLER_

#include "os/interface.h"
#include "eventQueue.h"

/*
 * @attention relayEventHandler.h
 */
void setEscapeCode(int code);

/*
 * @attention relayEventHandler.h
 */
int start();

/*
 * @attention relayEventHandler.h
 * @attention IF code corresponds to a modifier code, flagMask's value will be set to that specific modifier´s flag, 
 * @attention ELSE flagMask´s value is set to 0
 * @param int code
 * @param uint64_t* flagMask
 */
void setCorrespondingModifierFlag(int code, uint64_t* flagMask);


#endif // _RELAYEVENTHANDLER_