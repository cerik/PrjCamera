
#include "datatype.h"
#include "main.h"
#include "cqueue.h"
#include "userTask.h"
#include "sysdb.h"

#define MSG_BUFFER_SIZE 64

tCQueue gMsgQueue;

static UINT8 lgMsgBuf[MSG_BUFFER_SIZE];
static UINT8 lgComRxDat;

void InitCmdProcess(void)
{
    CQueueInit(&gMsgQueue,lgMsgBuf,MSG_BUFFER_SIZE);
}

/*
 *
 */
void StartComRxInterrupt(void)
{
    if(HAL_UART_Receive_IT(&huart1, (uint8_t *)&lgComRxDat, 1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
    StartComRxInterrupt();
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
    CQueuePut(&gMsgQueue,&lgComRxDat,1);
    if(CQueueGetMsgSize(&gMsgQueue) >= sizeof(tHostReqMsg))
    {
        SetComMsgEvent();
    }
    StartComRxInterrupt();
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler();
}
