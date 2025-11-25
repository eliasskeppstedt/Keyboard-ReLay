#ifndef _CONSTANTS_
#define _CONSTANTS_

#define MAX_QUEUE_SIZE 100
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
    READ_LAYERS           = 1,
    READ_LAYER_NR         = 2,
    READ_KEYCODE          = 3,
    READ_FROM             = 4,
    READ_TO_ON_PRESS      = 5,
    READ_TO_ON_HOLD       = 6,
    READ_LAYER_ENTRIES    = 8,
    READ_KEY_CODE_ENTRIES = 9,
    NO_VALUE_FROM         = 10
} ErrorCode;

typedef enum DataCodes {
    NULL_EVENT          = 100000, // check must be same as NONE key value from website
    ON_HOLD_TIMER_EVENT = 100001,
    SIMULATED_EVENT     = 100002,
    NO_VALUE            = 999999, // must also be same as website
} DataCodes;

#endif // _CONSTANTS_