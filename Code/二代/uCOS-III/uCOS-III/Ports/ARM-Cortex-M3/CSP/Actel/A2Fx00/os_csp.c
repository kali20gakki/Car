/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                The Real-Time Kernel
*
*
*                                      (c) Copyright 2009, Micrium, Weston, FL
*                                                 All Rights Reserved
*
*                                            ARM CORTEX-M3 OS CHIP SUPPORT
*                                                    ACTEL A2Fx00 
*                                            
*
* File      : OS_CPU.H
* Version   : V3.01.1
* By        : JJL
*             FT
*
* For       : ARMv7M Cortex-M3
* Mode      : Thumb2
************************************************************************************************************************
*/

#define  OS_CSP_MODULE
#include <os.h>
#include <csp.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  OS_CSP_REG_SYSTICK_CR                  (* (CPU_REG32 *)(0xE0042038))

#define  OS_CSP_MSK_SYSTICK_CR_STCLK_DIV        DEF_BIT_FIELD(2u, 28u)
#define  OS_CSP_BIT_SYSTICK_CR_STCLK_DIV_04     DEF_BIT_NONE
#define  OS_CSP_BIT_SYSTICK_CR_STCLK_DIV_08     DEF_BIT_MSK(1u, 28u)
#define  OS_CSP_BIT_SYSTICK_CR_STCLK_DIV_16     DEF_BIT_MSK(2u, 28u)
#define  OS_CSP_BIT_SYSTICK_CR_STCLK_DIV_32     DEF_BIT_MSK(3u, 28u)
#define  OS_CSP_BIT_SYSTICK_CR_NOREF            DEF_BIT_25
#define  OS_CSP_BIT_SYSTICK_CR_SKEW             DEF_BIT_24


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/



/*$PAGE*/
/*
************************************************************************************************************************
*                                                INITIALIZE TICKER INTERRUPT
*
* Description: Initialize the the Tick Interrupt.
*
* Arguments  : none.
*
* Note(s)    : (1) This function MUST be called after OSStart() & after processor initialization.
*
*              (2) The A2Fx00 uses a SYSTIC_CR in conjunction with the systick control register within in the NVIC 
*                  module to control the behavior of the systick timer.
*
*                 The SysTick timer uses FCLK if 'NOREF' bit is set to 1, and uses STCLK if NOREF is set to 0.
************************************************************************************************************************
*/

void  OS_CSP_TickInit (void)
{
    CPU_INT32U  cnts;
    CPU_INT32U  systick_clk;    

    
    systick_clk = CSP_PM_CPU_ClkFreqGet();                      /* Get the FCLK frequency.                              */
    DEF_BIT_SET(OS_CSP_REG_SYSTICK_CR, OS_CSP_BIT_SYSTICK_CR_NOREF);

    cnts = (systick_clk / OSCfg_TickRate_Hz);                  /* Calculate the number of SysTick counts               */

    OS_CPU_SysTickInit(cnts);                                   /* Call the Generic OS Systick initialization           */
}


/*$PAGE*/
/*
************************************************************************************************************************
*                                               INTERRUPT CONTROLLER HANDLER
*
* Description: Handles all Cortex M3 NVIC's external sources interrupts
*
* Arguments  : src_nbr  External interrupt source number
*
* Note(s)    : none.
************************************************************************************************************************
*/

void  OS_CPU_IntHandler (void) 
{
    CPU_SR_ALLOC();
    
    
    CPU_CRITICAL_ENTER();                                       /* Tell the OS that we are starting an ISR              */

    OSIntEnter();

    CPU_CRITICAL_EXIT();
    
    CSP_IntHandler();                                           /* Call the generic CSP interrupt handler.              */
    
    OSIntExit();                                                /* Tell the OS that we are leaving the ISR              */
}