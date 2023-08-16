/*******************************************************************************
* File Name: us_r_isr.h
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
#if !defined(CY_ISR_us_r_isr_H)
#define CY_ISR_us_r_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void us_r_isr_Start(void);
void us_r_isr_StartEx(cyisraddress address);
void us_r_isr_Stop(void);

CY_ISR_PROTO(us_r_isr_Interrupt);

void us_r_isr_SetVector(cyisraddress address);
cyisraddress us_r_isr_GetVector(void);

void us_r_isr_SetPriority(uint8 priority);
uint8 us_r_isr_GetPriority(void);

void us_r_isr_Enable(void);
uint8 us_r_isr_GetState(void);
void us_r_isr_Disable(void);

void us_r_isr_SetPending(void);
void us_r_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the us_r_isr ISR. */
#define us_r_isr_INTC_VECTOR            ((reg32 *) us_r_isr__INTC_VECT)

/* Address of the us_r_isr ISR priority. */
#define us_r_isr_INTC_PRIOR             ((reg8 *) us_r_isr__INTC_PRIOR_REG)

/* Priority of the us_r_isr interrupt. */
#define us_r_isr_INTC_PRIOR_NUMBER      us_r_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable us_r_isr interrupt. */
#define us_r_isr_INTC_SET_EN            ((reg32 *) us_r_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the us_r_isr interrupt. */
#define us_r_isr_INTC_CLR_EN            ((reg32 *) us_r_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the us_r_isr interrupt state to pending. */
#define us_r_isr_INTC_SET_PD            ((reg32 *) us_r_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the us_r_isr interrupt. */
#define us_r_isr_INTC_CLR_PD            ((reg32 *) us_r_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_us_r_isr_H */


/* [] END OF FILE */
