#include "./../header/converter.h"

void setCodeFromMac(int macCode, int* pWebCode, int* pOSToWeb)
{
    *pWebCode = pOSToWeb[macCode]; // OStoWeb will contain mac and web codes if run on mac machine when multi OS support is implemented
}   

void setCodeToMac(int webCode, int* pMacCode, int* pWebToOs)
{
    *pMacCode = pWebToOs[webCode];
}

void setFlagsFromMac(int macMask, int* pWebMask)
{
    *pWebMask = 0;
    macHelper(macMask, NX_ALPHASHIFTMASK, 0, 0, OS_CAPS_LOCK_MASK, 0, 0, pWebMask);
    macHelper(macMask, NX_SHIFTMASK, NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, OS_SHIFT_MASK, OS_L_SHIFT_MASK, OS_R_SHIFT_MASK, pWebMask);
    macHelper(macMask, NX_CONTROLMASK, NX_DEVICELCTLKEYMASK, NX_DEVICERCTLKEYMASK, OS_CONTROL_MASK, OS_L_CONTROL_MASK, OS_R_CONTROL_MASK, pWebMask);
    macHelper(macMask, NX_ALTERNATEMASK, NX_DEVICELALTKEYMASK, NX_DEVICERALTKEYMASK, OS_ALTERNATE_MASK, OS_L_ALTARNATE_MASK, OS_R_ALTARNATE_MASK, pWebMask);
    macHelper(macMask, NX_COMMANDMASK, NX_DEVICELCMDKEYMASK, NX_DEVICERCMDKEYMASK, OS_COMMAND_MASK, OS_L_COMMAND_MASK, OS_R_COMMAND_MASK, pWebMask);
    macHelper(macMask, NX_HELPMASK, 0, 0, OS_HELP_MASK, 0, 0, pWebMask);
    macHelper(macMask, NX_SECONDARYFNMASK, 0, 0, OS_SECONDARY_FN_MASK, 0, 0, pWebMask);
    macHelper(macMask, NX_NUMERICPADMASK, 0, 0, OS_NUMERIC_PAD_MASK, 0, 0, pWebMask);
}

void setFlagsToMac(int webMask, int* pMacMask)
{
    *pMacMask = 0;
    macHelper(webMask, OS_SHIFT_MASK, OS_L_SHIFT_MASK, OS_R_SHIFT_MASK, NX_SHIFTMASK, NX_DEVICELSHIFTKEYMASK, NX_DEVICERSHIFTKEYMASK, pMacMask);
}

void macHelper(int oldMask, int oldModMask, int oldLModMask, int oldRModMask, int newModMask, int newLModMask, int newRModMask, int* pWebMask)
{
    if ((oldMask & oldModMask) == oldModMask)
    {
        if ((oldMask & oldLModMask) == oldLModMask)
        {
            *pWebMask |= newLModMask;
        }
        else if ((oldMask & oldRModMask) == oldRModMask)
        {
            *pWebMask |= newRModMask;
        }
        *pWebMask |= newModMask;
    }
}