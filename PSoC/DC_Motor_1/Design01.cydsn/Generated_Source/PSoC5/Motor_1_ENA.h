/*******************************************************************************
* File Name: Motor_1_ENA.h  
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

#if !defined(CY_PINS_Motor_1_ENA_H) /* Pins Motor_1_ENA_H */
#define CY_PINS_Motor_1_ENA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Motor_1_ENA_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Motor_1_ENA__PORT == 15 && ((Motor_1_ENA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Motor_1_ENA_Write(uint8 value);
void    Motor_1_ENA_SetDriveMode(uint8 mode);
uint8   Motor_1_ENA_ReadDataReg(void);
uint8   Motor_1_ENA_Read(void);
void    Motor_1_ENA_SetInterruptMode(uint16 position, uint16 mode);
uint8   Motor_1_ENA_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Motor_1_ENA_SetDriveMode() function.
     *  @{
     */
        #define Motor_1_ENA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Motor_1_ENA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Motor_1_ENA_DM_RES_UP          PIN_DM_RES_UP
        #define Motor_1_ENA_DM_RES_DWN         PIN_DM_RES_DWN
        #define Motor_1_ENA_DM_OD_LO           PIN_DM_OD_LO
        #define Motor_1_ENA_DM_OD_HI           PIN_DM_OD_HI
        #define Motor_1_ENA_DM_STRONG          PIN_DM_STRONG
        #define Motor_1_ENA_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Motor_1_ENA_MASK               Motor_1_ENA__MASK
#define Motor_1_ENA_SHIFT              Motor_1_ENA__SHIFT
#define Motor_1_ENA_WIDTH              1u

/* Interrupt constants */
#if defined(Motor_1_ENA__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Motor_1_ENA_SetInterruptMode() function.
     *  @{
     */
        #define Motor_1_ENA_INTR_NONE      (uint16)(0x0000u)
        #define Motor_1_ENA_INTR_RISING    (uint16)(0x0001u)
        #define Motor_1_ENA_INTR_FALLING   (uint16)(0x0002u)
        #define Motor_1_ENA_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Motor_1_ENA_INTR_MASK      (0x01u) 
#endif /* (Motor_1_ENA__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Motor_1_ENA_PS                     (* (reg8 *) Motor_1_ENA__PS)
/* Data Register */
#define Motor_1_ENA_DR                     (* (reg8 *) Motor_1_ENA__DR)
/* Port Number */
#define Motor_1_ENA_PRT_NUM                (* (reg8 *) Motor_1_ENA__PRT) 
/* Connect to Analog Globals */                                                  
#define Motor_1_ENA_AG                     (* (reg8 *) Motor_1_ENA__AG)                       
/* Analog MUX bux enable */
#define Motor_1_ENA_AMUX                   (* (reg8 *) Motor_1_ENA__AMUX) 
/* Bidirectional Enable */                                                        
#define Motor_1_ENA_BIE                    (* (reg8 *) Motor_1_ENA__BIE)
/* Bit-mask for Aliased Register Access */
#define Motor_1_ENA_BIT_MASK               (* (reg8 *) Motor_1_ENA__BIT_MASK)
/* Bypass Enable */
#define Motor_1_ENA_BYP                    (* (reg8 *) Motor_1_ENA__BYP)
/* Port wide control signals */                                                   
#define Motor_1_ENA_CTL                    (* (reg8 *) Motor_1_ENA__CTL)
/* Drive Modes */
#define Motor_1_ENA_DM0                    (* (reg8 *) Motor_1_ENA__DM0) 
#define Motor_1_ENA_DM1                    (* (reg8 *) Motor_1_ENA__DM1)
#define Motor_1_ENA_DM2                    (* (reg8 *) Motor_1_ENA__DM2) 
/* Input Buffer Disable Override */
#define Motor_1_ENA_INP_DIS                (* (reg8 *) Motor_1_ENA__INP_DIS)
/* LCD Common or Segment Drive */
#define Motor_1_ENA_LCD_COM_SEG            (* (reg8 *) Motor_1_ENA__LCD_COM_SEG)
/* Enable Segment LCD */
#define Motor_1_ENA_LCD_EN                 (* (reg8 *) Motor_1_ENA__LCD_EN)
/* Slew Rate Control */
#define Motor_1_ENA_SLW                    (* (reg8 *) Motor_1_ENA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Motor_1_ENA_PRTDSI__CAPS_SEL       (* (reg8 *) Motor_1_ENA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Motor_1_ENA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Motor_1_ENA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Motor_1_ENA_PRTDSI__OE_SEL0        (* (reg8 *) Motor_1_ENA__PRTDSI__OE_SEL0) 
#define Motor_1_ENA_PRTDSI__OE_SEL1        (* (reg8 *) Motor_1_ENA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Motor_1_ENA_PRTDSI__OUT_SEL0       (* (reg8 *) Motor_1_ENA__PRTDSI__OUT_SEL0) 
#define Motor_1_ENA_PRTDSI__OUT_SEL1       (* (reg8 *) Motor_1_ENA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Motor_1_ENA_PRTDSI__SYNC_OUT       (* (reg8 *) Motor_1_ENA__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Motor_1_ENA__SIO_CFG)
    #define Motor_1_ENA_SIO_HYST_EN        (* (reg8 *) Motor_1_ENA__SIO_HYST_EN)
    #define Motor_1_ENA_SIO_REG_HIFREQ     (* (reg8 *) Motor_1_ENA__SIO_REG_HIFREQ)
    #define Motor_1_ENA_SIO_CFG            (* (reg8 *) Motor_1_ENA__SIO_CFG)
    #define Motor_1_ENA_SIO_DIFF           (* (reg8 *) Motor_1_ENA__SIO_DIFF)
#endif /* (Motor_1_ENA__SIO_CFG) */

/* Interrupt Registers */
#if defined(Motor_1_ENA__INTSTAT)
    #define Motor_1_ENA_INTSTAT            (* (reg8 *) Motor_1_ENA__INTSTAT)
    #define Motor_1_ENA_SNAP               (* (reg8 *) Motor_1_ENA__SNAP)
    
	#define Motor_1_ENA_0_INTTYPE_REG 		(* (reg8 *) Motor_1_ENA__0__INTTYPE)
#endif /* (Motor_1_ENA__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Motor_1_ENA_H */


/* [] END OF FILE */
