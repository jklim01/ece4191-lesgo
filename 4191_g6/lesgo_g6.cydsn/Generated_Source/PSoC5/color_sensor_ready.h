/*******************************************************************************
* File Name: color_sensor_ready.h
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
#if !defined(CY_ISR_color_sensor_ready_H)
#define CY_ISR_color_sensor_ready_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void color_sensor_ready_Start(void);
void color_sensor_ready_StartEx(cyisraddress address);
void color_sensor_ready_Stop(void);

CY_ISR_PROTO(color_sensor_ready_Interrupt);

void color_sensor_ready_SetVector(cyisraddress address);
cyisraddress color_sensor_ready_GetVector(void);

void color_sensor_ready_SetPriority(uint8 priority);
uint8 color_sensor_ready_GetPriority(void);

void color_sensor_ready_Enable(void);
uint8 color_sensor_ready_GetState(void);
void color_sensor_ready_Disable(void);

void color_sensor_ready_SetPending(void);
void color_sensor_ready_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the color_sensor_ready ISR. */
#define color_sensor_ready_INTC_VECTOR            ((reg32 *) color_sensor_ready__INTC_VECT)

/* Address of the color_sensor_ready ISR priority. */
#define color_sensor_ready_INTC_PRIOR             ((reg8 *) color_sensor_ready__INTC_PRIOR_REG)

/* Priority of the color_sensor_ready interrupt. */
#define color_sensor_ready_INTC_PRIOR_NUMBER      color_sensor_ready__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable color_sensor_ready interrupt. */
#define color_sensor_ready_INTC_SET_EN            ((reg32 *) color_sensor_ready__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the color_sensor_ready interrupt. */
#define color_sensor_ready_INTC_CLR_EN            ((reg32 *) color_sensor_ready__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the color_sensor_ready interrupt state to pending. */
#define color_sensor_ready_INTC_SET_PD            ((reg32 *) color_sensor_ready__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the color_sensor_ready interrupt. */
#define color_sensor_ready_INTC_CLR_PD            ((reg32 *) color_sensor_ready__INTC_CLR_PD_REG)


#endif /* CY_ISR_color_sensor_ready_H */


/* [] END OF FILE */
