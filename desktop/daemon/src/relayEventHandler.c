#include "../header/relayEventHandler.h"
#include <stdint.h>

/* @attention relayEventHandler.c */
static ModKeys MOD_KEYS;
static uint64_t activeEventFlags = 0;
static int ESCAPE = 53;

/* if the event flag is not 0 then the mod flag is on, must be preceded by setCorrespondingModifierFlag */
static bool isModDown(uint64_t flagMask);
static void removeModFlags(uint64_t* flagMask);
static bool isModCode(int code);

int start()
{
    setModFlags(&MOD_KEYS); // uh naming is inconsistent, but this struct has mod key members and they are set by the os specific file in order to access their respective mod flags
    initRunLoop();
    return 0;
}

// defined in interfaces.h
void eventCallBack(RLEvent* incomingEvent)
{   
    const int incomingEventCode = incomingEvent->code;
    
    if (incomingEventCode == ESCAPE) 
    {
        bool shiftDown = activeEventFlags & MOD_KEYS.SHIFT_MASK;

        if(shiftDown)
        {
            printf("runloop closes from relayEventHandler\n");
            free(incomingEvent);
            while ((incomingEvent = dequeue())) 
            {
                free(incomingEvent);
            }
            closeRunLoop(NULL); 
            return;
        }
    }
    
    RLEvent* outgoingEvent = NULL;

    int code = NO_VALUE;
    RLEvent* headEventPreview = NULL;
    KeyInfo* keyInfo = NULL;

    KeyStatus* keyStatus = getKeyStatus(incomingEventCode);

    // handle incoming event //

    if (incomingEventCode == NO_VALUE) 
    {
        // unvalid event, is from timer -> key hold event (only exist if a hold mapping exist hence keyInfo and Status is not null)
        free(incomingEvent);

        headEventPreview = getEvent(HEAD);
        code = headEventPreview->code;
        keyInfo = getKeyInfo(code);
        keyStatus = getKeyStatus(code);
        
        headEventPreview->code = keyInfo->codeOnHold;
        headEventPreview->state = SEND;
        
        keyStatus->onHold = true;

        goto checkEvent;
    }
    else // if event is valid
    {
        enqueue(incomingEvent);
        headEventPreview = getEvent(HEAD);
        

        if (!getKeyInfo(incomingEventCode)) 
        {
            // if incoming event hasn´t modification, skip to post
            goto postEvent;
        } 
        else // if event has modification
        {
            // if keystatus keyDown is false, it means that this event MUST be a press event
            // since i only allow 1 type of a key per layer.
            bool isPressEvent = !keyStatus->keyDown;
            incomingEvent->keyDown = isPressEvent;
            keyStatus->keyDown = isPressEvent;

            if (isPressEvent && getKeyInfo(incomingEventCode)->codeOnHold != NO_VALUE)
            {
                // press event and POTENTIAL on hold event
                incomingEvent->state = PENDING;
                startOnHoldTimer(&incomingEvent->timer);
            }
            else 
            {
                incomingEvent->state = SEND;
            }
        }
    }

    // handle outgoing events //

    while (headEventPreview)
    {
        code = headEventPreview->code;
        keyInfo = getKeyInfo(code);
        keyStatus = getKeyStatus(code);

        if (!keyInfo) goto postEvent; // no remap table entry available

        const int codeOnPress = keyInfo->codeOnPress;
        
        if (headEventPreview->state == PENDING) 
        {
            if (code == incomingEventCode && !keyStatus->keyDown) 
            {
                // key press/release pair (pending event always down, and if the status is down, then incoming event is a release event)
                if (codeOnPress != NO_VALUE)
                {
                    headEventPreview->code = codeOnPress;
                    incomingEvent->code = codeOnPress;
                }
                headEventPreview->state = SEND;
                keyStatus->onHold = false;

                if (headEventPreview->timer) 
                {
                    invalidateTimer(&headEventPreview->timer); // should always exist a timer on this event tho
                }
                else 
                {
                    printf("                          timer did not exist o.o???\n");
                }
            }
            else // if not a key press/release pair
            {
                goto postEvent;
            }
        }
        else // if not pending
        {
            if (codeOnPress != NO_VALUE)
            {
                headEventPreview->code = codeOnPress;
            }
        }
        
        checkEvent:

        if (keyStatus->onHold)
        {
            // has already become a hold event making this a on hold release event
            //headEventPreview->code = keyInfo->codeOnHold;
            headEventPreview->code = keyInfo->codeOnHold;
        }

        uint64_t flagMask = getCorrespondingModifierFlag(headEventPreview->code);
        
        // key code corresponds to a modifier key
        //Event(headEventPreview);
        if (headEventPreview->keyDown) // event must be press
        {
            if (flagMask) activeEventFlags |= flagMask;
            keyStatus->keyDown = true;
        }
        else // event must be release
        {
            if (flagMask) activeEventFlags &= ~flagMask;
            keyStatus->keyDown = false;
        }
        printf("active ev flag: %llu\n", activeEventFlags);
        
        postEvent:

        if (headEventPreview->state == PENDING) break;

        removeModFlags(&headEventPreview->flagMask);

        outgoingEvent = dequeue();
        //printf("POST EVENT:\n");
        printRLEvent(outgoingEvent);

        outgoingEvent->flagMask |= activeEventFlags;
        postEvent(outgoingEvent, SIMULATED_EVENT);
        free(outgoingEvent);
        headEventPreview = getEvent(HEAD);
    }
}

