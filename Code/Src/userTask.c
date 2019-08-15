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
#include <string.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "main.h"
#include "userTask.h"
#include "sysdb.h"
#include "cqueue.h"
#include "bsp.h"
#include "i2cdrv.h"
#include "cmdProcess.h"
#include "flash.h"
#include "sysdb.h"
#include "UsartDriver.h"

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

static void initUsartIT(UART_HandleTypeDef *huart)
{
    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;
    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    /* Enable the UART Parity Error Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_PE);

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
    
}

static void init_system(void)
{
	initUsartBuff(USART1_ID);
	initUsartIT(&huart1);
	//write(USART1_ID, "USART1 ENBALE\n", sizeof("USART1 ENBALE\n")/sizeof(char));
}
/* function code end */


void CreateUserTask(void)
{
    /* Create the thread(s) */
    /* definition and creation of TaskCmdHandleNa */
    osThreadDef(TaskCmdHandleNa, TaskCmdHandle, osPriorityNormal, 0, 64);
    TaskCmdHandleNaHandle = osThreadCreate(osThread(TaskCmdHandleNa), NULL);

    /* definition and creation of TaskDBGatherNam */
    osThreadDef(TaskDBGatherNam, TaskDBGather, osPriorityIdle, 0, 64);
    TaskDBGatherNamHandle = osThreadCreate(osThread(TaskDBGatherNam), NULL);
    
    init_system();
    
    osThreadDef(UART1RXTask, usart1_receive_task, osPriorityRealtime, 0, 512);
    osThreadCreate(osThread(UART1RXTask), NULL);
    
    osThreadDef(UART1TXTask, usart1_send_task, osPriorityBelowNormal, 0, 256);
    osThreadCreate(osThread(UART1TXTask), NULL);    
    
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
    tToDevMsg *mMsgPtr;
    
    InitCmdProcess();
    
    /* USER CODE BEGIN 5 */
    StartAdc();
    
    StartTim();
    
    /* Infinite loop */
    StartComRxInterrupt();
    /* Infinite loop */
    
    ProcessADCData();
    
    printf("ACOK:%d \r\n\r\n", GetACOK());
    
    for(;;)
    {
        mEvt = osSignalWait( BIT_1, 1000);
        if(mEvt.status == osEventSignal)
        {
            if(BIT_1 == mEvt.value.signals )
            {
                mMsgPtr = (tToDevMsg *)mMsgBuf;
                while(1)
                {
                    if((CQueueGetMsgSize(&gMsgQueue) < sizeof(tToDevMsg)))
                    {
                        break;
                    }
                    
                    if(mMsgPtr->mHeader == 0xAA55)
                    {
                        CQueueGet(&gMsgQueue,mMsgBuf,sizeof(tToDevMsg),TRUE);
                        switch(mMsgPtr->mCmd)
                        {
                        case 1:
                            //gI2C_SetFun[mMsgPtr->FromHost.mChip-1](mMsgBuf,2);
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
        //printf("task1 \r\n");
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
    UINT8 buf[12],i;
    /* USER CODE BEGIN TaskDBGather */
    /* Infinite loop */
    
    //Ð´flash
    UINT8 src[]={0x05};
    UINT32 size = sizeof(src);
    UINT8 buff[size];
    Flash_Write(src, DEST_ADDR, size);
    
    osDelay(1000);
    
    //¶Áflash
    Flash_Read(buff, DEST_ADDR, size);
    printf("buff:%04x", buff[0]);
    
    for(;;)
    {
        ToggleLED(1);
        //printf("task2\n");
        
        memset(buf,0,10);
        //BQ24725_Get(buf,0xFE,2);
        //printf("BQ24725_reg:0xFE=%02X,%02X",buf[0],buf[1]);
        
        //BQ24725_Get(buf+2,0xFF,2);
        //printf("BQ24725_reg:0xFD=%02X,%02X",buf[2],buf[3]);
        
        /*
        if( SYA1232_Get(buf,0x09,10) )
        {
            printf("Read Fail, Please reset I2C.\n");
        }
        else
        {
            printf("******REG 0x09:\n");
            for(i=0;i<10;i++)
            {
                printf("%02X ",buf[i]);
            }
            printf("\n");
        }
        osDelay(1000);
        
        if(SYA1232_Get(buf,0x00,4))
        {
            printf("Read Fail, Please reset I2C.\n");
        }
        else
        {
            printf("******REG 0x00:\n");
            for(i=0;i<4;i++)
            {
                printf("%02X ",buf[i]);
            }
            printf("\n");
        }
        */
        osDelay(1000);
#ifdef IWDG_ENABLE
        HAL_IWDG_Refresh(&hiwdg);
#endif
    }
    /* USER CODE END TaskDBGather */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
