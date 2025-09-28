#include "./../header/converter.h"

void setCodeFromMac(int macCode, int* pUniversalCode, int* pOSToUniversal)
{
    *pUniversalCode = pOSToUniversal[macCode]; // OStoUniversal will contain mac and universal codes if run on mac machine when multi OS support is implemented
    // printf("DEBUG pOSToUniversalLookUp[osCode] = universalCode\n");
    // printf("DEBUG                      ^%d       ^%d\nDEBUG\n", *pUniversalCode, pOSToUniversal[*pUniversalCode]);
}   

void setCodeToMac(int universalCode, int* pMacCode, int* pUniversalToOs)
{
    *pMacCode = pUniversalToOs[universalCode];
}

void setFlagsFromMac(int macMask, int* pUniversalMask)
{
    *pUniversalMask = 0;
    macHelper(macMask, NX_ALPHASHIFTMASK, 0, 0, OS_CAPS_LOCK_MASK, 0, 0, pUniversalMask);
    macHelper(macMask, NX_SHIFTMASK, NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, OS_SHIFT_MASK, OS_L_SHIFT_MASK, OS_R_SHIFT_MASK, pUniversalMask);
    macHelper(macMask, NX_CONTROLMASK, NX_DEVICELCTLKEYMASK, NX_DEVICERCTLKEYMASK, OS_CONTROL_MASK, OS_L_CONTROL_MASK, OS_R_CONTROL_MASK, pUniversalMask);
    macHelper(macMask, NX_ALTERNATEMASK, NX_DEVICELALTKEYMASK, NX_DEVICERALTKEYMASK, OS_ALTERNATE_MASK, OS_L_ALTARNATE_MASK, OS_R_ALTARNATE_MASK, pUniversalMask);
    macHelper(macMask, NX_COMMANDMASK, NX_DEVICELCMDKEYMASK, NX_DEVICERCMDKEYMASK, OS_COMMAND_MASK, OS_L_COMMAND_MASK, OS_R_COMMAND_MASK, pUniversalMask);
    macHelper(macMask, NX_HELPMASK, 0, 0, OS_HELP_MASK, 0, 0, pUniversalMask);
    macHelper(macMask, NX_SECONDARYFNMASK, 0, 0, OS_SECONDARY_FN_MASK, 0, 0, pUniversalMask);
    macHelper(macMask, NX_NUMERICPADMASK, 0, 0, OS_NUMERIC_PAD_MASK, 0, 0, pUniversalMask);
}

void setFlagsToMac(int universalMask, int* pMacMask)
{
    *pMacMask = 0;
    macHelper(universalMask, OS_SHIFT_MASK, OS_L_SHIFT_MASK, OS_R_SHIFT_MASK, NX_SHIFTMASK, NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, pMacMask);
}

void macHelper(int oldMask, int oldModMask, int oldLModMask, int oldRModMask, int newModMask, int newLModMask, int newRModMask, int* pUniversalMask)
{
    if ((oldMask & oldModMask) == oldModMask)
    {
        if ((oldMask & oldLModMask) == oldLModMask)
        {
            *pUniversalMask |= newLModMask;
        }
        else if ((oldMask & oldRModMask) == oldRModMask)
        {
            *pUniversalMask |= newRModMask;
        }
        *pUniversalMask |= newModMask;
    }
}