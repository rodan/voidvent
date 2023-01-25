#ifndef __PROJ_H__
#define __PROJ_H__

#include <msp430.h>
#include <stdlib.h>
#include <inttypes.h>

#define true                1
#define false               0

/*!
	\brief List of possible message types for the event handler.
	\sa eh_register()
*/

#define                  SYS_MSG_NULL  0
#define           SYS_MSG_UART_BCL_RX  0x1   ///< UART received something
#define                 SYS_MSG_CC_RX  0x2   ///< tranceiver RX flag
#define                 SYS_MSG_CC_TX  0x4   ///< transceiver TX has finished 
#define             SYS_MSG_SCHEDULER  0x8   ///< scheduler has triggered
#define                SYS_MSG_PWR_SM  0x10  ///< power management state machine
#define         SYS_MSG_RF_DECODE_RDY  0x20  ///< result of the RF decoding is ready

#define               SCHEDULE_PWR_SM  SCH_SLOT_0
//#define                                SCH_SLOT_1
//#define                                SCH_SLOT_2
//#define                                SCH_SLOT_3
//#define                                SCH_SLOT_4
//#define                                SCH_SLOT_5
//#define                                SCH_SLOT_6
//#define                                SCH_SLOT_7
//#define                                SCH_SLOT_8


#endif
