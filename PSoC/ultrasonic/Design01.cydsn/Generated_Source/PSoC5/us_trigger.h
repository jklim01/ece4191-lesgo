/*******************************************************************************
* File Name: us_trigger.h
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

#if !defined(CY_PWM_us_trigger_H)
#define CY_PWM_us_trigger_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 us_trigger_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define us_trigger_Resolution                     (16u)
#define us_trigger_UsingFixedFunction             (1u)
#define us_trigger_DeadBandMode                   (0u)
#define us_trigger_KillModeMinTime                (0u)
#define us_trigger_KillMode                       (1u)
#define us_trigger_PWMMode                        (0u)
#define us_trigger_PWMModeIsCenterAligned         (0u)
#define us_trigger_DeadBandUsed                   (0u)
#define us_trigger_DeadBand2_4                    (0u)

#if !defined(us_trigger_PWMUDB_genblk8_stsreg__REMOVED)
    #define us_trigger_UseStatus                  (0u)
#else
    #define us_trigger_UseStatus                  (0u)
#endif /* !defined(us_trigger_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(us_trigger_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define us_trigger_UseControl                 (1u)
#else
    #define us_trigger_UseControl                 (0u)
#endif /* !defined(us_trigger_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define us_trigger_UseOneCompareMode              (1u)
#define us_trigger_MinimumKillTime                (1u)
#define us_trigger_EnableMode                     (0u)

#define us_trigger_CompareMode1SW                 (0u)
#define us_trigger_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define us_trigger__B_PWM__DISABLED 0
#define us_trigger__B_PWM__ASYNCHRONOUS 1
#define us_trigger__B_PWM__SINGLECYCLE 2
#define us_trigger__B_PWM__LATCHED 3
#define us_trigger__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define us_trigger__B_PWM__DBMDISABLED 0
#define us_trigger__B_PWM__DBM_2_4_CLOCKS 1
#define us_trigger__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define us_trigger__B_PWM__ONE_OUTPUT 0
#define us_trigger__B_PWM__TWO_OUTPUTS 1
#define us_trigger__B_PWM__DUAL_EDGE 2
#define us_trigger__B_PWM__CENTER_ALIGN 3
#define us_trigger__B_PWM__DITHER 5
#define us_trigger__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define us_trigger__B_PWM__LESS_THAN 1
#define us_trigger__B_PWM__LESS_THAN_OR_EQUAL 2
#define us_trigger__B_PWM__GREATER_THAN 3
#define us_trigger__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define us_trigger__B_PWM__EQUAL 0
#define us_trigger__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!us_trigger_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!us_trigger_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!us_trigger_PWMModeIsCenterAligned) */
        #if (us_trigger_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (us_trigger_UseStatus) */

        /* Backup for Deadband parameters */
        #if(us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_256_CLOCKS || \
            us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(us_trigger_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (us_trigger_KillModeMinTime) */

        /* Backup control register */
        #if(us_trigger_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (us_trigger_UseControl) */

    #endif /* (!us_trigger_UsingFixedFunction) */

}us_trigger_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    us_trigger_Start(void) ;
void    us_trigger_Stop(void) ;

#if (us_trigger_UseStatus || us_trigger_UsingFixedFunction)
    void  us_trigger_SetInterruptMode(uint8 interruptMode) ;
    uint8 us_trigger_ReadStatusRegister(void) ;
#endif /* (us_trigger_UseStatus || us_trigger_UsingFixedFunction) */

#define us_trigger_GetInterruptSource() us_trigger_ReadStatusRegister()

#if (us_trigger_UseControl)
    uint8 us_trigger_ReadControlRegister(void) ;
    void  us_trigger_WriteControlRegister(uint8 control)
          ;
#endif /* (us_trigger_UseControl) */

#if (us_trigger_UseOneCompareMode)
   #if (us_trigger_CompareMode1SW)
       void    us_trigger_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (us_trigger_CompareMode1SW) */
