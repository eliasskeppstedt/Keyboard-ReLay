#ifndef _DATA_
#define _DATA_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NO_VALUE -1

#define MAX_QUEUE_SIZE 100
#define ERROR_READ_JSON 100
//#define ON_HOLD_THRESHOLD 150000 // 150 000 us => 150 ms 
#define CF_ON_HOLD_THRESHOLD 0.150 // ummm olika tidsenheter, byt till samma vid tillfälle 0.150 s => 150 ms
#define ACTIVE_LAYER "active layer"

typedef enum OS {
    MACOS, LINUX, WINDOWS
} OS;

typedef enum EventState {
    NORMAL, PENDING, SEND
} EventState;

typedef enum ErrorCode {
    READ_LAYERS = 1,
    READ_LAYER_NR = 2,
    READ_KEYCODE = 3,
    READ_FROM = 4,
    READ_TO_ON_PRESS = 5,
    READ_TO_ON_HOLD = 6,
    READ_LAYER_ENTRIES = 8,
    READ_KEY_CODE_ENTRIES = 9,
    NO_VALUE_FROM = 10
} ErrorCode;

typedef enum UserDefinedData {
    SIMULATED_EVENT = 1,
    ON_HOLD_TIMER_EVENT = 2,
    EVENT_STILL_PENDING = 3
} UserDefinedData;

/*
 * @param int code;
 * @param uint64_t flagMask;
 * @param uint64_t preservedFlagMask;
 * @param uint64_t timeStampOnPress;
 * @param EventState state; // for holding logic
 * @param bool isModifier;
 * @param bool keyDown;
 * @param void* timer; // for handling timer in different OS's, cast to the correct timer type
 */
typedef struct RLEvent {
    int code;
    uint64_t flagMask;
    uint64_t timeStampOnPress;
    EventState state; // for holding logic
    bool isModifier;
    bool keyDown;
    void* timer; // for handling timer in different OS's, cast to the correct timer type
} RLEvent;

/*
RLEvent* buffer[MAX_QUEUE_SIZE];
int head;
int tail;   
bool isFull;
*/
typedef struct EventQueue {
    RLEvent* buffer[MAX_QUEUE_SIZE];
    int head;
    int tail;   
    bool isFull;
} EventQueue;

/*
 * int code;
 * int codeOnPress;
 * int codeOnHold;
 */
typedef struct KeyInfo {
    int code;
    int codeOnPress;
    int codeOnHold;
} KeyInfo;

/*
KeyData* mappings; // mappings[LAYER][KEY]
int activeLayer; // to index into correct layer
    bool* keyDownTable;
    EventQueue eventQueue;
    int* osToRL;
    int* rlToOS;
    int keyEntries;
*/

/*
    int code
    int keysDown;
    bool keyDown;
*/
typedef struct KeyStatus {
    int code;
    bool keyDown;
    bool onHold;
} KeyStatus;

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

#endif // _DATA_