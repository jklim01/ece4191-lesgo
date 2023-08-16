/*******************************************************************************
* File Name: color_sensor_clk.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_color_sensor_clk_H)
#define CY_CLOCK_color_sensor_clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void color_sensor_clk_Start(void) ;
void color_sensor_clk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void color_sensor_clk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void color_sensor_clk_StandbyPower(uint8 state) ;
void color_sensor_clk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 color_sensor_clk_GetDividerRegister(void) ;
void color_sensor_clk_SetModeRegister(uint8 modeBitMask) ;
void color_sensor_clk_ClearModeRegister(uint8 modeBitMask) ;
uint8 color_sensor_clk_GetModeRegister(void) ;
void color_sensor_clk_SetSourceRegister(uint8 clkSource) ;
uint8 color_sensor_clk_GetSourceRegister(void) ;
#if defined(color_sensor_clk__CFG3)
void color_sensor_clk_SetPhaseRegister(uint8 clkPhase) ;
uint8 color_sensor_clk_GetPhaseRegister(void) ;
#endif /* defined(color_sensor_clk__CFG3) */

#define color_sensor_clk_Enable()                       color_sensor_clk_Start()
#define color_sensor_clk_Disable()                      color_sensor_clk_Stop()
#define color_sensor_clk_SetDivider(clkDivider)         color_sensor_clk_SetDividerRegister(clkDivider, 1u)
#define color_sensor_clk_SetDividerValue(clkDivider)    color_sensor_clk_SetDividerRegister((clkDivider) - 1u, 1u)
#define color_sensor_clk_SetMode(clkMode)               color_sensor_clk_SetModeRegister(clkMode)
#define color_sensor_clk_SetSource(clkSource)           color_sensor_clk_SetSourceRegister(clkSource)
#if defined(color_sensor_clk__CFG3)
#define color_sensor_clk_SetPhase(clkPhase)             color_sensor_clk_SetPhaseRegister(clkPhase)
#define color_sensor_clk_SetPhaseValue(clkPhase)        color_sensor_clk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(color_sensor_clk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define color_sensor_clk_CLKEN              (* (reg8 *) color_sensor_clk__PM_ACT_CFG)
#define color_sensor_clk_CLKEN_PTR          ((reg8 *) color_sensor_clk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define color_sensor_clk_CLKSTBY            (* (reg8 *) color_sensor_clk__PM_STBY_CFG)
#define color_sensor_clk_CLKSTBY_PTR        ((reg8 *) color_sensor_clk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define color_sensor_clk_DIV_LSB            (* (reg8 *) color_sensor_clk__CFG0)
#define color_sensor_clk_DIV_LSB_PTR        ((reg8 *) color_sensor_clk__CFG0)
#define color_sensor_clk_DIV_PTR            ((reg16 *) color_sensor_clk__CFG0)

/* Clock MSB divider configuration register. */
#define color_sensor_clk_DIV_MSB            (* (reg8 *) color_sensor_clk__CFG1)
#define color_sensor_clk_DIV_MSB_PTR        ((reg8 *) color_sensor_clk__CFG1)

/* Mode and source configuration register */
#define color_sensor_clk_MOD_SRC            (* (reg8 *) color_sensor_clk__CFG2)
#define color_sensor_clk_MOD_SRC_PTR        ((reg8 *) color_sensor_clk__CFG2)

#if defined(color_sensor_clk__CFG3)
/* Analog clock phase configuration register */
#define color_sensor_clk_PHASE              (* (reg8 *) color_sensor_clk__CFG3)
#define color_sensor_clk_PHASE_PTR          ((reg8 *) color_sensor_clk__CFG3)
#endif /* defined(color_sensor_clk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define color_sensor_clk_CLKEN_MASK         color_sensor_clk__PM_ACT_MSK
#define color_sensor_clk_CLKSTBY_MASK       color_sensor_clk__PM_STBY_MSK

/* CFG2 field masks */
#define color_sensor_clk_SRC_SEL_MSK        color_sensor_clk__CFG2_SRC_SEL_MASK
#define color_sensor_clk_MODE_MASK          (~(color_sensor_clk_SRC_SEL_MSK))

#if defined(color_sensor_clk__CFG3)
/* CFG3 phase mask */
#define color_sensor_clk_PHASE_MASK         color_sensor_clk__CFG3_PHASE_DLY_MASK
#endif /* defined(color_sensor_clk__CFG3) */

#endif /* CY_CLOCK_color_sensor_clk_H */


/* [] END OF FILE */