#else
    #if (us_trigger_CompareMode1SW)
        void    us_trigger_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (us_trigger_CompareMode1SW) */
    #if (us_trigger_CompareMode2SW)
        void    us_trigger_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (us_trigger_CompareMode2SW) */
#endif /* (us_trigger_UseOneCompareMode) */

#if (!us_trigger_UsingFixedFunction)
    uint16   us_trigger_ReadCounter(void) ;
    uint16 us_trigger_ReadCapture(void) ;

    #if (us_trigger_UseStatus)
            void us_trigger_ClearFIFO(void) ;
    #endif /* (us_trigger_UseStatus) */

    void    us_trigger_WriteCounter(uint16 counter)
            ;
#endif /* (!us_trigger_UsingFixedFunction) */

void    us_trigger_WritePeriod(uint16 period)
        ;
uint16 us_trigger_ReadPeriod(void) ;

#if (us_trigger_UseOneCompareMode)
    void    us_trigger_WriteCompare(uint16 compare)
            ;
    uint16 us_trigger_ReadCompare(void) ;
#else
    void    us_trigger_WriteCompare1(uint16 compare)
            ;
    uint16 us_trigger_ReadCompare1(void) ;
    void    us_trigger_WriteCompare2(uint16 compare)
            ;
    uint16 us_trigger_ReadCompare2(void) ;
#endif /* (us_trigger_UseOneCompareMode) */


#if (us_trigger_DeadBandUsed)
    void    us_trigger_WriteDeadTime(uint8 deadtime) ;
    uint8   us_trigger_ReadDeadTime(void) ;
#endif /* (us_trigger_DeadBandUsed) */

#if ( us_trigger_KillModeMinTime)
    void us_trigger_WriteKillTime(uint8 killtime) ;
    uint8 us_trigger_ReadKillTime(void) ;
#endif /* ( us_trigger_KillModeMinTime) */

void us_trigger_Init(void) ;
void us_trigger_Enable(void) ;
void us_trigger_Sleep(void) ;
void us_trigger_Wakeup(void) ;
void us_trigger_SaveConfig(void) ;
void us_trigger_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define us_trigger_INIT_PERIOD_VALUE          (65535u)
#define us_trigger_INIT_COMPARE_VALUE1        (65525u)
#define us_trigger_INIT_COMPARE_VALUE2        (63u)
#define us_trigger_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    us_trigger_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    us_trigger_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    us_trigger_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    us_trigger_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define us_trigger_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  us_trigger_CTRL_CMPMODE2_SHIFT)
#define us_trigger_DEFAULT_COMPARE1_MODE      (uint8)((uint8)3u <<  us_trigger_CTRL_CMPMODE1_SHIFT)
#define us_trigger_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (us_trigger_UsingFixedFunction)
   #define us_trigger_PERIOD_LSB              (*(reg16 *) us_trigger_PWMHW__PER0)
   #define us_trigger_PERIOD_LSB_PTR          ( (reg16 *) us_trigger_PWMHW__PER0)
   #define us_trigger_COMPARE1_LSB            (*(reg16 *) us_trigger_PWMHW__CNT_CMP0)
   #define us_trigger_COMPARE1_LSB_PTR        ( (reg16 *) us_trigger_PWMHW__CNT_CMP0)
   #define us_trigger_COMPARE2_LSB            (0x00u)
   #define us_trigger_COMPARE2_LSB_PTR        (0x00u)
   #define us_trigger_COUNTER_LSB             (*(reg16 *) us_trigger_PWMHW__CNT_CMP0)
   #define us_trigger_COUNTER_LSB_PTR         ( (reg16 *) us_trigger_PWMHW__CNT_CMP0)
   #define us_trigger_CAPTURE_LSB             (*(reg16 *) us_trigger_PWMHW__CAP0)
   #define us_trigger_CAPTURE_LSB_PTR         ( (reg16 *) us_trigger_PWMHW__CAP0)
   #define us_trigger_RT1                     (*(reg8 *)  us_trigger_PWMHW__RT1)
   #define us_trigger_RT1_PTR                 ( (reg8 *)  us_trigger_PWMHW__RT1)

