/*******************************************************************************
* File Name: motor_l_pwm.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_motor_l_pwm_H)
#define CY_PWM_motor_l_pwm_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 motor_l_pwm_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define motor_l_pwm_Resolution                     (16u)
#define motor_l_pwm_UsingFixedFunction             (1u)
#define motor_l_pwm_DeadBandMode                   (0u)
#define motor_l_pwm_KillModeMinTime                (0u)
#define motor_l_pwm_KillMode                       (1u)
#define motor_l_pwm_PWMMode                        (0u)
#define motor_l_pwm_PWMModeIsCenterAligned         (0u)
#define motor_l_pwm_DeadBandUsed                   (0u)
#define motor_l_pwm_DeadBand2_4                    (0u)

#if !defined(motor_l_pwm_PWMUDB_genblk8_stsreg__REMOVED)
    #define motor_l_pwm_UseStatus                  (0u)
#else
    #define motor_l_pwm_UseStatus                  (0u)
#endif /* !defined(motor_l_pwm_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(motor_l_pwm_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define motor_l_pwm_UseControl                 (1u)
#else
    #define motor_l_pwm_UseControl                 (0u)
#endif /* !defined(motor_l_pwm_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define motor_l_pwm_UseOneCompareMode              (1u)
#define motor_l_pwm_MinimumKillTime                (1u)
#define motor_l_pwm_EnableMode                     (0u)

#define motor_l_pwm_CompareMode1SW                 (0u)
#define motor_l_pwm_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define motor_l_pwm__B_PWM__DISABLED 0
#define motor_l_pwm__B_PWM__ASYNCHRONOUS 1
#define motor_l_pwm__B_PWM__SINGLECYCLE 2
#define motor_l_pwm__B_PWM__LATCHED 3
#define motor_l_pwm__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define motor_l_pwm__B_PWM__DBMDISABLED 0
#define motor_l_pwm__B_PWM__DBM_2_4_CLOCKS 1
#define motor_l_pwm__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define motor_l_pwm__B_PWM__ONE_OUTPUT 0
#define motor_l_pwm__B_PWM__TWO_OUTPUTS 1
#define motor_l_pwm__B_PWM__DUAL_EDGE 2
#define motor_l_pwm__B_PWM__CENTER_ALIGN 3
#define motor_l_pwm__B_PWM__DITHER 5
#define motor_l_pwm__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define motor_l_pwm__B_PWM__LESS_THAN 1
#define motor_l_pwm__B_PWM__LESS_THAN_OR_EQUAL 2
#define motor_l_pwm__B_PWM__GREATER_THAN 3
#define motor_l_pwm__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define motor_l_pwm__B_PWM__EQUAL 0
#define motor_l_pwm__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!motor_l_pwm_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!motor_l_pwm_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!motor_l_pwm_PWMModeIsCenterAligned) */
        #if (motor_l_pwm_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (motor_l_pwm_UseStatus) */

        /* Backup for Deadband parameters */
        #if(motor_l_pwm_DeadBandMode == motor_l_pwm__B_PWM__DBM_256_CLOCKS || \
            motor_l_pwm_DeadBandMode == motor_l_pwm__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(motor_l_pwm_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (motor_l_pwm_KillModeMinTime) */

        /* Backup control register */
        #if(motor_l_pwm_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (motor_l_pwm_UseControl) */

    #endif /* (!motor_l_pwm_UsingFixedFunction) */

}motor_l_pwm_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    motor_l_pwm_Start(void) ;
void    motor_l_pwm_Stop(void) ;

#if (motor_l_pwm_UseStatus || motor_l_pwm_UsingFixedFunction)
    void  motor_l_pwm_SetInterruptMode(uint8 interruptMode) ;
    uint8 motor_l_pwm_ReadStatusRegister(void) ;
#endif /* (motor_l_pwm_UseStatus || motor_l_pwm_UsingFixedFunction) */

#define motor_l_pwm_GetInterruptSource() motor_l_pwm_ReadStatusRegister()

#if (motor_l_pwm_UseControl)
    uint8 motor_l_pwm_ReadControlRegister(void) ;
    void  motor_l_pwm_WriteControlRegister(uint8 control)
          ;
#endif /* (motor_l_pwm_UseControl) */

#if (motor_l_pwm_UseOneCompareMode)
   #if (motor_l_pwm_CompareMode1SW)
       void    motor_l_pwm_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (motor_l_pwm_CompareMode1SW) */
#else
    #if (motor_l_pwm_CompareMode1SW)
        void    motor_l_pwm_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (motor_l_pwm_CompareMode1SW) */
    #if (motor_l_pwm_CompareMode2SW)
        void    motor_l_pwm_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (motor_l_pwm_CompareMode2SW) */
