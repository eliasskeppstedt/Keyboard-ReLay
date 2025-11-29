#ifndef _INTERFACE_RELAYEVENTHANDLER_
#define _INTERFACE_RELAYEVENTHANDLER_

#include <sys/time.h>
#include <stdint.h>

/*
 * uint64_t code;
 * uint64_t codeOnPress;
 * uint64_t codeOnHold;
 */
typedef struct KeyInfo {
    uint64_t code;
    uint64_t codeOnPress;
    uint64_t codeOnHold;
} KeyInfo;

/*
    uint64_t code
    int keysDown;
    bool keyDown;
*/
typedef struct KeyStatus {
    uint64_t code;
    bool keyDown;
    bool onHold;
} KeyStatus;

/*
 * @param uint64_t code;
 * @param uint64_t flagMask;
 * @param uint64_t preservedFlagMask;
 * @param uint64_t timeStampOnPress;
 * @param EventState state; // for holding logic
 * @param bool isModifier;
 * @param bool keyDown;
 * @param void* timer; // for handling timer in different OS's, cast to the correct timer type
 */
typedef struct RLEvent {
    uint64_t code;
    uint64_t flagMask;
    uint64_t timeStampOnPress;
    EventState state; // for holding logic
    bool isModifier;
    bool keyDown;
    void* timer; // for handling timer in different OS's, cast to the correct timer type
} RLEvent;

/*
 * @if on MACOS
 * @param void* remaps is an array of CFMutableDictionaryRef
 * @param void* statusTable is a CFMutableDictionaryRef
 * @param int activeLayer initially set from initRemapTable
 * @param int layerEntries set from initRemapTable
 */
typedef struct MyReLay {
    void* remaps;
    void* statusTable;
    int activeLayer;
    int layerEntries;
} MyReLay;

typedef struct ModKeys { // hm better way??
    // masks
    uint64_t CAPS_LOCK_MASK;
    uint64_t SHIFT_MASK;
    uint64_t CONTROL_MASK;
    uint64_t ALTERNATE_MASK;
    uint64_t META_MASK;
    uint64_t L_SHIFT_MASK;
    uint64_t R_SHIFT_MASK;
    uint64_t L_CONTROL_MASK;
    uint64_t R_CONTROL_MASK;
    uint64_t L_ALTERNATE_MASK;
    uint64_t R_ALTERNATE_MASK;
    uint64_t L_META_MASK;
    uint64_t R_META_MASK;
    // mac specific masks ig?
    uint64_t NUMERIC_PAD_MASK;
    uint64_t HELP_MASK;
    uint64_t SECONDARY_FN_MASK;
    // codes
    uint64_t CAPS_LOCK_CODE;
    uint64_t SHIFT_CODE;
    uint64_t CONTROL_CODE;
    uint64_t ALTERNATE_CODE;
    uint64_t META_CODE;
    uint64_t L_SHIFT_CODE;
    uint64_t R_SHIFT_CODE;
    uint64_t L_CONTROL_CODE;
    uint64_t R_CONTROL_CODE;
    uint64_t L_ALTERNATE_CODE;
    uint64_t R_ALTERNATE_CODE;
    uint64_t L_META_CODE;
    uint64_t R_META_CODE;
    // mac specific masks ig?
    uint64_t NUMERIC_PAD_CODE;
    uint64_t HELP_CODE;
    uint64_t SECONDARY_FN_CODE;
} ModKeys;

/*
 * @attention relayEventHandler.h
 * @param uint64_t code, bool keyDown, uint64_t flagMask, uint64_t timeStamp, UserDefinedData userDefinedData
 */
void postEvent(uint64_t code, bool keyDown, uint64_t flagMask, uint64_t timeStamp, DataCodes userDefinedData);  

/*
 * @attention relayEventHandler.h
 * @return RLEvent* created rl event, must be freed when posted!
 */
RLEvent* RLEventCreate(uint64_t code, bool keyDown, uint64_t flagMask, uint64_t timeStamp, bool isModifier, DataCodes userDefinedData);

/*
 * @attention relayEventHandler.h
 */
void startOnHoldTimer(void** eventTimer);

/*
 * @attention relayEventHandler.h
 */
void invalidateTimer(void** eventTimer);

/*
 * @attention relayEventHandler.h
 */
int initRunLoop();

/*
 * @attention relayEventHandler.h
 */
void closeRunLoop(void* ctx);

/*
 * @attention relayEventHandler.h
 */
void eventCallBack(RLEvent* incomingEvent);

/*
 * @attention relayEventHandler.h
 */
uint64_t getTimeStamp();

/*
 * @attention relayEventHandler.h
 */
void printRLEvent(RLEvent* rlEvent);

/*
 * @attention relayEventHandler.h
 * @brief set the ModFlag struct´s values to be the specific os´s flags
 */
void setModFlags(ModKeys* flagMask);

/*
 * @attention relayEventHandler.h
 * @param int code from which to retrieve the key info
 * @return KeyInfo* keyInfo for given code if exists else NULL  
 */
KeyInfo* getKeyInfo(uint64_t code);

/*
 * @attention relayEventHandler.h
 * @param int int code from which to retreive the key status
 * @return KeyStatus* keyStatus for given code if exists else NULL  
 */
KeyStatus* getKeyStatus(uint64_t code);

/*
 * @attention relayEventHandler.h
 * @brief if possible update to new layer, else stay on current layer
 * @param int update to this layer
 */
void changeToLayer(int layer);

int getLayerEntries();

int getActiveLayer();

void deleteMyReMap();

#endif // _INTERFACE_RELAYEVENTHANDLER_