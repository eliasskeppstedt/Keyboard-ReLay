#ifndef _DATA_
#define _DATA_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100
#define SIMULATED_EVENT 1
#define ON_HOLD_TIMER_EVENT -2
#define DONT_POST_DUE_TO_PENDING_EVENT 3
#define NO_VALUE -1
#define ERROR_READ_JSON 100
#define ON_HOLD_THRESHOLD 150000 // 150 000 us => 150 ms 
#define CF_ON_HOLD_THRESHOLD 0.150 // ummm olika tidsenheter, byt till samma vid tillfälle 0.150 s => 150 ms

typedef enum OS {
    MACOS, LINUX, WINDOWS
} OS;

typedef enum EventState {
    NORMAL, PENDING, SEND
} EventState;

/*
 * @param int code;
 * @param int perserved code (for non supported event)
 * @param uint64_t flagMask;
 * @param uint64_t preservedOSFlagMask;
 * @param uint64_t timeStampOnPress;
 * @param EventState state; // for holding logic
 * @param bool isModifier;
 * @param bool keyDown;
 * @param bool if code is supported by 
 * @param void* timer for handling timer in different OS's, cast to the correct timer type
 */
typedef struct RLEvent {
    int code;
    uint64_t flagMask;
    uint64_t preservedOSFlagMask;
    uint64_t timeStampOnPress;
    EventState state; // for holding logic
    bool isModifier;
    bool keyDown;
    bool isSupported;
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
int code;
int codeOnPress;
int codeOnHold;
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

// only relevant for modifier keys
typedef struct KeyStatus {
    int code;
    int osCode;
    bool keyDown;
    int keysDown;
    int originalCodeDown;
} KeyStatus;

typedef struct MyReLay {
    KeyStatus* statusTable;
    EventQueue eventQueue;
    int activeLayer;
} MyReLay;

int* osToRL; // convertion tables
int* rlToOS; //
int osKeyEntries;
int rlKeyEntries;
int layerEntries;
KeyInfo** remapTable;

/*
 * @param int code from which to retrieve the on press code
 * @param int current active layer
 * @returns the on press code if it exists, else returns the regular code 
 */
int getCodeOnPress(int code, int activeLayer);

/*
 * @param int code from which to retrieve the on hold code
 * @param int current active layer
 * @returns the on hold code if it exists, else returns the regular code 
 */
int getCodeOnHold(int code, int activeLayer);

/*
 * @brief if possible change to new layer, else stay on current layer
 * @param int change to this layer
 * @param int* pointer to the active layer variable within MyReLay struct
 */
void changeToLayer(int layer, int* activeLayer);

#endif // _DATA_