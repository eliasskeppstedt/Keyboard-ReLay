#ifndef _KEYCODECONVERTER_
#define _KEYCODECONVERTER_

#include <stdio.h>
#include <string.h>
#include "keyHandler.h"

#define MASK(x, y) (x & y)

#define	OS_CAPS_LOCK_MASK	    0x01
#define	OS_SHIFT_MASK		    0x02
#define	OS_CONTROL_MASK	    	0x03
#define	OS_ALTERNATE_MASK	    0x04
#define	OS_COMMAND_MASK		    0x05
#define	OS_HELP_MASK		    0x06
#define	OS_SECONDARY_FN_MASK    0x07
#define	OS_NUMERIC_PAD_MASK	    0x08

#define	OS_L_CONTROL_MASK	    0x10
#define OS_R_CONTROL_MASK   	0x20
#define	OS_L_SHIFT_MASK 	    0x30
#define	OS_R_SHIFT_MASK     	0x40
#define	OS_L_COMMAND_MASK   	0x50
#define	OS_R_COMMAND_MASK   	0x60
#define	OS_L_ALTARNATE_MASK 	0x70
#define	OS_R_ALTARNATE_MASK 	0x80

// mac converter stuff ////////////////////////////////////////////////

void setCodeFromMac(int macCode, int* pUniversalCode, int* pOSToUniversal);

/**
 * pUniversalToOs; pointer to universal to os lookup table
 */
void setCodeToMac(int universalCode, int* pMacCode, int* pUniversalToOs);

void setFlagsFromMac(int macMask, int* pUniversalMask);

void setFlagsToMac(int universalMask, int* pMacMask);

void macHelper(
    int oldMask, 
    int oldModMask, 
    int oldLModMask, 
    int oldRModMask, 
    int newModMask, 
    int newLModMask, 
    int newRModMask, 
    int* pUniversalMask
);

// from IOLLEvent.h, comments from CGEventTypes.h

/* Masks for the bits in event.flags */

/* device-independent */

#define	NX_ALPHASHIFTMASK	0x00010000
#define	NX_SHIFTMASK		0x00020000
#define	NX_CONTROLMASK		0x00040000
#define	NX_ALTERNATEMASK	0x00080000
#define	NX_COMMANDMASK		0x00100000

/* Special key identifiers. */

#define	NX_HELPMASK		    0x00400000
#define	NX_SECONDARYFNMASK	0x00800000

/* Identifies key events from numeric keypad area on extended keyboards. */

#define	NX_NUMERICPADMASK	0x00200000

/* device-dependent (really?) */

#define	NX_DEVICELCTLKEYMASK	0x00000001
#define	NX_DEVICELSHIFTKEYMASK	0x00000002
#define	NX_DEVICERSHIFTKEYMASK	0x00000004
#define	NX_DEVICELCMDKEYMASK	0x00000008
#define	NX_DEVICERCMDKEYMASK	0x00000010
#define	NX_DEVICELALTKEYMASK	0x00000020
#define	NX_DEVICERALTKEYMASK	0x00000040
#define NX_DEVICERCTLKEYMASK	0x00002000
// end from //

// end mac converter stuff //

#endif // _KEYCODECONVERTER_