#ifndef _KEYCODECONVERTER_
#define _KEYCODECONVERTER_

#include <stdio.h>
#include <string.h>
#include "keyHandler.h"

#define MASK(x, y) (x & y)

/* device-independent */

#define	KR_CAPS_LOCK_MASK	    0x00010000ull
#define	KR_SHIFT_MASK		    0x00020000ull
#define	KR_CONTROL_MASK	    	0x00040000ull
#define	KR_ALTERNATE_MASK	    0x00080000ull
#define	KR_COMMAND_MASK		    0x00100000ull
#define	KR_NUMERIC_PAD_MASK	    0x00200000ull
#define	KR_HELP_MASK		    0x00400000ull
#define	KR_SECONDARY_FN_MASK    0x00800000ull

#define	KR_L_CONTROL_MASK	    0x00000001ull
#define KR_R_CONTROL_MASK   	0x00002000ull
#define	KR_L_SHIFT_MASK 	    0x00000002ull
#define	KR_R_SHIFT_MASK     	0x00000004ull
#define	KR_L_COMMAND_MASK   	0x00000008ull
#define	KR_R_COMMAND_MASK   	0x00000010ull
#define	KR_L_ALTARNATE_MASK 	0x00000020ull
#define	KR_R_ALTARNATE_MASK 	0x00000040ull

// mac converter stuff ////////////////////////////////////////////////

int setCodeFromMac(int macCode, int* krCode, int* osToKR);

/**
 * pKRToOs; pointer to kr to os lookup table
 */
void setCodeToMac(int krCode, int* macCode, int* krToOs);

void setFlagsFromMac(uint64_t macMask, uint64_t* krMask);

void setFlagsToMac(uint64_t krMask, uint64_t* macMask);

void macHelper(
    uint64_t oldMask, 
    uint64_t oldModMask, 
    uint64_t oldLModMask, 
    uint64_t oldRModMask, 
    uint64_t newModMask, 
    uint64_t newLModMask, 
    uint64_t newRModMask, 
    uint64_t* krMask
);

// from IOLLEvent.h, comments from CGEventTypes.h

/* Masks for the bits in event.flags */


// device-independent //
/*
#define	NX_ALPHASHIFTMASK	0x00010000ull
#define	NX_SHIFTMASK		0x00020000ull
#define	NX_CONTROLMASK		0x00040000ull
#define	NX_ALTERNATEMASK	0x00080000ull
#define	NX_COMMANDMASK		0x00100000ull
#define	NX_NUMERICPADMASK	0x00200000ull
#define	NX_HELPMASK		    0x00400000ull
#define	NX_SECONDARYFNMASK	0x00800000ull
//#define NX_ALPHASHIFT_STATELESS_MASK    0x01000000ull

// device-dependent (really?) //

#define	NX_DEVICELCTLKEYMASK	0x00000001ull
#define	NX_DEVICELSHIFTKEYMASK	0x00000002ull
#define	NX_DEVICERSHIFTKEYMASK	0x00000004ull
#define	NX_DEVICELCMDKEYMASK	0x00000008ull
#define	NX_DEVICERCMDKEYMASK	0x00000010ull
#define	NX_DEVICELALTKEYMASK	0x00000020ull
#define	NX_DEVICERALTKEYMASK	0x00000040ull
//#define NX_DEVICE_ALPHASHIFT_STATELESS_MASK 0x00000080ull
#define NX_DEVICERCTLKEYMASK	0x00002000ull
*/
// end from //

// end mac converter stuff //

#endif // _KEYCODECONVERTER_