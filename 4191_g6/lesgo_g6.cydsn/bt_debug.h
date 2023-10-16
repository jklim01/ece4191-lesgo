/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef BT_DEBUG_H
#define BT_DEBUG_H

#include <stdbool.h>
#include "cytypes.h"


// globals
extern bool bt_dbg_puck_found;


// ISRs
CY_ISR_PROTO(bt_dbg_ir_handler);


// API
int bt_dbg(void);


#endif

/* [] END OF FILE */
