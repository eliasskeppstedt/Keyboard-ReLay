#ifndef _DATA_
#define _DATA_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100
#define SIMULATED_EVENT 1
#define ON_HOLD_TIMER_EVENT 2
#define NO_VALUE -1
#define ERROR_READ_JSON 100
#define ON_HOLD_THRESHOLD 150000 // 150 000 us => 150 ms 

typedef enum OS {
    MACOS, LINUX, WINDOWS
} OS;

typedef enum EventState {
    NORMAL, PENDING, SEND
} EventState;

/*
int code;
uint64_t flagMask;
uint64_t timeStampOnPress;
EventState state; // for holding logic
bool isModifier;
bool keyDown;
*/
typedef struct RLEvent {
    int code;
    uint64_t flagMask;
    uint64_t timeStampOnPress;
    EventState state; // for holding logic
    bool isModifier;
    bool keyDown;
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

typedef struct KeyStatus {
    bool keyDown;
    void* timer; // for handling timer in different OS's, cast to the correct timer type
} KeyStatus;

typedef struct MyReLay {
    KeyInfo** remapTable;
    int activeLayer;
    KeyStatus* statusTable;
    EventQueue eventQueue;
    int* osToRL;
    int* rlToOS;
    int osKeyEntries;
    int rlKeyEntries;
} MyReLay;

#endif // _DATA_