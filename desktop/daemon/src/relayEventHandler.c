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
    EventQueue* eventQueue = &myReLay->eventQueue;
    // printRLEvent(incomingEvent);
    if (activeEventFlags & RL_SHIFT_MASK && incomingEvent->code == RL_ESCAPE) 
    {
        free(incomingEvent);
        while ((incomingEvent = dequeue(eventQueue))) 
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
        headEventPreview = getEvent(eventQueue, HEAD);
        goto timerTriggeredEvent;
    }

    enqueue(incomingEvent, eventQueue);
    headEventPreview = getEvent(eventQueue, HEAD);

    while (headEventPreview)
    {
        if (!headEventPreview->isSupported)
        {
            printf("  key code (%d) not supported!!!\n", headEventPreview->code);
            goto unsupportedKeyCode;
        }

        if (incomingEvent->keyDown && getCodeOnHold(incomingEvent->code, myReLay->activeLayer) != NO_VALUE)
        {
            incomingEvent->state = PENDING;
            startOnHoldTimer(&incomingEvent->timer);
        }
        else
        {
            incomingEvent->state = SEND;
        }

        int headEventCodeOnPress = getCodeOnPress(headEventPreview->code, myReLay->activeLayer);
        if (headEventPreview->state != PENDING) 
        {
            if (headEventCodeOnPress != NO_VALUE)
            {
                headEventPreview->code = headEventCodeOnPress;
            }
            goto notPending;
        }


        if (headEventPreview->code == incomingEvent->code && !incomingEvent->keyDown) // i.e. if it is a key down/key up pair
        {
            printf("  keyDown/keyUp pair\n");
            if (headEventCodeOnPress != NO_VALUE)
            {
                headEventPreview->code = headEventCodeOnPress;
            }
            headEventPreview->state = SEND;
            invalidateTimer(&headEventPreview->timer);
        }

        goto notPending;
        timerTriggeredEvent:
        
        myReLay->statusTable[headEventPreview->code].keyDown = true;
        headEventPreview->code = getCodeOnHold(headEventPreview->code, myReLay->activeLayer);
        headEventPreview->state = SEND;

        notPending:

        if (!headEventPreview->keyDown)
        {
            if (myReLay->statusTable[headEventPreview->code].keyDown) // if true, then prev corresponding key down event got turned into a hold event
            {
                myReLay->statusTable[headEventPreview->code].keyDown = false;
                if (headEventCodeOnPress != NO_VALUE) headEventPreview->code = headEventCodeOnPress;
                if (getCodeOnHold(headEventPreview->code, myReLay->activeLayer) != NO_VALUE) headEventPreview->code = getCodeOnHold(headEventPreview->code, myReLay->activeLayer);
            }
        }

        unsupportedKeyCode:
        
        isModKey = setCorrespondingModifierFlag(headEventPreview->code, &modKeyFlag); //modKeyFlag is also set to corresponding flag if mod key
        if (isModKey && headEventPreview->state == SEND)
        {

            bool modKeyDown;
            if (headEventPreview->isModifier)
            {
                modKeyDown = ((headEventPreview->flagMask & modKeyFlag) != 0); // keyDown for native modifiers
                headEventPreview->keyDown = modKeyDown;
            } 
            else
            { 
                modKeyDown = headEventPreview->keyDown;
            }
            headEventPreview->flagMask = modKeyFlag;
            
            if (modKeyDown)
            { 
                activeEventFlags |= headEventPreview->flagMask;
            }
            else
            {
                activeEventFlags ^= headEventPreview->flagMask;
            }
        }            
        printf("current active flags: %llu\n", activeEventFlags);

        //printRLEvent(headEventPreview)

        // POST EVENT
        if (headEventPreview->state == PENDING) break;
        outgoingEvent = dequeue(eventQueue);
        outgoingEvent->flagMask = activeEventFlags;
        postEvent(outgoingEvent, SIMULATED_EVENT);
        free(outgoingEvent);
        headEventPreview = getEvent(eventQueue, HEAD);
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