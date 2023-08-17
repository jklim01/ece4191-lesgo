/*******************************************************************************
* File Name: us_trigger.h  
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

#if !defined(CY_PINS_us_trigger_H) /* Pins us_trigger_H */
#define CY_PINS_us_trigger_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "us_trigger_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 us_trigger__PORT == 15 && ((us_trigger__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    us_trigger_Write(uint8 value);
void    us_trigger_SetDriveMode(uint8 mode);
uint8   us_trigger_ReadDataReg(void);
uint8   us_trigger_Read(void);
void    us_trigger_SetInterruptMode(uint16 position, uint16 mode);
uint8   us_trigger_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the us_trigger_SetDriveMode() function.
     *  @{
     */
        #define us_trigger_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define us_trigger_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define us_trigger_DM_RES_UP          PIN_DM_RES_UP
        #define us_trigger_DM_RES_DWN         PIN_DM_RES_DWN
        #define us_trigger_DM_OD_LO           PIN_DM_OD_LO
        #define us_trigger_DM_OD_HI           PIN_DM_OD_HI
        #define us_trigger_DM_STRONG          PIN_DM_STRONG
        #define us_trigger_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define us_trigger_MASK               us_trigger__MASK
#define us_trigger_SHIFT              us_trigger__SHIFT
#define us_trigger_WIDTH              1u

/* Interrupt constants */
#if defined(us_trigger__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in us_trigger_SetInterruptMode() function.
     *  @{
     */
        #define us_trigger_INTR_NONE      (uint16)(0x0000u)
        #define us_trigger_INTR_RISING    (uint16)(0x0001u)
        #define us_trigger_INTR_FALLING   (uint16)(0x0002u)
        #define us_trigger_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define us_trigger_INTR_MASK      (0x01u) 
#endif /* (us_trigger__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define us_trigger_PS                     (* (reg8 *) us_trigger__PS)
/* Data Register */
#define us_trigger_DR                     (* (reg8 *) us_trigger__DR)
/* Port Number */
#define us_trigger_PRT_NUM                (* (reg8 *) us_trigger__PRT) 
/* Connect to Analog Globals */                                                  
#define us_trigger_AG                     (* (reg8 *) us_trigger__AG)                       
/* Analog MUX bux enable */
#define us_trigger_AMUX                   (* (reg8 *) us_trigger__AMUX) 
/* Bidirectional Enable */                                                        
#define us_trigger_BIE                    (* (reg8 *) us_trigger__BIE)
/* Bit-mask for Aliased Register Access */
#define us_trigger_BIT_MASK               (* (reg8 *) us_trigger__BIT_MASK)
/* Bypass Enable */
#define us_trigger_BYP                    (* (reg8 *) us_trigger__BYP)
/* Port wide control signals */                                                   
#define us_trigger_CTL                    (* (reg8 *) us_trigger__CTL)
/* Drive Modes */
#define us_trigger_DM0                    (* (reg8 *) us_trigger__DM0) 
#define us_trigger_DM1                    (* (reg8 *) us_trigger__DM1)
#define us_trigger_DM2                    (* (reg8 *) us_trigger__DM2) 
/* Input Buffer Disable Override */
#define us_trigger_INP_DIS                (* (reg8 *) us_trigger__INP_DIS)
/* LCD Common or Segment Drive */
#define us_trigger_LCD_COM_SEG            (* (reg8 *) us_trigger__LCD_COM_SEG)
/* Enable Segment LCD */
#define us_trigger_LCD_EN                 (* (reg8 *) us_trigger__LCD_EN)
/* Slew Rate Control */
#define us_trigger_SLW                    (* (reg8 *) us_trigger__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define us_trigger_PRTDSI__CAPS_SEL       (* (reg8 *) us_trigger__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define us_trigger_PRTDSI__DBL_SYNC_IN    (* (reg8 *) us_trigger__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define us_trigger_PRTDSI__OE_SEL0        (* (reg8 *) us_trigger__PRTDSI__OE_SEL0) 
#define us_trigger_PRTDSI__OE_SEL1        (* (reg8 *) us_trigger__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define us_trigger_PRTDSI__OUT_SEL0       (* (reg8 *) us_trigger__PRTDSI__OUT_SEL0) 
#define us_trigger_PRTDSI__OUT_SEL1       (* (reg8 *) us_trigger__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define us_trigger_PRTDSI__SYNC_OUT       (* (reg8 *) us_trigger__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(us_trigger__SIO_CFG)
    #define us_trigger_SIO_HYST_EN        (* (reg8 *) us_trigger__SIO_HYST_EN)
    #define us_trigger_SIO_REG_HIFREQ     (* (reg8 *) us_trigger__SIO_REG_HIFREQ)
    #define us_trigger_SIO_CFG            (* (reg8 *) us_trigger__SIO_CFG)
    #define us_trigger_SIO_DIFF           (* (reg8 *) us_trigger__SIO_DIFF)
#endif /* (us_trigger__SIO_CFG) */

/* Interrupt Registers */
#if defined(us_trigger__INTSTAT)
    #define us_trigger_INTSTAT            (* (reg8 *) us_trigger__INTSTAT)
    #define us_trigger_SNAP               (* (reg8 *) us_trigger__SNAP)
    
	#define us_trigger_0_INTTYPE_REG 		(* (reg8 *) us_trigger__0__INTTYPE)
#endif /* (us_trigger__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_us_trigger_H */


/* [] END OF FILE */
