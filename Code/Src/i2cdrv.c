
#include "main.h"
#include "i2cdrv.h"

#define I2C_ADDR_BQ24725  0x12
#define I2C_ADDR_SYA1232  0x67 //0x77

#if 0
static HAL_StatusTypeDef Internal_I2C_Set_DMA(I2C_HandleTypeDef *devI2C,UINT8 devAddr,UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst = HAL_I2C_Master_Transmit_DMA(devI2C, (UINT16)devAddr, (UINT8*)buf, len);//!= HAL_OK)
    if(mRst != HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge its address)
         Master restarts communication */
        if (HAL_I2C_GetError(devI2C) != HAL_I2C_ERROR_AF)
        {
          Error_Handler();
        }
    }
    return mRst;
}

static HAL_StatusTypeDef Internal_I2C_Get_DMA(I2C_HandleTypeDef *devI2C,UINT8 devAddr,UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst = HAL_I2C_Master_Receive_DMA(devI2C, (uint16_t)devAddr, (uint8_t *)buf, len);
    if(mRst != HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
        When Acknowledge failure occurs (Slave don't acknowledge its address)
        Master restarts communication */
        if (HAL_I2C_GetError(devI2C) != HAL_I2C_ERROR_AF)
        {
            Error_Handler();
        }
    }
    return mRst;
}
#endif


static HAL_StatusTypeDef Internal_I2C_Set(I2C_HandleTypeDef *devI2C,UINT8 devAddr,UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    
    mRst = HAL_I2C_Master_Transmit(devI2C, (uint16_t)devAddr, (uint8_t*)buf, len, 10000);
    if(mRst == HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
        When Acknowledge failure occurs (Slave don't acknowledge its address)
        Master restarts communication */
        if (HAL_I2C_GetError(devI2C) != HAL_I2C_ERROR_AF)
        {
            Error_Handler();
        }
    }
    return mRst;
}

static HAL_StatusTypeDef Internal_I2C_Get(I2C_HandleTypeDef *devI2C,UINT8 devAddr,UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst = HAL_I2C_Master_Receive(devI2C, (uint16_t)devAddr, (uint8_t *)buf, len, 10000);
    if(mRst == HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
        When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
        if (HAL_I2C_GetError(devI2C) != HAL_I2C_ERROR_AF)
        {
            Error_Handler();
        }
    }
    return mRst;
}

UINT8 CheckReady()
{
    /*  Before starting a new communication transfer, you need to check the current   
      state of the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
    return (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)?1:0;
}

/*******************************************************************************
 *     External Function;
 */


UINT8 BQ24725_Get(UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst = Internal_I2C_Get(&hi2c1,I2C_ADDR_BQ24725,buf,len);
    return mRst == HAL_OK?0:1;
}

UINT8 BQ24725_Set(UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst =  Internal_I2C_Set(&hi2c1,I2C_ADDR_BQ24725,buf,len);
    return mRst == HAL_OK?0:1;
}

UINT8 SYA1232_Get(UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst = Internal_I2C_Get(&hi2c2,I2C_ADDR_SYA1232,buf,len);
    return mRst == HAL_OK?0:1;
}

UINT8 SYA1232_Set(UINT8 *buf,UINT8 len)
{
    HAL_StatusTypeDef mRst;
    mRst = Internal_I2C_Set(&hi2c2,I2C_ADDR_SYA1232,buf,len);
    return mRst == HAL_OK?0:1;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  I2cHandle: I2C handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED1: Transfer in transmission process is correct */
  //BSP_LED_Toggle(LED1);
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED1: Transfer in reception process is correct */
  //BSP_LED_Toggle(LED1);
}


/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  //BSP_LED_On(LED3); 
}