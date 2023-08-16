/*******************************************************************************
* File Name: trigger.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_trigger_H) /* Pins trigger_H */
#define CY_PINS_trigger_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "trigger_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 trigger__PORT == 15 && ((trigger__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    trigger_Write(uint8 value);
void    trigger_SetDriveMode(uint8 mode);
uint8   trigger_ReadDataReg(void);
uint8   trigger_Read(void);
void    trigger_SetInterruptMode(uint16 position, uint16 mode);
uint8   trigger_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the trigger_SetDriveMode() function.
     *  @{
     */
        #define trigger_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define trigger_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define trigger_DM_RES_UP          PIN_DM_RES_UP
        #define trigger_DM_RES_DWN         PIN_DM_RES_DWN
        #define trigger_DM_OD_LO           PIN_DM_OD_LO
        #define trigger_DM_OD_HI           PIN_DM_OD_HI
        #define trigger_DM_STRONG          PIN_DM_STRONG
        #define trigger_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define trigger_MASK               trigger__MASK
#define trigger_SHIFT              trigger__SHIFT
#define trigger_WIDTH              1u

/* Interrupt constants */
#if defined(trigger__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in trigger_SetInterruptMode() function.
     *  @{
     */
        #define trigger_INTR_NONE      (uint16)(0x0000u)
        #define trigger_INTR_RISING    (uint16)(0x0001u)
        #define trigger_INTR_FALLING   (uint16)(0x0002u)
        #define trigger_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define trigger_INTR_MASK      (0x01u) 
#endif /* (trigger__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define trigger_PS                     (* (reg8 *) trigger__PS)
/* Data Register */
#define trigger_DR                     (* (reg8 *) trigger__DR)
/* Port Number */
#define trigger_PRT_NUM                (* (reg8 *) trigger__PRT) 
/* Connect to Analog Globals */                                                  
#define trigger_AG                     (* (reg8 *) trigger__AG)                       
/* Analog MUX bux enable */
#define trigger_AMUX                   (* (reg8 *) trigger__AMUX) 
/* Bidirectional Enable */                                                        
#define trigger_BIE                    (* (reg8 *) trigger__BIE)
/* Bit-mask for Aliased Register Access */
#define trigger_BIT_MASK               (* (reg8 *) trigger__BIT_MASK)
/* Bypass Enable */
#define trigger_BYP                    (* (reg8 *) trigger__BYP)
/* Port wide control signals */                                                   
#define trigger_CTL                    (* (reg8 *) trigger__CTL)
/* Drive Modes */
#define trigger_DM0                    (* (reg8 *) trigger__DM0) 
#define trigger_DM1                    (* (reg8 *) trigger__DM1)
#define trigger_DM2                    (* (reg8 *) trigger__DM2) 
/* Input Buffer Disable Override */
#define trigger_INP_DIS                (* (reg8 *) trigger__INP_DIS)
/* LCD Common or Segment Drive */
#define trigger_LCD_COM_SEG            (* (reg8 *) trigger__LCD_COM_SEG)
/* Enable Segment LCD */
#define trigger_LCD_EN                 (* (reg8 *) trigger__LCD_EN)
/* Slew Rate Control */
#define trigger_SLW                    (* (reg8 *) trigger__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define trigger_PRTDSI__CAPS_SEL       (* (reg8 *) trigger__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define trigger_PRTDSI__DBL_SYNC_IN    (* (reg8 *) trigger__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define trigger_PRTDSI__OE_SEL0        (* (reg8 *) trigger__PRTDSI__OE_SEL0) 
#define trigger_PRTDSI__OE_SEL1        (* (reg8 *) trigger__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define trigger_PRTDSI__OUT_SEL0       (* (reg8 *) trigger__PRTDSI__OUT_SEL0) 
#define trigger_PRTDSI__OUT_SEL1       (* (reg8 *) trigger__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define trigger_PRTDSI__SYNC_OUT       (* (reg8 *) trigger__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(trigger__SIO_CFG)
    #define trigger_SIO_HYST_EN        (* (reg8 *) trigger__SIO_HYST_EN)
    #define trigger_SIO_REG_HIFREQ     (* (reg8 *) trigger__SIO_REG_HIFREQ)
    #define trigger_SIO_CFG            (* (reg8 *) trigger__SIO_CFG)
    #define trigger_SIO_DIFF           (* (reg8 *) trigger__SIO_DIFF)
#endif /* (trigger__SIO_CFG) */

/* Interrupt Registers */
#if defined(trigger__INTSTAT)
    #define trigger_INTSTAT            (* (reg8 *) trigger__INTSTAT)
    #define trigger_SNAP               (* (reg8 *) trigger__SNAP)
    
	#define trigger_0_INTTYPE_REG 		(* (reg8 *) trigger__0__INTTYPE)
#endif /* (trigger__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_trigger_H */


/* [] END OF FILE */