#else
   #if (us_trigger_Resolution == 8u) /* 8bit - PWM */

       #if(us_trigger_PWMModeIsCenterAligned)
           #define us_trigger_PERIOD_LSB      (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define us_trigger_PERIOD_LSB_PTR  ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define us_trigger_PERIOD_LSB      (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define us_trigger_PERIOD_LSB_PTR  ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (us_trigger_PWMModeIsCenterAligned) */

       #define us_trigger_COMPARE1_LSB        (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define us_trigger_COMPARE1_LSB_PTR    ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define us_trigger_COMPARE2_LSB        (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define us_trigger_COMPARE2_LSB_PTR    ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define us_trigger_COUNTERCAP_LSB      (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define us_trigger_COUNTERCAP_LSB_PTR  ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define us_trigger_COUNTER_LSB         (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define us_trigger_COUNTER_LSB_PTR     ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define us_trigger_CAPTURE_LSB         (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define us_trigger_CAPTURE_LSB_PTR     ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(us_trigger_PWMModeIsCenterAligned)
               #define us_trigger_PERIOD_LSB      (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define us_trigger_PERIOD_LSB_PTR  ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define us_trigger_PERIOD_LSB      (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define us_trigger_PERIOD_LSB_PTR  ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (us_trigger_PWMModeIsCenterAligned) */

            #define us_trigger_COMPARE1_LSB       (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define us_trigger_COMPARE1_LSB_PTR   ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define us_trigger_COMPARE2_LSB       (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define us_trigger_COMPARE2_LSB_PTR   ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define us_trigger_COUNTERCAP_LSB     (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define us_trigger_COUNTERCAP_LSB_PTR ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define us_trigger_COUNTER_LSB        (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define us_trigger_COUNTER_LSB_PTR    ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define us_trigger_CAPTURE_LSB        (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define us_trigger_CAPTURE_LSB_PTR    ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(us_trigger_PWMModeIsCenterAligned)
               #define us_trigger_PERIOD_LSB      (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define us_trigger_PERIOD_LSB_PTR  ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define us_trigger_PERIOD_LSB      (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define us_trigger_PERIOD_LSB_PTR  ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (us_trigger_PWMModeIsCenterAligned) */

            #define us_trigger_COMPARE1_LSB       (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define us_trigger_COMPARE1_LSB_PTR   ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define us_trigger_COMPARE2_LSB       (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define us_trigger_COMPARE2_LSB_PTR   ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define us_trigger_COUNTERCAP_LSB     (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define us_trigger_COUNTERCAP_LSB_PTR ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define us_trigger_COUNTER_LSB        (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define us_trigger_COUNTER_LSB_PTR    ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define us_trigger_CAPTURE_LSB        (*(reg16 *) us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define us_trigger_CAPTURE_LSB_PTR    ((reg16 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define us_trigger_AUX_CONTROLDP1          (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define us_trigger_AUX_CONTROLDP1_PTR      ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (us_trigger_Resolution == 8) */

   #define us_trigger_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define us_trigger_AUX_CONTROLDP0          (*(reg8 *)  us_trigger_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define us_trigger_AUX_CONTROLDP0_PTR      ((reg8 *)   us_trigger_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (us_trigger_UsingFixedFunction) */

#if(us_trigger_KillModeMinTime )
    #define us_trigger_KILLMODEMINTIME        (*(reg8 *)  us_trigger_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define us_trigger_KILLMODEMINTIME_PTR    ((reg8 *)   us_trigger_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (us_trigger_KillModeMinTime ) */

#if(us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_256_CLOCKS)
    #define us_trigger_DEADBAND_COUNT         (*(reg8 *)  us_trigger_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define us_trigger_DEADBAND_COUNT_PTR     ((reg8 *)   us_trigger_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define us_trigger_DEADBAND_LSB_PTR       ((reg8 *)   us_trigger_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define us_trigger_DEADBAND_LSB           (*(reg8 *)  us_trigger_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (us_trigger_UsingFixedFunction)
        #define us_trigger_DEADBAND_COUNT         (*(reg8 *)  us_trigger_PWMHW__CFG0)
        #define us_trigger_DEADBAND_COUNT_PTR     ((reg8 *)   us_trigger_PWMHW__CFG0)
        #define us_trigger_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << us_trigger_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define us_trigger_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define us_trigger_DEADBAND_COUNT         (*(reg8 *)  us_trigger_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define us_trigger_DEADBAND_COUNT_PTR     ((reg8 *)   us_trigger_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define us_trigger_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << us_trigger_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define us_trigger_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (us_trigger_UsingFixedFunction) */
