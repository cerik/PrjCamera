/*******************************************************************************
  *
  *
  * ------------------------------------------
  * 2017.03.09, Create by CCZY;
  */
#include <string.h>
#include "cqueue.h"


//*********************************************************************************
void CQueueInit(tCQueue *pQueue,UINT8 *bufPtr,UINT16 bufLen)
{
    pQueue->mTotalSize = bufLen;
    pQueue->mBufPtr = bufPtr;
    pQueue->mHeader = 0;
    pQueue->mTail = 0;
    memset(pQueue->mBufPtr,0xFF,bufLen);
}

void  CQueueFlush(tCQueue *pQueue)
{
    pQueue->mHeader = 0;
    pQueue->mTail = 0;
}

INT16 CQueueGetMsgSize(const tCQueue *pQueue)
{
    INT16 len=0;

    if(pQueue->mHeader <= pQueue->mTail)
    {
        len = pQueue->mTail - pQueue->mHeader;
    }
    else
    {
        len = pQueue->mTotalSize - pQueue->mHeader+pQueue->mTail;
    }
    return len;
}

INT16 CQueueGetFreeSize(const tCQueue *pQueue)
{
    return pQueue->mTotalSize - CQueueGetMsgSize(pQueue) - 1;
}

//==================================================================================
//返回成功入队的字节数目;
//
INT16 CQueuePut(tCQueue *pQueue,const UINT8 *pSrc,INT16 len)
{
    INT16 index=0;
    if(len > CQueueGetMsgSize(pQueue))
    {
        return 0;
    }
    for(index=0;index<len;index++)
    {
        pQueue->mBufPtr[pQueue->mTail]  = pSrc[index];
        pQueue->mTail = (pQueue->mTail+1)% pQueue->mTotalSize;
    }
    return index;
}

//==================================================================================
//如果update==true,则被取出的内容会被更新;
//否则为保护型取出;
//返回被读取出的数据字节数目;
//
INT16 CQueueGet(tCQueue *pQueue,UINT8 *pSrc,INT16 len,BOOL update)
{
    INT16 index=0,getLen;
    INT16 mHeader=0;

    getLen = CQueueGetMsgSize(pQueue);
    if(getLen > len)
    {
        getLen = len;
    }

    if(update)
    {
        for(index=0;index<getLen;index++)
        {
            pSrc[index]=pQueue->mBufPtr[pQueue->mHeader];
            pQueue->mBufPtr[pQueue->mHeader] = 0xFF;
            pQueue->mHeader = (pQueue->mHeader+1)% pQueue->mTotalSize;
        }
    }
    else
    {
        mHeader = pQueue->mHeader;
        for(index=0;index<getLen;index++)
        {
            pSrc[index]=pQueue->mBufPtr[mHeader];
            mHeader = (mHeader+1)% pQueue->mTotalSize;
        }
    }
    return index;
}

