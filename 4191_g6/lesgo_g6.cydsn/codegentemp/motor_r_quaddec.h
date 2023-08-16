/*******************************************************************************
* File Name: motor_r_quaddec.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_QUADRATURE_DECODER_motor_r_quaddec_H)
#define CY_QUADRATURE_DECODER_motor_r_quaddec_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define motor_r_quaddec_COUNTER_SIZE               (32u)
#define motor_r_quaddec_COUNTER_SIZE_8_BIT         (8u)
#define motor_r_quaddec_COUNTER_SIZE_16_BIT        (16u)
#define motor_r_quaddec_COUNTER_SIZE_32_BIT        (32u)

#if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT)
    #include "motor_r_quaddec_Cnt8.h"
#else 
    /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_16_BIT) || 
    *  (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT) 
    */
    #include "motor_r_quaddec_Cnt16.h"
#endif /* motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT */

extern uint8 motor_r_quaddec_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define motor_r_quaddec_COUNTER_RESOLUTION         (4u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} motor_r_quaddec_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  motor_r_quaddec_Init(void) ;
void  motor_r_quaddec_Start(void) ;
void  motor_r_quaddec_Stop(void) ;
void  motor_r_quaddec_Enable(void) ;
uint8 motor_r_quaddec_GetEvents(void) ;
void  motor_r_quaddec_SetInterruptMask(uint8 mask) ;
uint8 motor_r_quaddec_GetInterruptMask(void) ;
int32 motor_r_quaddec_GetCounter(void) ;
void  motor_r_quaddec_SetCounter(int32 value)
;
void  motor_r_quaddec_Sleep(void) ;
void  motor_r_quaddec_Wakeup(void) ;
void  motor_r_quaddec_SaveConfig(void) ;
void  motor_r_quaddec_RestoreConfig(void) ;

#if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(motor_r_quaddec_ISR);
#endif /* motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT)
    #define motor_r_quaddec_ISR_NUMBER             ((uint8) motor_r_quaddec_isr__INTC_NUMBER)
    #define motor_r_quaddec_ISR_PRIORITY           ((uint8) motor_r_quaddec_isr__INTC_PRIOR_NUM)
#endif /* motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define motor_r_quaddec_GLITCH_FILTERING           (1u)
#define motor_r_quaddec_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT)
    #define motor_r_quaddec_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_16_BIT) ||
    *  (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT)
    */
    #define motor_r_quaddec_COUNTER_INIT_VALUE    (0x8000u)
    #define motor_r_quaddec_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define motor_r_quaddec_STATUS_REG                 (* (reg8 *) motor_r_quaddec_bQuadDec_Stsreg__STATUS_REG)
#define motor_r_quaddec_STATUS_PTR                 (  (reg8 *) motor_r_quaddec_bQuadDec_Stsreg__STATUS_REG)
#define motor_r_quaddec_STATUS_MASK                (* (reg8 *) motor_r_quaddec_bQuadDec_Stsreg__MASK_REG)
#define motor_r_quaddec_STATUS_MASK_PTR            (  (reg8 *) motor_r_quaddec_bQuadDec_Stsreg__MASK_REG)
#define motor_r_quaddec_SR_AUX_CONTROL             (* (reg8 *) motor_r_quaddec_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define motor_r_quaddec_SR_AUX_CONTROL_PTR         (  (reg8 *) motor_r_quaddec_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define motor_r_quaddec_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define motor_r_quaddec_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define motor_r_quaddec_COUNTER_RESET_SHIFT        (0x02u)
#define motor_r_quaddec_INVALID_IN_SHIFT           (0x03u)
#define motor_r_quaddec_COUNTER_OVERFLOW           ((uint8) (0x01u << motor_r_quaddec_COUNTER_OVERFLOW_SHIFT))
#define motor_r_quaddec_COUNTER_UNDERFLOW          ((uint8) (0x01u << motor_r_quaddec_COUNTER_UNDERFLOW_SHIFT))
#define motor_r_quaddec_COUNTER_RESET              ((uint8) (0x01u << motor_r_quaddec_COUNTER_RESET_SHIFT))
#define motor_r_quaddec_INVALID_IN                 ((uint8) (0x01u << motor_r_quaddec_INVALID_IN_SHIFT))

#define motor_r_quaddec_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define motor_r_quaddec_INTERRUPTS_ENABLE          ((uint8)(0x01u << motor_r_quaddec_INTERRUPTS_ENABLE_SHIFT))
#define motor_r_quaddec_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define motor_r_quaddec_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_motor_r_quaddec_H */


/* [] END OF FILE */