uint64_t getCorrespondingModifierFlag(int code)
{
    uint64_t flagMask = 0;

    if (code == MOD_KEYS.L_SHIFT_CODE)
    {
        flagMask = MOD_KEYS.SHIFT_MASK | MOD_KEYS.L_SHIFT_MASK;
    }
    else if (code == MOD_KEYS.R_SHIFT_CODE) 
    {
        flagMask = MOD_KEYS.SHIFT_MASK | MOD_KEYS.R_SHIFT_MASK;
    }
    else if (code == MOD_KEYS.L_CONTROL_CODE) 
    {
        flagMask = MOD_KEYS.CONTROL_MASK | MOD_KEYS.L_CONTROL_MASK;
    }
    else if (code == MOD_KEYS.R_CONTROL_CODE) 
    {
        flagMask = MOD_KEYS.CONTROL_MASK | MOD_KEYS.R_CONTROL_MASK;
    }
    else if (code == MOD_KEYS.L_META_CODE) 
    {
        flagMask = MOD_KEYS.META_MASK | MOD_KEYS.L_META_MASK;
    }
    else if (code == MOD_KEYS.R_META_CODE) 
    {
        flagMask = MOD_KEYS.META_MASK | MOD_KEYS.R_META_MASK;
    }
    else if (code == MOD_KEYS.L_ALTERNATE_CODE) 
    {
        flagMask = MOD_KEYS.ALTERNATE_MASK | MOD_KEYS.L_ALTERNATE_MASK;
    }
    else if (code == MOD_KEYS.R_ALTERNATE_CODE) 
    {
        flagMask = MOD_KEYS.ALTERNATE_MASK | MOD_KEYS.R_ALTERNATE_MASK;
    }
    else if (code == MOD_KEYS.CAPS_LOCK_CODE) 
    {
        flagMask = MOD_KEYS.CAPS_LOCK_MASK;
    }
    return flagMask;
}

static void removeModFlags(uint64_t* flagMask)
{
    *flagMask &= ~(
        MOD_KEYS.SHIFT_MASK     | MOD_KEYS.L_SHIFT_MASK     | MOD_KEYS.R_SHIFT_MASK     |
        MOD_KEYS.CONTROL_MASK   | MOD_KEYS.L_CONTROL_MASK   | MOD_KEYS.R_CONTROL_MASK   |
        MOD_KEYS.META_MASK      | MOD_KEYS.L_META_MASK      | MOD_KEYS.R_META_MASK      |
        MOD_KEYS.ALTERNATE_MASK | MOD_KEYS.L_ALTERNATE_MASK | MOD_KEYS.R_ALTERNATE_MASK |
        MOD_KEYS.CAPS_LOCK_MASK
    );
    *flagMask |= 256; // seems like this is some kind of standard flag on all events
}

static bool isModCode(int code)
{
    return (
        code == MOD_KEYS.SHIFT_MASK     || code == MOD_KEYS.L_SHIFT_MASK     || code == MOD_KEYS.R_SHIFT_MASK     || 
        code == MOD_KEYS.CONTROL_MASK   || code == MOD_KEYS.L_CONTROL_MASK   || code == MOD_KEYS.R_CONTROL_MASK   || 
        code == MOD_KEYS.META_MASK      || code == MOD_KEYS.L_META_MASK      || code == MOD_KEYS.R_META_MASK      || 
        code == MOD_KEYS.ALTERNATE_MASK || code == MOD_KEYS.L_ALTERNATE_MASK || code == MOD_KEYS.R_ALTERNATE_MASK || 
        code == MOD_KEYS.CAPS_LOCK_MASK
    );
}

void initModFlags()
{
    MOD_KEYS = (ModKeys) {
        .CAPS_LOCK_MASK     = 0,
        .SHIFT_MASK         = 0,
        .CONTROL_MASK       = 0,
        .ALTERNATE_MASK     = 0,
        .META_MASK          = 0,
        .L_SHIFT_MASK       = 0,
        .R_SHIFT_MASK       = 0,
        .L_CONTROL_MASK     = 0,
        .R_CONTROL_MASK     = 0,
        .L_ALTERNATE_MASK   = 0,
        .R_ALTERNATE_MASK   = 0,
        .L_META_MASK        = 0,
        .R_META_MASK        = 0,
        .NUMERIC_PAD_MASK   = 0,
        .HELP_MASK          = 0,
        .SECONDARY_FN_MASK  = 0,
        .CAPS_LOCK_CODE     = 0,
        .SHIFT_CODE         = 0,
        .CONTROL_CODE       = 0,
        .ALTERNATE_CODE     = 0,
        .META_CODE          = 0,
        .L_SHIFT_CODE       = 0,
        .R_SHIFT_CODE       = 0,
        .L_CONTROL_CODE     = 0,
        .R_CONTROL_CODE     = 0,
        .L_ALTERNATE_CODE   = 0,
        .R_ALTERNATE_CODE   = 0,
        .L_META_CODE        = 0,
        .R_META_CODE        = 0,
        .NUMERIC_PAD_CODE   = 0,
        .HELP_CODE          = 0,
        .SECONDARY_FN_CODE  = 0
    };
}

void printRLEvent(RLEvent* rlEvent)
{
    printf("> rlEvent {\n");
    printf(">   code: %d\n", rlEvent->code);
    printf(">   flagMask: %llu\n", rlEvent->flagMask);
    printf(">   timeStampOnPress: %llu\n", rlEvent->timeStampOnPress);
    char* state = "[state]";
    if (rlEvent->state == NORMAL) { state = "NORMAL"; }
    printf(">   state (SEND?): %s\n", rlEvent->state == SEND ? "true" : "false");
    printf(">   isModifier: %s\n", rlEvent->isModifier ? "true" : "false");
    printf(">   keyDown: %s\n", rlEvent->keyDown ? "true" : "false");
    printf("> }\n");
}

void setEscapeCode(int code)
{
    ESCAPE = code;
}

static bool isModDown(uint64_t flagMask)
{
    return flagMask != 0 ? true : false;
}