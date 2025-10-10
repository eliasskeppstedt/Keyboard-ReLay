#include "./../header/converter.h"
#include <IOKit/hidsystem/IOLLEvent.h>

int setCodeFromMac(int macCode, int* krCode, int* osToKR)
{
    if (osToKR[macCode] == NO_VALUE) return 1;

    *krCode = osToKR[macCode]; // OStoKR will contain mac and kr codes if run on mac machine when multi OS support is implemented
    // printf("DEBUG osToKRLookUp[osCode] = krCode\n");
    // printf("DEBUG                      ^%d       ^%d\n\n", macCode, osToKR[macCode]);
    return 0;
}   

void setCodeToMac(int krCode, int* macCode, int* krToOs)
{
    *macCode = krToOs[krCode];
}

void setFlagsFromMac(uint64_t macMask, uint64_t* krMask)
{
    *krMask = 0;
    macHelper(macMask, NX_ALPHASHIFTMASK,  0,                      0,                      KR_CAPS_LOCK_MASK,    0,                   0,                   krMask);
    macHelper(macMask, NX_SHIFTMASK,       NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, KR_SHIFT_MASK,        KR_L_SHIFT_MASK,     KR_R_SHIFT_MASK,     krMask);
    macHelper(macMask, NX_CONTROLMASK,     NX_DEVICELCTLKEYMASK,   NX_DEVICERCTLKEYMASK,   KR_CONTROL_MASK,      KR_L_CONTROL_MASK,   KR_R_CONTROL_MASK,   krMask);
    macHelper(macMask, NX_ALTERNATEMASK,   NX_DEVICELALTKEYMASK,   NX_DEVICERALTKEYMASK,   KR_ALTERNATE_MASK,    KR_L_ALTARNATE_MASK, KR_R_ALTARNATE_MASK, krMask);
    macHelper(macMask, NX_COMMANDMASK,     NX_DEVICELCMDKEYMASK,   NX_DEVICERCMDKEYMASK,   KR_COMMAND_MASK,      KR_L_COMMAND_MASK,   KR_R_COMMAND_MASK,   krMask);
    macHelper(macMask, NX_HELPMASK,        0,                      0,                      KR_HELP_MASK,         0,                   0,                   krMask);
    macHelper(macMask, NX_SECONDARYFNMASK, 0,                      0,                      KR_SECONDARY_FN_MASK, 0,                   0,                   krMask);
    macHelper(macMask, NX_NUMERICPADMASK,  0,                      0,                      KR_NUMERIC_PAD_MASK,  0,                   0,                   krMask);
}

void setFlagsToMac(uint64_t krMask, uint64_t* macMask)
{
    *macMask = 0;
    macHelper(krMask, KR_CAPS_LOCK_MASK,    0,                   0,                     NX_ALPHASHIFTMASK,  0,                      0,                      macMask);
    macHelper(krMask, KR_SHIFT_MASK,        KR_L_SHIFT_MASK,     KR_R_SHIFT_MASK,       NX_SHIFTMASK,       NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, macMask);
    macHelper(krMask, KR_CONTROL_MASK,      KR_L_CONTROL_MASK,   KR_R_CONTROL_MASK,     NX_CONTROLMASK,     NX_DEVICELCTLKEYMASK,   NX_DEVICERCTLKEYMASK,   macMask);
    macHelper(krMask, KR_ALTERNATE_MASK,    KR_L_ALTARNATE_MASK, KR_R_ALTARNATE_MASK,   NX_ALTERNATEMASK,   NX_DEVICELALTKEYMASK,   NX_DEVICERALTKEYMASK,   macMask);
    macHelper(krMask, KR_COMMAND_MASK,      KR_L_COMMAND_MASK,   KR_R_COMMAND_MASK,     NX_COMMANDMASK,     NX_DEVICELCMDKEYMASK,   NX_DEVICERCMDKEYMASK,   macMask);
    macHelper(krMask, KR_HELP_MASK,         0,                   0,                     NX_HELPMASK,        0,                      0,                      macMask);
    macHelper(krMask, KR_SECONDARY_FN_MASK, 0,                   0,                     NX_SECONDARYFNMASK, 0,                      0,                      macMask);
    macHelper(krMask, KR_NUMERIC_PAD_MASK,  0,                   0,                     NX_NUMERICPADMASK,  0,                      0,                      macMask);
}

void macHelper(uint64_t oldMask, uint64_t oldModMask, uint64_t oldLModMask, uint64_t oldRModMask, uint64_t newModMask, uint64_t newLModMask, uint64_t newRModMask, uint64_t* macMask)
{
    if ((oldMask & oldModMask) == oldModMask)
    {
        if ((oldMask & oldLModMask) == oldLModMask) *macMask |= newLModMask;

        if ((oldMask & oldRModMask) == oldRModMask) *macMask |= newRModMask;

        *macMask |= newModMask;
    }
}