#endif /* (motor_l_pwm_UseOneCompareMode) */

#if (!motor_l_pwm_UsingFixedFunction)
    uint16   motor_l_pwm_ReadCounter(void) ;
    uint16 motor_l_pwm_ReadCapture(void) ;

    #if (motor_l_pwm_UseStatus)
            void motor_l_pwm_ClearFIFO(void) ;
    #endif /* (motor_l_pwm_UseStatus) */

    void    motor_l_pwm_WriteCounter(uint16 counter)
            ;
#endif /* (!motor_l_pwm_UsingFixedFunction) */

void    motor_l_pwm_WritePeriod(uint16 period)
        ;
uint16 motor_l_pwm_ReadPeriod(void) ;

#if (motor_l_pwm_UseOneCompareMode)
    void    motor_l_pwm_WriteCompare(uint16 compare)
            ;
    uint16 motor_l_pwm_ReadCompare(void) ;
#else
    void    motor_l_pwm_WriteCompare1(uint16 compare)
            ;
    uint16 motor_l_pwm_ReadCompare1(void) ;
    void    motor_l_pwm_WriteCompare2(uint16 compare)
            ;
    uint16 motor_l_pwm_ReadCompare2(void) ;
#endif /* (motor_l_pwm_UseOneCompareMode) */


#if (motor_l_pwm_DeadBandUsed)
    void    motor_l_pwm_WriteDeadTime(uint8 deadtime) ;
    uint8   motor_l_pwm_ReadDeadTime(void) ;
#endif /* (motor_l_pwm_DeadBandUsed) */

#if ( motor_l_pwm_KillModeMinTime)
    void motor_l_pwm_WriteKillTime(uint8 killtime) ;
    uint8 motor_l_pwm_ReadKillTime(void) ;
#endif /* ( motor_l_pwm_KillModeMinTime) */

void motor_l_pwm_Init(void) ;
void motor_l_pwm_Enable(void) ;
void motor_l_pwm_Sleep(void) ;
void motor_l_pwm_Wakeup(void) ;
void motor_l_pwm_SaveConfig(void) ;
void motor_l_pwm_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define motor_l_pwm_INIT_PERIOD_VALUE          (24999u)
#define motor_l_pwm_INIT_COMPARE_VALUE1        (12500u)
#define motor_l_pwm_INIT_COMPARE_VALUE2        (63u)
#define motor_l_pwm_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    motor_l_pwm_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor_l_pwm_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor_l_pwm_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    motor_l_pwm_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define motor_l_pwm_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  motor_l_pwm_CTRL_CMPMODE2_SHIFT)
#define motor_l_pwm_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  motor_l_pwm_CTRL_CMPMODE1_SHIFT)
#define motor_l_pwm_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (motor_l_pwm_UsingFixedFunction)
   #define motor_l_pwm_PERIOD_LSB              (*(reg16 *) motor_l_pwm_PWMHW__PER0)
   #define motor_l_pwm_PERIOD_LSB_PTR          ( (reg16 *) motor_l_pwm_PWMHW__PER0)
   #define motor_l_pwm_COMPARE1_LSB            (*(reg16 *) motor_l_pwm_PWMHW__CNT_CMP0)
   #define motor_l_pwm_COMPARE1_LSB_PTR        ( (reg16 *) motor_l_pwm_PWMHW__CNT_CMP0)
   #define motor_l_pwm_COMPARE2_LSB            (0x00u)
   #define motor_l_pwm_COMPARE2_LSB_PTR        (0x00u)
   #define motor_l_pwm_COUNTER_LSB             (*(reg16 *) motor_l_pwm_PWMHW__CNT_CMP0)
   #define motor_l_pwm_COUNTER_LSB_PTR         ( (reg16 *) motor_l_pwm_PWMHW__CNT_CMP0)
   #define motor_l_pwm_CAPTURE_LSB             (*(reg16 *) motor_l_pwm_PWMHW__CAP0)
   #define motor_l_pwm_CAPTURE_LSB_PTR         ( (reg16 *) motor_l_pwm_PWMHW__CAP0)
   #define motor_l_pwm_RT1                     (*(reg8 *)  motor_l_pwm_PWMHW__RT1)
   #define motor_l_pwm_RT1_PTR                 ( (reg8 *)  motor_l_pwm_PWMHW__RT1)

