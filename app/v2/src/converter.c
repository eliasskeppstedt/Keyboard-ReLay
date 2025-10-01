#include "./../header/converter.h"

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

void setFlagsFromMac(int macMask, int* krMask)
{
    *krMask = 0;
    macHelper(macMask, NX_ALPHASHIFTMASK, 0, 0, OS_CAPS_LOCK_MASK, 0, 0, krMask);
    macHelper(macMask, NX_SHIFTMASK, NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, OS_SHIFT_MASK, OS_L_SHIFT_MASK, OS_R_SHIFT_MASK, krMask);
    macHelper(macMask, NX_CONTROLMASK, NX_DEVICELCTLKEYMASK, NX_DEVICERCTLKEYMASK, OS_CONTROL_MASK, OS_L_CONTROL_MASK, OS_R_CONTROL_MASK, krMask);
    macHelper(macMask, NX_ALTERNATEMASK, NX_DEVICELALTKEYMASK, NX_DEVICERALTKEYMASK, OS_ALTERNATE_MASK, OS_L_ALTARNATE_MASK, OS_R_ALTARNATE_MASK, krMask);
    macHelper(macMask, NX_COMMANDMASK, NX_DEVICELCMDKEYMASK, NX_DEVICERCMDKEYMASK, OS_COMMAND_MASK, OS_L_COMMAND_MASK, OS_R_COMMAND_MASK, krMask);
    macHelper(macMask, NX_HELPMASK, 0, 0, OS_HELP_MASK, 0, 0, krMask);
    macHelper(macMask, NX_SECONDARYFNMASK, 0, 0, OS_SECONDARY_FN_MASK, 0, 0, krMask);
    macHelper(macMask, NX_NUMERICPADMASK, 0, 0, OS_NUMERIC_PAD_MASK, 0, 0, krMask);
}

void setFlagsToMac(int krMask, int* macMask)
{
    *macMask = 0;
    macHelper(krMask, OS_SHIFT_MASK, OS_L_SHIFT_MASK, OS_R_SHIFT_MASK, NX_SHIFTMASK, NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, macMask);
}

void macHelper(int oldMask, int oldModMask, int oldLModMask, int oldRModMask, int newModMask, int newLModMask, int newRModMask, int* macMask)
{
    if ((oldMask & oldModMask) == oldModMask)
    {
        if ((oldMask & oldLModMask) == oldLModMask)
        {
            *macMask |= newLModMask;
        }
        else if ((oldMask & oldRModMask) == oldRModMask)
        {
            *macMask |= newRModMask;
        }
        *macMask |= newModMask;
    }
}