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
    uint64_t modKeyFlag;
    bool isModKey;

    if (incomingEvent->code == NO_VALUE)
    {
        headEventPreview = getEvent(&myReLay->eventQueue, HEAD);
        goto timerTriggeredEvent;
    }

    enqueue(incomingEvent, &myReLay->eventQueue);
    headEventPreview = getEvent(&myReLay->eventQueue, HEAD);

    if (incomingEvent->keyDown && KEY_INFO(myReLay, incomingEvent).codeOnHold != NO_VALUE)
    {
        incomingEvent->state = PENDING;
        startOnHoldTimer(&incomingEvent->timer);
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
        myReLay->statusTable[headEventPreview->code].keyDown = true;
        headEventPreview->code = KEY_INFO(myReLay, headEventPreview).codeOnHold;
        headEventPreview->state = SEND;
        invalidateTimer(&headEventPreview->timer);
    } 
    else if (headEventPreview->code == incomingEvent->code && !incomingEvent->keyDown) // i.e. if it is a key down/key up pair
    {
        int codeOnPress = KEY_INFO(myReLay, headEventPreview).codeOnPress;
        if (codeOnPress != NO_VALUE)
        {
            headEventPreview->code = codeOnPress;
        }
        headEventPreview->state = SEND;
        invalidateTimer(&headEventPreview->timer);
    }

    goto notPending;
    timerTriggeredEvent:

    myReLay->statusTable[headEventPreview->code].keyDown = true;
    headEventPreview->code = KEY_INFO(myReLay, headEventPreview).codeOnHold;
    headEventPreview->state = SEND;

    notPending:

    if (!headEventPreview->keyDown)
    {
        if (myReLay->statusTable[headEventPreview->code].keyDown) // if true, then prev corresponding key down event got turned into a hold event
        {
            myReLay->statusTable[headEventPreview->code].keyDown = false;
            headEventPreview->code = KEY_INFO(myReLay, headEventPreview).codeOnHold;
        }
    }
    
    isModKey = modKeyRLFlag(headEventPreview->code, &modKeyFlag); //modKeyFlag is also set to corresponding flag if mod key
    if (isModKey && headEventPreview->state == SEND)
    {
        bool modKeyDown;
        if (headEventPreview->isModifier)
        {
            modKeyDown = MASK(headEventPreview->flagMask, modKeyFlag) != 0; // keyDown for native modifiers
            headEventPreview->keyDown = modKeyDown;
        } 
        else
        { 
            modKeyDown = headEventPreview->keyDown;
            if (headEventPreview->keyDown) headEventPreview->flagMask = modKeyFlag;
            myReLay->statusTable[headEventPreview->code].keyDown = false;
        }

        printf("modKeyFlag : %llu\n", modKeyFlag);
        printf("modkeydown : %s\n", modKeyDown ? "true" : "false");
        

        printf("key %s\n", modKeyDown ? "down\n" : "up\n");
        printf("active flags: %llu\n", activeEventFlags);
        if (modKeyDown)
        { 
            activeEventFlags |= headEventPreview->flagMask;
        }
        else
        {
            activeEventFlags &= headEventPreview->flagMask;
        }
        printf("active flags: %llu\n", activeEventFlags);
    }
    //printRLEvent(headEventPreview)
    
    while (headEventPreview)
    {
        if (headEventPreview->state == PENDING) break;
        outgoingEvent = dequeue(&myReLay->eventQueue);
        outgoingEvent->flagMask = activeEventFlags;
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