#endif /* (us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_256_CLOCKS) */



#if (us_trigger_UsingFixedFunction)
    #define us_trigger_STATUS                 (*(reg8 *) us_trigger_PWMHW__SR0)
    #define us_trigger_STATUS_PTR             ((reg8 *) us_trigger_PWMHW__SR0)
    #define us_trigger_STATUS_MASK            (*(reg8 *) us_trigger_PWMHW__SR0)
    #define us_trigger_STATUS_MASK_PTR        ((reg8 *) us_trigger_PWMHW__SR0)
    #define us_trigger_CONTROL                (*(reg8 *) us_trigger_PWMHW__CFG0)
    #define us_trigger_CONTROL_PTR            ((reg8 *) us_trigger_PWMHW__CFG0)
    #define us_trigger_CONTROL2               (*(reg8 *) us_trigger_PWMHW__CFG1)
    #define us_trigger_CONTROL3               (*(reg8 *) us_trigger_PWMHW__CFG2)
    #define us_trigger_GLOBAL_ENABLE          (*(reg8 *) us_trigger_PWMHW__PM_ACT_CFG)
    #define us_trigger_GLOBAL_ENABLE_PTR      ( (reg8 *) us_trigger_PWMHW__PM_ACT_CFG)
    #define us_trigger_GLOBAL_STBY_ENABLE     (*(reg8 *) us_trigger_PWMHW__PM_STBY_CFG)
    #define us_trigger_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) us_trigger_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define us_trigger_BLOCK_EN_MASK          (us_trigger_PWMHW__PM_ACT_MSK)
    #define us_trigger_BLOCK_STBY_EN_MASK     (us_trigger_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define us_trigger_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define us_trigger_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define us_trigger_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define us_trigger_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define us_trigger_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define us_trigger_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define us_trigger_CTRL_ENABLE            (uint8)((uint8)0x01u << us_trigger_CTRL_ENABLE_SHIFT)
    #define us_trigger_CTRL_RESET             (uint8)((uint8)0x01u << us_trigger_CTRL_RESET_SHIFT)
    #define us_trigger_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << us_trigger_CTRL_CMPMODE2_SHIFT)
    #define us_trigger_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << us_trigger_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define us_trigger_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define us_trigger_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << us_trigger_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define us_trigger_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define us_trigger_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define us_trigger_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define us_trigger_STATUS_TC_INT_EN_MASK_SHIFT            (us_trigger_STATUS_TC_SHIFT - 4u)
    #define us_trigger_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define us_trigger_STATUS_CMP1_INT_EN_MASK_SHIFT          (us_trigger_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define us_trigger_STATUS_TC              (uint8)((uint8)0x01u << us_trigger_STATUS_TC_SHIFT)
    #define us_trigger_STATUS_CMP1            (uint8)((uint8)0x01u << us_trigger_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define us_trigger_STATUS_TC_INT_EN_MASK              (uint8)((uint8)us_trigger_STATUS_TC >> 4u)
    #define us_trigger_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)us_trigger_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define us_trigger_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define us_trigger_RT1_MASK              (uint8)((uint8)0x03u << us_trigger_RT1_SHIFT)
    #define us_trigger_SYNC                  (uint8)((uint8)0x03u << us_trigger_RT1_SHIFT)
    #define us_trigger_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define us_trigger_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << us_trigger_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define us_trigger_SYNCDSI_EN            (uint8)((uint8)0x0Fu << us_trigger_SYNCDSI_SHIFT)


