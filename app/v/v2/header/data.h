#ifndef _DATA_
#define _DATA_

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

/*
@param EventQueue* eventQueue
*/
#define MAX_QUEUE_SIZE 100
#define PRINT_REMAP_TABLE_ENTRY(remapTableKeyEntry) printf("pRemapTable[%d]\n  code: %d \n  codeOnPress: %d \n  codeOnHold: %d \n  keyDown: %s\n\n", remapTableKeyEntry.code, remapTableKeyEntry.code, remapTableKeyEntry.codeOnPress, remapTableKeyEntry.codeOnHold, remapTableKeyEntry.keyDown ? "true" : "false");
#define hasCodeOnPress(remapTable, event) remapTable[event->code].codeOnPress != NO_VALUE ? true : false
#define hasCodeOnHold(remapTable, event) remapTable[event->code].codeOnHold != NO_VALUE ? true : false
//

/*
MAC
LINUX
WINDOWS
*/
typedef enum OS {
    MACOS, LINUX, WINDOWS
} OS;

typedef enum KREventType {
    KEY_UP, KEY_DOWN
} KREventType;

typedef enum EventState {
    NORMAL, PENDING, ACTIVE
} EventState;

/*
int code;
int flagMask;
uint64_t timeStampOnPress;
bool keyDown;
*/
typedef struct GeneralizedEvent {
    int code;
    int flagMask;
    uint64_t timeStampOnPress;
    bool keyDown;
} GeneralizedEvent;

/*
GeneralizedEvent* buffer[MAX_QUEUE_SIZE];
GeneralizedEvent* bufferReadyForDispatch[MAX_QUEUE_SIZE];
int head;
int tail;   
bool isFull;
*/
typedef struct EventQueue {
    GeneralizedEvent* buffer[MAX_QUEUE_SIZE];
    GeneralizedEvent* bufferReadyForDispatch[MAX_QUEUE_SIZE];
    int head;
    int tail;   
    bool isFull;
} EventQueue;

/*
int code;
int codeOnPress;
int codeOnHold;
bool keyDown;
*/
typedef struct KRKeyData {
    int code;
    int codeOnPress;
    int codeOnHold;
    bool keyDown;
} KRKeyData;

/*
int code;
bool keyDown; // for marking the event tupe
EventState state; // for holding logic
*/
typedef struct KRKeyStatus {
    int code;
    bool keyDown; // for marking the event tupe
    EventState state; // for holding logic
} KRKeyStatus;


/*
int* krToOS;
int* osToKR;
int krKeyEntries;
int osKeyEntries;
KRKeyStatus* statusTable;
EventQueue* eventQueue;
*/
typedef struct LookUpTables {
    int* krToOS;
    int* osToKR;
    int krKeyEntries;
    int osKeyEntries;
    KRKeyStatus* statusTable;
    EventQueue* eventQueue;
} LookUpTables;

/*
char* name;
int nr;
KRKeyData* remapTable;
*/
typedef struct Layer {
    char* name;
    int nr;
    KRKeyData* remapTable;
} Layer;

#define NO_VALUE -1
#define MOD_ALREADY_ACTIVE 1
#define kKRSimulatedEventAutorepeat 8 
#define kKREventTypeKeyDown 9
#define kKREventTypeKeyUp 10
#define kKREventSupress 11
// new section
// new section
#define U_SEC_FOR_ON_HOLD_EVENT 150 // 150 000 micro sec => 150 milli sec, TODO user uuh... choosable... constant
#define U_SEC_FOR_AUTOREPEAT_DETECTION 1000 // 1 000 micro sec => 1 milli sec
//
// EXIT CODES
#define EXIT_CODE_CREATE_LOOK_UP_TABLE_FAILED 1000
#define EXIT_CODE_CREATE_LAYER_ENTRIES_FAILED 1001
#define EXIT_CODE_CREATE_EVENT_QUEUE_FAILED 1002
#define EXIT_CODE_EVENT_QUEUE_FULL 1003
#define EXIT_CODE_DEBUG 1004
#define EXIT_CODE_INDEX_ERROR 1005

#endif // _DATA_