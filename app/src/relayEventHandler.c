#include "../header/relayEventHandler.h"

static uint64_t activeEventFlags = 0;

int start(MyReLay* myReLay)
{
    initRunLoop(myReLay);
    return 0;
}

// defined in interfaces.h
void eventCallBack(MyReLay* myReLay, RLEvent* incomingEvent)
{   
    // printRLEvent(incomingEvent);
    if (incomingEvent->code == RL_ESCAPE) 
    {
        free(incomingEvent);
        while ((incomingEvent = dequeue(&myReLay->eventQueue))) 
        {
            free(incomingEvent);
        }
        closeRunLoop(NULL); 
        return;
    }

    RLEvent* headEventPreview = NULL;
    RLEvent* outgoingEvent = NULL;

    enqueue(incomingEvent, &myReLay->eventQueue);
    headEventPreview = getEvent(&myReLay->eventQueue, HEAD);

    if (incomingEvent->keyDown && KEY_INFO(myReLay, incomingEvent).codeOnHold != NO_VALUE)
    {
        incomingEvent->state = PENDING;
    }
    else
    {
        incomingEvent->state = SEND;
    }

    //printf("\n> current time       : %llu\n", getTimeStamp());
    //printf("> time on press      : %llu\n", incomingEvent->timeStampOnPress);
    //printf("> time since pressed : %llu\n", getTimeStamp() - incomingEvent->timeStampOnPress);


    if (headEventPreview->state != PENDING) 
    {
        if (KEY_INFO(myReLay, headEventPreview).codeOnPress != NO_VALUE)
        {
            headEventPreview->code = KEY_INFO(myReLay, headEventPreview).codeOnPress;
        }
        goto notPending;
    }

    if ((getTimeStamp() - headEventPreview->timeStampOnPress) > ON_HOLD_THRESHOLD)
    {
        headEventPreview->code = KEY_INFO(myReLay, headEventPreview).codeOnHold;
        headEventPreview->state = SEND;
    }
    else if (headEventPreview->code == incomingEvent->code && !incomingEvent->keyDown) // i.e. if it is a key down/key up pair
    {
        int codeOnPress = KEY_INFO(myReLay, headEventPreview).codeOnPress;
        if (codeOnPress != NO_VALUE)
        {
            headEventPreview->code = codeOnPress;
        }
        headEventPreview->state = SEND;
    }
    notPending:

    if (headEventPreview->isModifier && headEventPreview->state == SEND)
    {
        uint64_t modKeyFlag = 0;
        modKeyRLFlag(headEventPreview->code, &modKeyFlag);
        if (MASK(headEventPreview->flagMask, modKeyFlag) != 0) // keyDown for both modifiers AND regular characters
        {
            activeEventFlags |= headEventPreview->flagMask;
        }
        else
        {
            activeEventFlags &= headEventPreview->flagMask;
        }
    }
    printRLEvent(headEventPreview);

    while (headEventPreview)
    {
        if (headEventPreview->state == PENDING) break;

        headEventPreview->flagMask = activeEventFlags;
        outgoingEvent = dequeue(&myReLay->eventQueue);
        postEvent(outgoingEvent, myReLay->rlToOS, SIMULATED_EVENT);
        free(outgoingEvent);
        headEventPreview = getEvent(&myReLay->eventQueue, HEAD);
    }
}

uint64_t getTimeStamp()
{
    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);
    return (uint64_t)timeStamp.tv_sec * 1000000ULL + (uint64_t)timeStamp.tv_usec; // return value in micro seconds
}

void printRLEvent(RLEvent* rlEvent)
{
    printf("> rlEvent {\n");
    printf(">   code: %d\n", rlEvent->code);
    printf(">   flagMask: %llu\n", rlEvent->flagMask);
    printf(">   timeStampOnPress: %llu\n", rlEvent->timeStampOnPress);
    char* state = "[state]";
    if (rlEvent->state == NORMAL) { state = "NORMAL"; }
    printf(">   state: %s\n", state);
    printf(">   isModifier: %s\n", rlEvent->isModifier ? "true" : "false");
    printf(">   keyDown: %s\n", rlEvent->keyDown ? "true" : "false");
    printf("> }\n");
}