#else
   #if (motor_l_pwm_Resolution == 8u) /* 8bit - PWM */

       #if(motor_l_pwm_PWMModeIsCenterAligned)
           #define motor_l_pwm_PERIOD_LSB      (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define motor_l_pwm_PERIOD_LSB_PTR  ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define motor_l_pwm_PERIOD_LSB      (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define motor_l_pwm_PERIOD_LSB_PTR  ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (motor_l_pwm_PWMModeIsCenterAligned) */

       #define motor_l_pwm_COMPARE1_LSB        (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define motor_l_pwm_COMPARE1_LSB_PTR    ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define motor_l_pwm_COMPARE2_LSB        (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define motor_l_pwm_COMPARE2_LSB_PTR    ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define motor_l_pwm_COUNTERCAP_LSB      (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define motor_l_pwm_COUNTERCAP_LSB_PTR  ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define motor_l_pwm_COUNTER_LSB         (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define motor_l_pwm_COUNTER_LSB_PTR     ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define motor_l_pwm_CAPTURE_LSB         (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define motor_l_pwm_CAPTURE_LSB_PTR     ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(motor_l_pwm_PWMModeIsCenterAligned)
               #define motor_l_pwm_PERIOD_LSB      (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define motor_l_pwm_PERIOD_LSB_PTR  ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define motor_l_pwm_PERIOD_LSB      (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define motor_l_pwm_PERIOD_LSB_PTR  ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (motor_l_pwm_PWMModeIsCenterAligned) */

            #define motor_l_pwm_COMPARE1_LSB       (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define motor_l_pwm_COMPARE1_LSB_PTR   ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define motor_l_pwm_COMPARE2_LSB       (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define motor_l_pwm_COMPARE2_LSB_PTR   ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define motor_l_pwm_COUNTERCAP_LSB     (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define motor_l_pwm_COUNTERCAP_LSB_PTR ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define motor_l_pwm_COUNTER_LSB        (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define motor_l_pwm_COUNTER_LSB_PTR    ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define motor_l_pwm_CAPTURE_LSB        (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define motor_l_pwm_CAPTURE_LSB_PTR    ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(motor_l_pwm_PWMModeIsCenterAligned)
               #define motor_l_pwm_PERIOD_LSB      (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define motor_l_pwm_PERIOD_LSB_PTR  ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define motor_l_pwm_PERIOD_LSB      (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define motor_l_pwm_PERIOD_LSB_PTR  ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (motor_l_pwm_PWMModeIsCenterAligned) */

            #define motor_l_pwm_COMPARE1_LSB       (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define motor_l_pwm_COMPARE1_LSB_PTR   ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define motor_l_pwm_COMPARE2_LSB       (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define motor_l_pwm_COMPARE2_LSB_PTR   ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define motor_l_pwm_COUNTERCAP_LSB     (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define motor_l_pwm_COUNTERCAP_LSB_PTR ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define motor_l_pwm_COUNTER_LSB        (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define motor_l_pwm_COUNTER_LSB_PTR    ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define motor_l_pwm_CAPTURE_LSB        (*(reg16 *) motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define motor_l_pwm_CAPTURE_LSB_PTR    ((reg16 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define motor_l_pwm_AUX_CONTROLDP1          (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define motor_l_pwm_AUX_CONTROLDP1_PTR      ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (motor_l_pwm_Resolution == 8) */

   #define motor_l_pwm_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define motor_l_pwm_AUX_CONTROLDP0          (*(reg8 *)  motor_l_pwm_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define motor_l_pwm_AUX_CONTROLDP0_PTR      ((reg8 *)   motor_l_pwm_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (motor_l_pwm_UsingFixedFunction) */

#if(motor_l_pwm_KillModeMinTime )
    #define motor_l_pwm_KILLMODEMINTIME        (*(reg8 *)  motor_l_pwm_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define motor_l_pwm_KILLMODEMINTIME_PTR    ((reg8 *)   motor_l_pwm_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (motor_l_pwm_KillModeMinTime ) */

#if(motor_l_pwm_DeadBandMode == motor_l_pwm__B_PWM__DBM_256_CLOCKS)
    #define motor_l_pwm_DEADBAND_COUNT         (*(reg8 *)  motor_l_pwm_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motor_l_pwm_DEADBAND_COUNT_PTR     ((reg8 *)   motor_l_pwm_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define motor_l_pwm_DEADBAND_LSB_PTR       ((reg8 *)   motor_l_pwm_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define motor_l_pwm_DEADBAND_LSB           (*(reg8 *)  motor_l_pwm_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(motor_l_pwm_DeadBandMode == motor_l_pwm__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (motor_l_pwm_UsingFixedFunction)
        #define motor_l_pwm_DEADBAND_COUNT         (*(reg8 *)  motor_l_pwm_PWMHW__CFG0)
        #define motor_l_pwm_DEADBAND_COUNT_PTR     ((reg8 *)   motor_l_pwm_PWMHW__CFG0)
        #define motor_l_pwm_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motor_l_pwm_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define motor_l_pwm_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define motor_l_pwm_DEADBAND_COUNT         (*(reg8 *)  motor_l_pwm_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motor_l_pwm_DEADBAND_COUNT_PTR     ((reg8 *)   motor_l_pwm_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define motor_l_pwm_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << motor_l_pwm_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define motor_l_pwm_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (motor_l_pwm_UsingFixedFunction) */
