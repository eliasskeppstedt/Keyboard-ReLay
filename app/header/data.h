#ifndef _DATA_
#define _DATA_

#include <stdbool.h>
#include <sys/time.h>

#define MAX_QUEUE_SIZE 100
#define PRINT_REMAP_TABLE_ENTRY(pRemapTableKeyEntry) printf("pRemapTable[%d]\n  code: %d \n  codeOnPress: %d \n  codeOnHold: %d \n  keyDown: %s\n\n", pRemapTableKeyEntry.code, pRemapTableKeyEntry.code, pRemapTableKeyEntry.codeOnPress, pRemapTableKeyEntry.codeOnHold, pRemapTableKeyEntry.keyDown ? "true" : "false");
// TODO user definable constants
#define TIME_FOR_ON_HOLD_EVENT_U_SEC 150000000 // x micro seconds
//

typedef enum UniversalEventType {
    KEY_UP, KEY_DOWN
} UniversalEventType;

/**
 * int code;
 * int eventFlagMask;
 * bool keyDown;
 */
typedef struct GeneralizedEvent {
    int code;
    int eventFlagMask;
    bool keyDown;
} GeneralizedEvent;

/**
 * GeneralizedEvent* event;
 * int head;
 * int tail;
 * bool isFull
 */
typedef struct EventQueue {
    GeneralizedEvent* buffer[MAX_QUEUE_SIZE];
    int head;
    int tail;
    bool isFull;
} EventQueue;

/**
 * int code;
 * int codeOnPress;
 * int codeOnHold;
 * bool keyDown;
 */
typedef struct UniversalKeyData {
    int code;
    int codeOnPress;
    int codeOnHold;
    bool keyDown;
} UniversalKeyData;

/**
 * int code;
 * int flagMask;
 * int timeStampUSecOnPress;
 * bool keyDown;
*/
typedef struct UniversalKeyStatus {
    int code;
    int flagMask;
    int timeStampUSecOnPress;
    bool keyDown;
} UniversalKeyStatus;

/**
 * int* pUniversalToOS;
 * int* pOSToUniversal;
 * int universalKeyEntries;
 * int osKeyEntries;
 * UniversalKeyStatus* pStatusTable;
 * EventQueue* pEventQueue;
 */
typedef struct LookUpTables {
    int* pUniversalToOS;
    int* pOSToUniversal;
    int universalKeyEntries;
    int osKeyEntries;
    UniversalKeyStatus* pStatusTable;
    EventQueue* pEventQueue;
} LookUpTables;

/**
 * char* layerName;
 * int layerNr;
 * UniversalKeyData* pRemapTable;
 */
typedef struct Layers {
    char* layerName;
    int layerNr;
    UniversalKeyData* pRemapTable;
} Layers;

/**
 * MAC
 */
typedef enum OS {
    MACOS, LINUX, WINDOWS
} OS;

#define NO_VALUE -1
#define MOD_ALREADY_ACTIVE 1
// EXIT CODES
#define EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED 1000
#define EXIT_CODE_CREATE_LAYER_ENTRIES_FAILED 1001
#define EXIT_CODE_CREATE_EVENT_QUEUE_FAILED 1002
#define EXIT_CODE_EVENT_QUEUE_FULL 1003
#define EXIT_CODE_DEBUG 1004
#define EXIT_CODE_INDEX_ERROR 1005

#endif // _DATA_