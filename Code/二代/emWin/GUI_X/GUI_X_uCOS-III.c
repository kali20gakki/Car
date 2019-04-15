/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_X_uCOS-III.C
Purpose     : This file provides emWin Interface with uCOS-III
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* This file is designed by Eric2013 */
#include <os.h>
#include "GUI_Private.H"

/*
*********************************************************************************************************
*                                         GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_SEM  DispSem;
static  OS_SEM  EventSem;

static  OS_SEM  KeySem;
static  int     KeyPressed;
static  char    KeyIsInited;


/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

int  GUI_X_GetTime (void) 
{
    OS_TICK  time_cur;
    OS_ERR   os_err;
	
    time_cur = OSTimeGet(&os_err);
   (void)&os_err;

    return ((int)time_cur);
}

void  GUI_X_Delay (int period) 
{
	OS_ERR      err;
	OSTimeDly(period, OS_OPT_TIME_DLY, &err);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
void GUI_X_ExecIdle (void) 
{
    GUI_X_Delay(1);
}


/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void  GUI_X_InitOS (void)
{ 
	OS_ERR     err;

	/* 用于资源共享 cnt = 1*/
    OSSemCreate((OS_SEM    *)&DispSem,
                (CPU_CHAR  *)"DispSem",
                (OS_SEM_CTR )1,
                (OS_ERR    *)&err);
	/* 用于事件触发 cnt = 0*/
	OSSemCreate((OS_SEM    *)&EventSem,
                (CPU_CHAR  *)"EventSem",
                (OS_SEM_CTR )0,
                (OS_ERR    *)&err);
}

void  GUI_X_Lock (void)
{ 
	OS_ERR     err;
	   
	OSSemPend((OS_SEM *)&DispSem,
			  (OS_TICK )0,
			  (OS_OPT  )OS_OPT_PEND_BLOCKING,
			  (CPU_TS  )0,
			  (OS_ERR *)&err);
}

void  GUI_X_Unlock (void)
{ 
	OS_ERR     err;

	OSSemPost((OS_SEM *)&DispSem,
	          (OS_OPT  )OS_OPT_POST_1,
	          (OS_ERR *)&err);
}


U32  GUI_X_GetTaskId (void) 
{ 
    CPU_INT16U  id;

    /* 由于存在同优先级的任务，这里不知道是不是可以, uCOS-III中已经没有任务的ID */
    id = (CPU_INT16U)OSTCBCurPtr->Prio;     
	
    return ((U32)id);
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/


void GUI_X_WaitEvent (void) 
{
	OS_ERR     err;
	   
	OSSemPend((OS_SEM *)&EventSem,
	(OS_TICK )0,
	(OS_OPT  )OS_OPT_PEND_BLOCKING,
	(CPU_TS  )0,
	(OS_ERR *)&err);
}


void GUI_X_SignalEvent (void) 
{
	OS_ERR     err;

	OSSemPost((OS_SEM *)&EventSem,
	      (OS_OPT  )OS_OPT_POST_1,
	      (OS_ERR *)&err);
}

/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           CheckInit()
*
* Description : Initialize the GUI keyboard if it is not already done.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : GUI_X_WaitKey().
*               GUI_X_GetKey().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  CheckInit (void) 
{
	if (KeyIsInited == DEF_FALSE) {
		KeyIsInited = DEF_TRUE;
		GUI_X_Init();
	}
}

/*
*********************************************************************************************************
*                                           GUI_X_Init()
*
* Description : (1) Perform keyboard initialization :
*
*                   (a) Implement keyboard initialization signal by creating a counting semaphore.
*
*                       (1) Initialize keyboard initialization signal with no signal by setting the
*                           semaphore count to 0 to block the semaphore.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CheckInit().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void GUI_X_Init (void) 
{
	OS_ERR err;
	
	OSSemCreate((OS_SEM    *)&KeySem,
				(CPU_CHAR  *)"KeySem",
				(OS_SEM_CTR )0,
				(OS_ERR    *)&err);
}

/*
*********************************************************************************************************
*                                           GUI_X_GetKey()
*
* Description : Get the pressed key.
*
* Argument(s) : none.
*
* Return(s)   : Pressed key.
*
* Caller(s)   : various.
*
* Note(s)     : none.
*********************************************************************************************************
*/
int  GUI_X_GetKey (void) 
{
	int r;

	r          = KeyPressed;
	CheckInit();
	KeyPressed = 0;
	return (r);
}

/*
*********************************************************************************************************
*                                           GUI_X_WaitKey()
*
* Description : Wait for a key to be pressed and return it.
*
* Argument(s) : none.
*
* Return(s)   : Pressed key.
*
* Caller(s)   : various.
*
* Note(s)     : none.
*********************************************************************************************************
*/
int  GUI_X_WaitKey (void) 
{
	int    r;
	OS_ERR err;


	CheckInit();
	if (KeyPressed == 0) {   
		OSSemPend((OS_SEM *)&EventSem,
		(OS_TICK )0,
		(OS_OPT  )OS_OPT_PEND_BLOCKING,
		(CPU_TS  )0,
		(OS_ERR *)&err);
	}
	r          = KeyPressed;
	KeyPressed = 0;
	return (r);		   
}

/*
*********************************************************************************************************
*                                          GUI_X_StoreKey()
*
* Description : Store the pressed key.
*
* Argument(s) : Pressed key.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  GUI_X_StoreKey (int k) 
{
	OS_ERR     err;
	
	KeyPressed = k;
	OSSemPost((OS_SEM *)&KeySem,
	      (OS_OPT  )OS_OPT_POST_1,
	      (OS_ERR *)&err);
}

void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }
