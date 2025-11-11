#include "../header/relayEventHandler.h"
#include <stdint.h>

/* @attention relayEventHandler.c */
static ModKeys MOD_KEYS;
static uint64_t activeEventFlags = 0;
static int ESCAPE = 53;

/* if the event flag is not 0 then the mod flag is on, must be preceded by setCorrespondingModifierFlag */
static bool isModDown(uint64_t flagMask);

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
        bool shiftDown = incomingEvent->flagMask & MOD_KEYS.SHIFT_MASK;

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
    
    RLEvent* headEventPreview = NULL;
    RLEvent* outgoingEvent = NULL;
    int code = NO_VALUE;
    KeyInfo* keyInfo = NULL;

    if (incomingEventCode == NO_VALUE) // from timer (e.g. incoming event is not a "valid" rl event)
    {
        free(incomingEvent);

        headEventPreview = getEvent(HEAD);
        code = headEventPreview->code;
        keyInfo = getKeyInfo(code);
        
        headEventPreview->code = keyInfo->codeOnHold;
        headEventPreview->state = SEND;
        goto checkSetEvent;
    }

    enqueue(incomingEvent);
    headEventPreview = getEvent(HEAD);

    if (!getKeyInfo(incomingEventCode)) // if incoming event does not have any alternation, just skip to post
    {
        incomingEvent->state = SEND;
        goto postEvent;
    } 
    else 
    {
        if (incomingEvent->keyDown && getKeyInfo(incomingEventCode)->codeOnHold != NO_VALUE)
        {
            incomingEvent->state = PENDING;
            startOnHoldTimer(&incomingEvent->timer);
        }
        else 
        {
            incomingEvent->state = SEND;
        }
    }

    while (headEventPreview)
    {
        code = headEventPreview->code;
        KeyInfo* keyInfo = getKeyInfo(code);

        if (!keyInfo) goto postEvent; // no remap table entry available

        keyInfo = getKeyInfo(code);
        const int codeOnPress = keyInfo->codeOnPress;
        
        if (headEventPreview->state == PENDING) 
        {        
            if (code == incomingEventCode && !incomingEvent->keyDown) // i.e. if it is a key down/key up pair (does not work if you would press on multiple native mod key of the same key at least)
            {
                if (codeOnPress != NO_VALUE)
                {
                    headEventPreview->code = codeOnPress;
                }
                headEventPreview->state = SEND;

                if (headEventPreview->timer) 
                {
                    invalidateTimer(&headEventPreview->timer); // should always exist a timer on this event tho
                }
                else 
                {
                    printf("                          timer did not exist o.o???\n");
                }
            }
            else 
            {
                goto postEvent;
            }
        }
        else
        {
            if (codeOnPress != NO_VALUE)
            {
                headEventPreview->state = codeOnPress;
            }
            headEventPreview->state = SEND;
            getKeyStatus(code)->keyDown = true;
            getKeyStatus(code)->keysDown++;
        }

        /* code is from this point the correct one for head event and will NOT change */
        checkSetEvent:

        if (!headEventPreview->keyDown && keyInfo->codeOnHold != NO_VALUE)
        {
            if (!keyInfo) goto postEvent;
            if (getKeyStatus(code)->keyDown)
            {
                headEventPreview->code = keyInfo->codeOnHold;
                getKeyStatus(code)->keyDown = false;
                getKeyStatus(code)->keysDown--;
            }
        }

        uint64_t flagMask = 0;
        setCorrespondingModifierFlag(headEventPreview->code, &flagMask);
        printf("code: %d, modflag: %llu\n", headEventPreview->code, flagMask);
        if (flagMask && headEventPreview->state == SEND)
        {
            headEventPreview->isModifier = true;
            
            if (headEventPreview->keyDown) // add mod flag
            {
                activeEventFlags |= flagMask;
            }
            else // remove mod flag
            {
                activeEventFlags &= ~flagMask;
            }
        } 
        
        postEvent:
        printRLEvent(headEventPreview);

        if (headEventPreview->state == PENDING) break;
        outgoingEvent = dequeue();
        outgoingEvent->flagMask |= activeEventFlags;
        postEvent(outgoingEvent, SIMULATED_EVENT);
        free(outgoingEvent);
        headEventPreview = getEvent(HEAD);
    }
}

void setCorrespondingModifierFlag(int code, uint64_t* flagMask)
{
    *flagMask = 0;

    if (code == MOD_KEYS.L_SHIFT_CODE)
    {
        *flagMask = MOD_KEYS.SHIFT_MASK | MOD_KEYS.L_SHIFT_MASK;
    }
    else if (code == MOD_KEYS.R_SHIFT_CODE) 
    {
        *flagMask = MOD_KEYS.SHIFT_MASK | MOD_KEYS.R_SHIFT_MASK;
    }
    else if (code == MOD_KEYS.L_CONTROL_CODE) 
    {
        *flagMask = MOD_KEYS.CONTROL_MASK | MOD_KEYS.L_CONTROL_MASK;
    }
    else if (code == MOD_KEYS.R_CONTROL_CODE) 
    {
        *flagMask = MOD_KEYS.CONTROL_MASK | MOD_KEYS.R_CONTROL_MASK;
    }
    else if (code == MOD_KEYS.L_META_CODE) 
    {
        *flagMask = MOD_KEYS.META_MASK | MOD_KEYS.L_META_MASK;
    }
    else if (code == MOD_KEYS.R_META_CODE) 
    {
        *flagMask = MOD_KEYS.META_MASK | MOD_KEYS.R_META_MASK;
    }
    else if (code == MOD_KEYS.L_ALTERNATE_CODE) 
    {
        *flagMask = MOD_KEYS.ALTERNATE_MASK | MOD_KEYS.L_ALTERNATE_MASK;
    }
    else if (code == MOD_KEYS.R_ALTERNATE_CODE) 
    {
        *flagMask = MOD_KEYS.ALTERNATE_MASK | MOD_KEYS.R_ALTERNATE_MASK;
    }
    else if (code == MOD_KEYS.CAPS_LOCK_CODE) 
    {
        *flagMask = MOD_KEYS.CAPS_LOCK_MASK;
    }
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