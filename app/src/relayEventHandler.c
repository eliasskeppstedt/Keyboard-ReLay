#include "../header/relayEventHandler.h"

int start(MyReLay* myReLay)
{
    initRunLoop(myReLay);
    return 0;
}

// defined in interfaces.h
void eventCallBack(MyReLay* myReLay, RLEvent* event)
{
    if (event->code == RL_ESCAPE) 
    {
        closeRunLoop(NULL); 
        return;
    }
    postEvent(event, myReLay->rlToOS);
}

uint64_t getTimeStamp()
{
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    return (uint64_t)timeStamp.tv_sec * 1000000ULL + (uint64_t)timeStamp.tv_usec; // ULL - unsigned long long
}

void printRLEvent(RLEvent* rlEvent)
{
    printf("  rlEvent {\n");
    printf("    code: %d\n", rlEvent->code);
    printf("    flagMask: %llu\n", rlEvent->flagMask);
    printf("    timeStampOnPress: %llu\n", rlEvent->timeStampOnPress);
    char* state = "[state]";
    if (rlEvent->state == NORMAL) { state = "NORMAL"; }
    printf("    state: %s\n", state);
    printf("    isModifier: %s\n", rlEvent->isModifier ? "true" : "false");
    printf("    keyDown: %s\n", rlEvent->keyDown ? "true" : "false");
    printf("  }\n");
}