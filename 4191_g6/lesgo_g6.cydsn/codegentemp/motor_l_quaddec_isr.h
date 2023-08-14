/*******************************************************************************
* File Name: motor_l_quaddec_isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_motor_l_quaddec_isr_H)
#define CY_ISR_motor_l_quaddec_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void motor_l_quaddec_isr_Start(void);
void motor_l_quaddec_isr_StartEx(cyisraddress address);
void motor_l_quaddec_isr_Stop(void);

CY_ISR_PROTO(motor_l_quaddec_isr_Interrupt);

void motor_l_quaddec_isr_SetVector(cyisraddress address);
cyisraddress motor_l_quaddec_isr_GetVector(void);

void motor_l_quaddec_isr_SetPriority(uint8 priority);
uint8 motor_l_quaddec_isr_GetPriority(void);

void motor_l_quaddec_isr_Enable(void);
uint8 motor_l_quaddec_isr_GetState(void);
void motor_l_quaddec_isr_Disable(void);

void motor_l_quaddec_isr_SetPending(void);
void motor_l_quaddec_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the motor_l_quaddec_isr ISR. */
#define motor_l_quaddec_isr_INTC_VECTOR            ((reg32 *) motor_l_quaddec_isr__INTC_VECT)

/* Address of the motor_l_quaddec_isr ISR priority. */
#define motor_l_quaddec_isr_INTC_PRIOR             ((reg8 *) motor_l_quaddec_isr__INTC_PRIOR_REG)

/* Priority of the motor_l_quaddec_isr interrupt. */
#define motor_l_quaddec_isr_INTC_PRIOR_NUMBER      motor_l_quaddec_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable motor_l_quaddec_isr interrupt. */
#define motor_l_quaddec_isr_INTC_SET_EN            ((reg32 *) motor_l_quaddec_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the motor_l_quaddec_isr interrupt. */
#define motor_l_quaddec_isr_INTC_CLR_EN            ((reg32 *) motor_l_quaddec_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the motor_l_quaddec_isr interrupt state to pending. */
#define motor_l_quaddec_isr_INTC_SET_PD            ((reg32 *) motor_l_quaddec_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the motor_l_quaddec_isr interrupt. */
#define motor_l_quaddec_isr_INTC_CLR_PD            ((reg32 *) motor_l_quaddec_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_motor_l_quaddec_isr_H */


/* [] END OF FILE */
