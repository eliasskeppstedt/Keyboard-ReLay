#include "./../header/relayEventHandler.h"
#include "./../header/interface/relayEventHandler.h"

/* @attention relayEventHandler.c */
static ModKeys MOD_KEYS;
static uint64_t activeEventFlags = 0;
static uint64_t ESCAPE = 53;

/* if the event flag is not 0 then the mod flag is on, must be preceded by setCorrespondingModifierFlag */
static bool isModDown(uint64_t flagMask);
static void removeModFlags(uint64_t* flagMask);
static bool isModCode(uint64_t code);

int start()
{
    setModFlags(&MOD_KEYS); // uh naming is inconsistent, but this struct has mod key members and they are set by the os specific file in order to access their respective mod flags
    initRunLoop();
    return 0;
}

// defined in interfaces.h
void eventCallBack(RLEvent* incomingEvent)
{   
    const uint64_t incomingEventCode = incomingEvent->code;
    
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

    uint64_t code = NO_VALUE;
    RLEvent* headEventPreview = NULL;
    KeyInfo* keyInfo = NULL;
    KeyStatus* keyStatus = NULL;

    //// handle incoming event //// 

    
    if (incomingEventCode == NULL_EVENT) // none character from the web interface
    {
        free(incomingEvent);
        if (!(headEventPreview = getEvent(HEAD)) )
        {
            return;
        }
    }
    else if (incomingEventCode == ON_HOLD_TIMER_EVENT) // event sent by timer callback -> pending event should convert into its hold modification
    {
        free(incomingEvent);
        headEventPreview = getEvent(HEAD);
        headEventPreview->state = SEND;
        getKeyStatus(headEventPreview->code)->onHold = true;
    }
    else // normal incoming event
    {
        enqueue(incomingEvent);
        headEventPreview = getEvent(HEAD);

        if (!getKeyInfo(headEventPreview->code)) goto handleOutgoingEvent;

        if (getKeyInfo(incomingEventCode)->codeOnHold != NO_VALUE &&
            !getKeyStatus(incomingEventCode)->keyDown)
        {
            // if its a key press
            incomingEvent->state = PENDING;
            startOnHoldTimer(&incomingEvent->timer);
        }
    }
    
    handleOutgoingEvent:
    
    while (headEventPreview)
    {
        code = headEventPreview->code;
        keyInfo = getKeyInfo(code);
        keyStatus = getKeyStatus(code);

        if (!keyInfo) goto postEvent; // no remap table entry available

        if (headEventPreview->state == PENDING)
        {
            if (code == incomingEventCode && headEventPreview->timeStampOnPress != incomingEvent->timeStampOnPress)
            {
                if (headEventPreview->timer) 
                {
                    invalidateTimer(&headEventPreview->timer); // should always exist a timer on this event tho
                }
                else printf("timer did not exist o.o???\n");

                // is a key press/release of a key that was pending but never turned into its hold form,
                // hence invalidate timer and treat as normal event
                headEventPreview->state = SEND;
                keyStatus->onHold = false;
            }
            else
            {
                break; // if head event is still pending
            }
        }
        else
        {
            if (headEventPreview->state == NORMAL) // i had some thougth about this,,, come back to it later
            {
                headEventPreview->state = SEND;
            }
        }

        bool keyDown = !keyStatus->keyDown; // if key is down already, current event is a key release, else a key press
        keyStatus->keyDown = keyDown;
        headEventPreview->keyDown = keyDown;

        if (keyStatus->onHold) 
        {
            // event was turned into its hold modification
            headEventPreview->code = keyInfo->codeOnHold;
            if (!keyDown) keyStatus->onHold = false;
        }
        else if (keyInfo->codeOnPress != NO_VALUE) // not pending nor hold modification
        {
            headEventPreview->code = keyInfo->codeOnPress;
        }

        uint64_t flagMask = getCorrespondingModifierFlag(headEventPreview->code);

        if (keyDown) 
        {
            if (flagMask) activeEventFlags |= flagMask;
            keyStatus->keyDown = true;
        } 
        else // event must be release
        {
            if (flagMask) activeEventFlags &= ~flagMask;
            keyStatus->keyDown = false;
        }
        
        postEvent:

        removeModFlags(&headEventPreview->flagMask);   

        outgoingEvent = dequeue();

        outgoingEvent->flagMask |= activeEventFlags;

        postEvent(
            outgoingEvent->code, 
            outgoingEvent->keyDown, 
            outgoingEvent->flagMask, 
            outgoingEvent->timeStampOnPress, 
            SIMULATED_EVENT
        );

        free(outgoingEvent);
        headEventPreview = getEvent(HEAD);
    }
}

RLEvent* RLEventCreate(uint64_t code, bool keyDown, uint64_t flagMask, uint64_t timeStamp, bool isModifier, DataCodes userDefinedData)
{    
    if (userDefinedData == NULL_EVENT ||
        userDefinedData == ON_HOLD_TIMER_EVENT) 
    {
        code = userDefinedData;
    }

    RLEvent* rlEvent = malloc(sizeof(RLEvent));
    *rlEvent = (RLEvent) {
        .code = code,
        .flagMask = flagMask,
        .timeStampOnPress = timeStamp,
        .state = NORMAL,
        .isModifier = isModifier,
        .keyDown = keyDown,
        .timer = NULL
    };
    
    return rlEvent;
}

uint64_t getCorrespondingModifierFlag(uint64_t code)
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

uint64_t getCorrespondingModifierCode(uint64_t flagMask)
{
    uint64_t code = NO_VALUE;

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
    *flagMask |= 256; // seems like this is some kind of standard flag on all events (um at least on mac...)
}

static bool isModCode(uint64_t code)
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
    printf(">   code: %llu\n", rlEvent->code);
    printf(">   flagMask: %llu\n", rlEvent->flagMask);
    printf(">   timeStampOnPress: %llu\n", rlEvent->timeStampOnPress);
    char* state = "[state]";
    if (rlEvent->state == NORMAL) { state = "NORMAL"; }
    printf(">   state (SEND?): %s\n", rlEvent->state == SEND ? "true" : "false");
    printf(">   isModifier: %s\n", rlEvent->isModifier ? "true" : "false");
    printf(">   keyDown: %s\n", rlEvent->keyDown ? "true" : "false");
    printf("> }\n");
}

void setEscapeCode(uint64_t code)
{
    ESCAPE = code;
}

static bool isModDown(uint64_t flagMask)
{
    return flagMask != 0 ? true : false;
}