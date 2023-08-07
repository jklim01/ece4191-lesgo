/*******************************************************************************
* File Name: motor_r_en.h  
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

#if !defined(CY_PINS_motor_r_en_H) /* Pins motor_r_en_H */
#define CY_PINS_motor_r_en_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "motor_r_en_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 motor_r_en__PORT == 15 && ((motor_r_en__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    motor_r_en_Write(uint8 value);
void    motor_r_en_SetDriveMode(uint8 mode);
uint8   motor_r_en_ReadDataReg(void);
uint8   motor_r_en_Read(void);
void    motor_r_en_SetInterruptMode(uint16 position, uint16 mode);
uint8   motor_r_en_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the motor_r_en_SetDriveMode() function.
     *  @{
     */
        #define motor_r_en_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define motor_r_en_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define motor_r_en_DM_RES_UP          PIN_DM_RES_UP
        #define motor_r_en_DM_RES_DWN         PIN_DM_RES_DWN
        #define motor_r_en_DM_OD_LO           PIN_DM_OD_LO
        #define motor_r_en_DM_OD_HI           PIN_DM_OD_HI
        #define motor_r_en_DM_STRONG          PIN_DM_STRONG
        #define motor_r_en_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define motor_r_en_MASK               motor_r_en__MASK
#define motor_r_en_SHIFT              motor_r_en__SHIFT
#define motor_r_en_WIDTH              1u

/* Interrupt constants */
#if defined(motor_r_en__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in motor_r_en_SetInterruptMode() function.
     *  @{
     */
        #define motor_r_en_INTR_NONE      (uint16)(0x0000u)
        #define motor_r_en_INTR_RISING    (uint16)(0x0001u)
        #define motor_r_en_INTR_FALLING   (uint16)(0x0002u)
        #define motor_r_en_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define motor_r_en_INTR_MASK      (0x01u) 
#endif /* (motor_r_en__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define motor_r_en_PS                     (* (reg8 *) motor_r_en__PS)
/* Data Register */
#define motor_r_en_DR                     (* (reg8 *) motor_r_en__DR)
/* Port Number */
#define motor_r_en_PRT_NUM                (* (reg8 *) motor_r_en__PRT) 
/* Connect to Analog Globals */                                                  
#define motor_r_en_AG                     (* (reg8 *) motor_r_en__AG)                       
/* Analog MUX bux enable */
#define motor_r_en_AMUX                   (* (reg8 *) motor_r_en__AMUX) 
/* Bidirectional Enable */                                                        
#define motor_r_en_BIE                    (* (reg8 *) motor_r_en__BIE)
/* Bit-mask for Aliased Register Access */
#define motor_r_en_BIT_MASK               (* (reg8 *) motor_r_en__BIT_MASK)
/* Bypass Enable */
#define motor_r_en_BYP                    (* (reg8 *) motor_r_en__BYP)
/* Port wide control signals */                                                   
#define motor_r_en_CTL                    (* (reg8 *) motor_r_en__CTL)
/* Drive Modes */
#define motor_r_en_DM0                    (* (reg8 *) motor_r_en__DM0) 
#define motor_r_en_DM1                    (* (reg8 *) motor_r_en__DM1)
#define motor_r_en_DM2                    (* (reg8 *) motor_r_en__DM2) 
/* Input Buffer Disable Override */
#define motor_r_en_INP_DIS                (* (reg8 *) motor_r_en__INP_DIS)
/* LCD Common or Segment Drive */
#define motor_r_en_LCD_COM_SEG            (* (reg8 *) motor_r_en__LCD_COM_SEG)
/* Enable Segment LCD */
#define motor_r_en_LCD_EN                 (* (reg8 *) motor_r_en__LCD_EN)
/* Slew Rate Control */
#define motor_r_en_SLW                    (* (reg8 *) motor_r_en__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define motor_r_en_PRTDSI__CAPS_SEL       (* (reg8 *) motor_r_en__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define motor_r_en_PRTDSI__DBL_SYNC_IN    (* (reg8 *) motor_r_en__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define motor_r_en_PRTDSI__OE_SEL0        (* (reg8 *) motor_r_en__PRTDSI__OE_SEL0) 
#define motor_r_en_PRTDSI__OE_SEL1        (* (reg8 *) motor_r_en__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define motor_r_en_PRTDSI__OUT_SEL0       (* (reg8 *) motor_r_en__PRTDSI__OUT_SEL0) 
#define motor_r_en_PRTDSI__OUT_SEL1       (* (reg8 *) motor_r_en__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define motor_r_en_PRTDSI__SYNC_OUT       (* (reg8 *) motor_r_en__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(motor_r_en__SIO_CFG)
    #define motor_r_en_SIO_HYST_EN        (* (reg8 *) motor_r_en__SIO_HYST_EN)
    #define motor_r_en_SIO_REG_HIFREQ     (* (reg8 *) motor_r_en__SIO_REG_HIFREQ)
    #define motor_r_en_SIO_CFG            (* (reg8 *) motor_r_en__SIO_CFG)
    #define motor_r_en_SIO_DIFF           (* (reg8 *) motor_r_en__SIO_DIFF)
#endif /* (motor_r_en__SIO_CFG) */

/* Interrupt Registers */
#if defined(motor_r_en__INTSTAT)
    #define motor_r_en_INTSTAT            (* (reg8 *) motor_r_en__INTSTAT)
    #define motor_r_en_SNAP               (* (reg8 *) motor_r_en__SNAP)
    
	#define motor_r_en_0_INTTYPE_REG 		(* (reg8 *) motor_r_en__0__INTTYPE)
#endif /* (motor_r_en__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_motor_r_en_H */


/* [] END OF FILE */
