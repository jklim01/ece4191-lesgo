/*******************************************************************************
* File Name: p_controller_isr.h
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
#if !defined(CY_ISR_p_controller_isr_H)
#define CY_ISR_p_controller_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void p_controller_isr_Start(void);
void p_controller_isr_StartEx(cyisraddress address);
void p_controller_isr_Stop(void);

CY_ISR_PROTO(p_controller_isr_Interrupt);

void p_controller_isr_SetVector(cyisraddress address);
cyisraddress p_controller_isr_GetVector(void);

void p_controller_isr_SetPriority(uint8 priority);
uint8 p_controller_isr_GetPriority(void);

void p_controller_isr_Enable(void);
uint8 p_controller_isr_GetState(void);
void p_controller_isr_Disable(void);

void p_controller_isr_SetPending(void);
void p_controller_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the p_controller_isr ISR. */
#define p_controller_isr_INTC_VECTOR            ((reg32 *) p_controller_isr__INTC_VECT)

/* Address of the p_controller_isr ISR priority. */
#define p_controller_isr_INTC_PRIOR             ((reg8 *) p_controller_isr__INTC_PRIOR_REG)

/* Priority of the p_controller_isr interrupt. */
#define p_controller_isr_INTC_PRIOR_NUMBER      p_controller_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable p_controller_isr interrupt. */
#define p_controller_isr_INTC_SET_EN            ((reg32 *) p_controller_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the p_controller_isr interrupt. */
#define p_controller_isr_INTC_CLR_EN            ((reg32 *) p_controller_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the p_controller_isr interrupt state to pending. */
#define p_controller_isr_INTC_SET_PD            ((reg32 *) p_controller_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the p_controller_isr interrupt. */
#define p_controller_isr_INTC_CLR_PD            ((reg32 *) p_controller_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_p_controller_isr_H */


/* [] END OF FILE */