#else
    #define us_trigger_STATUS                (*(reg8 *)   us_trigger_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define us_trigger_STATUS_PTR            ((reg8 *)    us_trigger_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define us_trigger_STATUS_MASK           (*(reg8 *)   us_trigger_PWMUDB_genblk8_stsreg__MASK_REG)
    #define us_trigger_STATUS_MASK_PTR       ((reg8 *)    us_trigger_PWMUDB_genblk8_stsreg__MASK_REG)
    #define us_trigger_STATUS_AUX_CTRL       (*(reg8 *)   us_trigger_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define us_trigger_CONTROL               (*(reg8 *)   us_trigger_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define us_trigger_CONTROL_PTR           ((reg8 *)    us_trigger_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define us_trigger_CTRL_ENABLE_SHIFT      (0x07u)
    #define us_trigger_CTRL_RESET_SHIFT       (0x06u)
    #define us_trigger_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define us_trigger_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define us_trigger_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define us_trigger_CTRL_ENABLE            (uint8)((uint8)0x01u << us_trigger_CTRL_ENABLE_SHIFT)
    #define us_trigger_CTRL_RESET             (uint8)((uint8)0x01u << us_trigger_CTRL_RESET_SHIFT)
    #define us_trigger_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << us_trigger_CTRL_CMPMODE2_SHIFT)
    #define us_trigger_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << us_trigger_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define us_trigger_STATUS_KILL_SHIFT          (0x05u)
    #define us_trigger_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define us_trigger_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define us_trigger_STATUS_TC_SHIFT            (0x02u)
    #define us_trigger_STATUS_CMP2_SHIFT          (0x01u)
    #define us_trigger_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define us_trigger_STATUS_KILL_INT_EN_MASK_SHIFT          (us_trigger_STATUS_KILL_SHIFT)
    #define us_trigger_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (us_trigger_STATUS_FIFONEMPTY_SHIFT)
    #define us_trigger_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (us_trigger_STATUS_FIFOFULL_SHIFT)
    #define us_trigger_STATUS_TC_INT_EN_MASK_SHIFT            (us_trigger_STATUS_TC_SHIFT)
    #define us_trigger_STATUS_CMP2_INT_EN_MASK_SHIFT          (us_trigger_STATUS_CMP2_SHIFT)
    #define us_trigger_STATUS_CMP1_INT_EN_MASK_SHIFT          (us_trigger_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define us_trigger_STATUS_KILL            (uint8)((uint8)0x00u << us_trigger_STATUS_KILL_SHIFT )
    #define us_trigger_STATUS_FIFOFULL        (uint8)((uint8)0x01u << us_trigger_STATUS_FIFOFULL_SHIFT)
    #define us_trigger_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << us_trigger_STATUS_FIFONEMPTY_SHIFT)
    #define us_trigger_STATUS_TC              (uint8)((uint8)0x01u << us_trigger_STATUS_TC_SHIFT)
    #define us_trigger_STATUS_CMP2            (uint8)((uint8)0x01u << us_trigger_STATUS_CMP2_SHIFT)
    #define us_trigger_STATUS_CMP1            (uint8)((uint8)0x01u << us_trigger_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define us_trigger_STATUS_KILL_INT_EN_MASK            (us_trigger_STATUS_KILL)
    #define us_trigger_STATUS_FIFOFULL_INT_EN_MASK        (us_trigger_STATUS_FIFOFULL)
    #define us_trigger_STATUS_FIFONEMPTY_INT_EN_MASK      (us_trigger_STATUS_FIFONEMPTY)
    #define us_trigger_STATUS_TC_INT_EN_MASK              (us_trigger_STATUS_TC)
    #define us_trigger_STATUS_CMP2_INT_EN_MASK            (us_trigger_STATUS_CMP2)
    #define us_trigger_STATUS_CMP1_INT_EN_MASK            (us_trigger_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define us_trigger_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define us_trigger_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define us_trigger_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define us_trigger_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define us_trigger_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* us_trigger_UsingFixedFunction */

#endif  /* CY_PWM_us_trigger_H */


/* [] END OF FILE */
