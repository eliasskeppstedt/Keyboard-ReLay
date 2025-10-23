#include "./../header/converter.h"
#include <IOKit/hidsystem/IOLLEvent.h>

int setCodeFromMac(int macCode, int* rlCode, int* osToRL)
{
    int code = osToRL[macCode];
    *rlCode = code; // OStoRL will contain mac and rl codes if run on mac machine when multi OS support is implemented
    if (code == NO_VALUE) 
    {
        printf("  no relay key found\n");
        return 1;
    }
    // printf("DEBUG osToRLLookUp[osCode] = rlCode\n");
    // printf("DEBUG                      ^%d       ^%d\n\n", macCode, osToRL[macCode]);
    return 0;
}   

void setCodeToMac(int rlCode, int* macCode, int* rlToOs)
{
    *macCode = rlToOs[rlCode];
}

void setFlagsFromMac(uint64_t macMask, uint64_t* rlMask)
{
    *rlMask = 0;
    macHelper(macMask, NX_ALPHASHIFTMASK,  0,                      0,                      RL_CAPS_LOCK_MASK,    0,                   0,                   rlMask);
    macHelper(macMask, NX_SHIFTMASK,       NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, RL_SHIFT_MASK,        RL_L_SHIFT_MASK,     RL_R_SHIFT_MASK,     rlMask);
    macHelper(macMask, NX_CONTROLMASK,     NX_DEVICELCTLKEYMASK,   NX_DEVICERCTLKEYMASK,   RL_CONTROL_MASK,      RL_L_CONTROL_MASK,   RL_R_CONTROL_MASK,   rlMask);
    macHelper(macMask, NX_ALTERNATEMASK,   NX_DEVICELALTKEYMASK,   NX_DEVICERALTKEYMASK,   RL_ALTERNATE_MASK,    RL_L_ALTARNATE_MASK, RL_R_ALTARNATE_MASK, rlMask);
    macHelper(macMask, NX_COMMANDMASK,     NX_DEVICELCMDKEYMASK,   NX_DEVICERCMDKEYMASK,   RL_COMMAND_MASK,      RL_L_COMMAND_MASK,   RL_R_COMMAND_MASK,   rlMask);
    macHelper(macMask, NX_HELPMASK,        0,                      0,                      RL_HELP_MASK,         0,                   0,                   rlMask);
    macHelper(macMask, NX_SECONDARYFNMASK, 0,                      0,                      RL_SECONDARY_FN_MASK, 0,                   0,                   rlMask);
    macHelper(macMask, NX_NUMERICPADMASK,  0,                      0,                      RL_NUMERIC_PAD_MASK,  0,                   0,                   rlMask);
}

void setFlagsToMac(uint64_t rlMask, uint64_t* macMask)
{
    *macMask = 0;
    macHelper(rlMask, RL_CAPS_LOCK_MASK,    0,                   0,                     NX_ALPHASHIFTMASK,  0,                      0,                      macMask);
    macHelper(rlMask, RL_SHIFT_MASK,        RL_L_SHIFT_MASK,     RL_R_SHIFT_MASK,       NX_SHIFTMASK,       NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, macMask);
    macHelper(rlMask, RL_CONTROL_MASK,      RL_L_CONTROL_MASK,   RL_R_CONTROL_MASK,     NX_CONTROLMASK,     NX_DEVICELCTLKEYMASK,   NX_DEVICERCTLKEYMASK,   macMask);
    macHelper(rlMask, RL_ALTERNATE_MASK,    RL_L_ALTARNATE_MASK, RL_R_ALTARNATE_MASK,   NX_ALTERNATEMASK,   NX_DEVICELALTKEYMASK,   NX_DEVICERALTKEYMASK,   macMask);
    macHelper(rlMask, RL_COMMAND_MASK,      RL_L_COMMAND_MASK,   RL_R_COMMAND_MASK,     NX_COMMANDMASK,     NX_DEVICELCMDKEYMASK,   NX_DEVICERCMDKEYMASK,   macMask);
    macHelper(rlMask, RL_HELP_MASK,         0,                   0,                     NX_HELPMASK,        0,                      0,                      macMask);
    macHelper(rlMask, RL_SECONDARY_FN_MASK, 0,                   0,                     NX_SECONDARYFNMASK, 0,                      0,                      macMask);
    macHelper(rlMask, RL_NUMERIC_PAD_MASK,  0,                   0,                     NX_NUMERICPADMASK,  0,                      0,                      macMask);
}

void macHelper(uint64_t oldMask, uint64_t oldModMask, uint64_t oldLModMask, uint64_t oldRModMask, uint64_t newModMask, uint64_t newLModMask, uint64_t newRModMask, uint64_t* macMask)
{
    if ((oldMask & oldModMask) == oldModMask)
    {
        *macMask |= newModMask;

        if ((oldMask & oldLModMask) == oldLModMask) *macMask |= newLModMask;

        if ((oldMask & oldRModMask) == oldRModMask) *macMask |= newRModMask;
    }
}