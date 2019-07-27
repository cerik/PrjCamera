/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "main.h"
#include "userTask.h"
#include "sysdb.h"
#include "cqueue.h"
#include "bsp.h"
#include "cmdProcess.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
      
#define BIT_0   ( 1 << 0 )
#define BIT_1   ( 1 << 1 )
#define BIT_2   ( 1 << 2 )
      
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
      
osThreadId TaskCmdHandleNaHandle;
osThreadId TaskDBGatherNamHandle;

osThreadId gHtbeatTaskHandle;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void SetComMsgEvent(void)
{
    osSignalSet( TaskCmdHandleNaHandle, BIT_1);
}

void CreateUserTask(void)
{
    /* Create the thread(s) */
    /* definition and creation of TaskCmdHandleNa */
    osThreadDef(TaskCmdHandleNa, TaskCmdHandle, osPriorityNormal, 0, 64);
    TaskCmdHandleNaHandle = osThreadCreate(osThread(TaskCmdHandleNa), NULL);

    /* definition and creation of TaskDBGatherNam */
    osThreadDef(TaskDBGatherNam, TaskDBGather, osPriorityIdle, 0, 64);
    TaskDBGatherNamHandle = osThreadCreate(osThread(TaskDBGatherNam), NULL);
}

/* USER CODE BEGIN Header_TaskCmdHandle */
/**
  * @brief  Function implementing the TaskCmdHandleNa thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_TaskCmdHandle */
void TaskCmdHandle(void const * argument)
{
    osEvent mEvt;
    UINT8   mMsgBuf[16];
    tHostReqMsg *mComMsgPtr;
    
    InitCmdProcess();
    
    /* USER CODE BEGIN 5 */
    StartAdc();
    
    /* Infinite loop */
    StartComRxInterrupt();
    /* Infinite loop */
    for(;;)
    {
        mEvt = osSignalWait( BIT_1, 1000);
        if(mEvt.status == osEventSignal)
        {
            if(BIT_1 == mEvt.value.signals )
            {
                mComMsgPtr = (tHostReqMsg *)mMsgBuf;
                while(1)
                {
                    if((CQueueGetMsgSize(&gMsgQueue) < sizeof(tHostReqMsg)))
                    {
                        break;
                    }
                    
                    if(mComMsgPtr->header == 0xAA55)
                    {
                        CQueueGet(&gMsgQueue,mMsgBuf,sizeof(tHostReqMsg),TRUE);
                        switch(mComMsgPtr->cmd)
                        {
                        case 1:
                            break;
                        }
                    }
                    else
                    {
                        CQueueGet(&gMsgQueue,mMsgBuf,1,TRUE);
                    }
                }
            }
        }
    }
    /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_TaskDBGather */
/**
* @brief Function implementing the TaskDBGatherNam thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TaskDBGather */
void TaskDBGather(void const * argument)
{
    /* USER CODE BEGIN TaskDBGather */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1000);
        ToggleLED(1);
#ifdef IWDG_ENABLE
        HAL_IWDG_Refresh(&hiwdg);
#endif
    }
    /* USER CODE END TaskDBGather */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