#endif /* (motor_l_pwm_DeadBandMode == motor_l_pwm__B_PWM__DBM_256_CLOCKS) */



#if (motor_l_pwm_UsingFixedFunction)
    #define motor_l_pwm_STATUS                 (*(reg8 *) motor_l_pwm_PWMHW__SR0)
    #define motor_l_pwm_STATUS_PTR             ((reg8 *) motor_l_pwm_PWMHW__SR0)
    #define motor_l_pwm_STATUS_MASK            (*(reg8 *) motor_l_pwm_PWMHW__SR0)
    #define motor_l_pwm_STATUS_MASK_PTR        ((reg8 *) motor_l_pwm_PWMHW__SR0)
    #define motor_l_pwm_CONTROL                (*(reg8 *) motor_l_pwm_PWMHW__CFG0)
    #define motor_l_pwm_CONTROL_PTR            ((reg8 *) motor_l_pwm_PWMHW__CFG0)
    #define motor_l_pwm_CONTROL2               (*(reg8 *) motor_l_pwm_PWMHW__CFG1)
    #define motor_l_pwm_CONTROL3               (*(reg8 *) motor_l_pwm_PWMHW__CFG2)
    #define motor_l_pwm_GLOBAL_ENABLE          (*(reg8 *) motor_l_pwm_PWMHW__PM_ACT_CFG)
    #define motor_l_pwm_GLOBAL_ENABLE_PTR      ( (reg8 *) motor_l_pwm_PWMHW__PM_ACT_CFG)
    #define motor_l_pwm_GLOBAL_STBY_ENABLE     (*(reg8 *) motor_l_pwm_PWMHW__PM_STBY_CFG)
    #define motor_l_pwm_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) motor_l_pwm_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define motor_l_pwm_BLOCK_EN_MASK          (motor_l_pwm_PWMHW__PM_ACT_MSK)
    #define motor_l_pwm_BLOCK_STBY_EN_MASK     (motor_l_pwm_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define motor_l_pwm_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define motor_l_pwm_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define motor_l_pwm_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define motor_l_pwm_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define motor_l_pwm_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define motor_l_pwm_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define motor_l_pwm_CTRL_ENABLE            (uint8)((uint8)0x01u << motor_l_pwm_CTRL_ENABLE_SHIFT)
    #define motor_l_pwm_CTRL_RESET             (uint8)((uint8)0x01u << motor_l_pwm_CTRL_RESET_SHIFT)
    #define motor_l_pwm_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motor_l_pwm_CTRL_CMPMODE2_SHIFT)
    #define motor_l_pwm_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motor_l_pwm_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define motor_l_pwm_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define motor_l_pwm_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << motor_l_pwm_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define motor_l_pwm_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define motor_l_pwm_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define motor_l_pwm_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define motor_l_pwm_STATUS_TC_INT_EN_MASK_SHIFT            (motor_l_pwm_STATUS_TC_SHIFT - 4u)
    #define motor_l_pwm_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define motor_l_pwm_STATUS_CMP1_INT_EN_MASK_SHIFT          (motor_l_pwm_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define motor_l_pwm_STATUS_TC              (uint8)((uint8)0x01u << motor_l_pwm_STATUS_TC_SHIFT)
    #define motor_l_pwm_STATUS_CMP1            (uint8)((uint8)0x01u << motor_l_pwm_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define motor_l_pwm_STATUS_TC_INT_EN_MASK              (uint8)((uint8)motor_l_pwm_STATUS_TC >> 4u)
    #define motor_l_pwm_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)motor_l_pwm_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define motor_l_pwm_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define motor_l_pwm_RT1_MASK              (uint8)((uint8)0x03u << motor_l_pwm_RT1_SHIFT)
    #define motor_l_pwm_SYNC                  (uint8)((uint8)0x03u << motor_l_pwm_RT1_SHIFT)
    #define motor_l_pwm_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define motor_l_pwm_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << motor_l_pwm_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define motor_l_pwm_SYNCDSI_EN            (uint8)((uint8)0x0Fu << motor_l_pwm_SYNCDSI_SHIFT)


#else
    #define motor_l_pwm_STATUS                (*(reg8 *)   motor_l_pwm_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motor_l_pwm_STATUS_PTR            ((reg8 *)    motor_l_pwm_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define motor_l_pwm_STATUS_MASK           (*(reg8 *)   motor_l_pwm_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motor_l_pwm_STATUS_MASK_PTR       ((reg8 *)    motor_l_pwm_PWMUDB_genblk8_stsreg__MASK_REG)
    #define motor_l_pwm_STATUS_AUX_CTRL       (*(reg8 *)   motor_l_pwm_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define motor_l_pwm_CONTROL               (*(reg8 *)   motor_l_pwm_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define motor_l_pwm_CONTROL_PTR           ((reg8 *)    motor_l_pwm_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define motor_l_pwm_CTRL_ENABLE_SHIFT      (0x07u)
    #define motor_l_pwm_CTRL_RESET_SHIFT       (0x06u)
    #define motor_l_pwm_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define motor_l_pwm_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define motor_l_pwm_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define motor_l_pwm_CTRL_ENABLE            (uint8)((uint8)0x01u << motor_l_pwm_CTRL_ENABLE_SHIFT)
    #define motor_l_pwm_CTRL_RESET             (uint8)((uint8)0x01u << motor_l_pwm_CTRL_RESET_SHIFT)
    #define motor_l_pwm_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << motor_l_pwm_CTRL_CMPMODE2_SHIFT)
    #define motor_l_pwm_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << motor_l_pwm_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define motor_l_pwm_STATUS_KILL_SHIFT          (0x05u)
    #define motor_l_pwm_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define motor_l_pwm_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define motor_l_pwm_STATUS_TC_SHIFT            (0x02u)
    #define motor_l_pwm_STATUS_CMP2_SHIFT          (0x01u)
    #define motor_l_pwm_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define motor_l_pwm_STATUS_KILL_INT_EN_MASK_SHIFT          (motor_l_pwm_STATUS_KILL_SHIFT)
    #define motor_l_pwm_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (motor_l_pwm_STATUS_FIFONEMPTY_SHIFT)
    #define motor_l_pwm_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (motor_l_pwm_STATUS_FIFOFULL_SHIFT)
    #define motor_l_pwm_STATUS_TC_INT_EN_MASK_SHIFT            (motor_l_pwm_STATUS_TC_SHIFT)
    #define motor_l_pwm_STATUS_CMP2_INT_EN_MASK_SHIFT          (motor_l_pwm_STATUS_CMP2_SHIFT)
    #define motor_l_pwm_STATUS_CMP1_INT_EN_MASK_SHIFT          (motor_l_pwm_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define motor_l_pwm_STATUS_KILL            (uint8)((uint8)0x00u << motor_l_pwm_STATUS_KILL_SHIFT )
    #define motor_l_pwm_STATUS_FIFOFULL        (uint8)((uint8)0x01u << motor_l_pwm_STATUS_FIFOFULL_SHIFT)
    #define motor_l_pwm_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << motor_l_pwm_STATUS_FIFONEMPTY_SHIFT)
    #define motor_l_pwm_STATUS_TC              (uint8)((uint8)0x01u << motor_l_pwm_STATUS_TC_SHIFT)
    #define motor_l_pwm_STATUS_CMP2            (uint8)((uint8)0x01u << motor_l_pwm_STATUS_CMP2_SHIFT)
    #define motor_l_pwm_STATUS_CMP1            (uint8)((uint8)0x01u << motor_l_pwm_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define motor_l_pwm_STATUS_KILL_INT_EN_MASK            (motor_l_pwm_STATUS_KILL)
    #define motor_l_pwm_STATUS_FIFOFULL_INT_EN_MASK        (motor_l_pwm_STATUS_FIFOFULL)
    #define motor_l_pwm_STATUS_FIFONEMPTY_INT_EN_MASK      (motor_l_pwm_STATUS_FIFONEMPTY)
    #define motor_l_pwm_STATUS_TC_INT_EN_MASK              (motor_l_pwm_STATUS_TC)
    #define motor_l_pwm_STATUS_CMP2_INT_EN_MASK            (motor_l_pwm_STATUS_CMP2)
    #define motor_l_pwm_STATUS_CMP1_INT_EN_MASK            (motor_l_pwm_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define motor_l_pwm_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define motor_l_pwm_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define motor_l_pwm_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define motor_l_pwm_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define motor_l_pwm_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* motor_l_pwm_UsingFixedFunction */

#endif  /* CY_PWM_motor_l_pwm_H */


/* [] END OF FILE */
