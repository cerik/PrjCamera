/*******************************************************************************
  *
  *
  * ------------------------------------------
  * 2017.03.09, Create by CCZY;
  */

#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_

#include "datatype.h"
#include "stm32f1xx_hal.h"
//==============================================================================
//循环队列定义;
// header为队头,为数据的出口;
// tail为队尾,为数据的入口;
// 当header==tail为真时,表示队列为空;
// 当 tail+1==header为真时,表示队列已满;
//-----------------------------------

#if defined(__CC_ARM)
#pragma anon_unions
#endif

#define COLD_FUN  1
#define WORM_FUN  0


typedef struct{
    UINT8 *mBufPtr;
    INT16  mHeader;
    INT16  mTail;
    UINT16 mTotalSize;
}tCQueue;

//******************************************

void  CQueueInit(tCQueue *pQueue,UINT8 *bufPtr,UINT16 bufLen);
void  CQueueFlush(tCQueue *pQueue);
INT16 CQueueGetMsgSize(const tCQueue *pQueue);
INT16 CQueueGetFreeSize(const tCQueue *pQueue);
//==================================================================================
//返回成功入队的字节数目;
//
INT16 CQueuePut(tCQueue *pQueue,const UINT8 *pSrc,INT16 len);

//==================================================================================
//如果update==true,则被取出的内容会被更新;
//否则为保护型取出;
INT16 CQueueGet(tCQueue *pQueue,UINT8 *pSrc,INT16 len,BOOL update);



#endif // QUEUE_H
