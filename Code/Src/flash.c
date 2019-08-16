#include "flash.h"
#include <string.h>

/**
  * @brief  Erases sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successeful, MAL_FAIL else.
  */
FLASH_StatusTypeDef Flash_Erase(uint32_t Add)
{
  uint32_t PageError = 0;
  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

   /* Get the number of sector to erase from 1st sector*/
  //eraseinitstruct.Banks = FLASH_BANK_1;
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = Add;//(Add - FLASH_BASE) / FLASH_PAGE_SIZE;
  eraseinitstruct.NbPages = 1;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if (status != HAL_OK)
  {
    return FLASH_ERR;
  }
  return FLASH_OK;
}

/**
  * @brief  Writes Data into Memory.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval 0 if operation is successeful, MAL_FAIL else.
  */
FLASH_StatusTypeDef Flash_Write(uint8_t *src, uint32_t dest_addr, uint32_t Len)
{
  uint32_t i = 0;
  
  HAL_FLASH_Unlock();
  
  Flash_Erase(dest_addr);

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  for(i = 0; i < Len; i += 8)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by byte */
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)(dest_addr+i), *(uint64_t*)(src+i)) == HAL_OK)
    {
     /* Check the written value */
      if(*(uint64_t *)(src + i) != *(uint64_t*)(dest_addr+i))
      {
        /* Flash content doesn't match SRAM content */
        HAL_FLASH_Lock();
        return FLASH_CHECK_ERR;
      }

    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      HAL_FLASH_Lock();
      return FLASH_ERR;
    }
  }
  
  HAL_FLASH_Lock();
  return FLASH_OK;
}

/**
  * @brief  Reads Data into Memory.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval return FLASH_OK.
  */
FLASH_StatusTypeDef Flash_Read(uint8_t* buff, uint32_t dest_addr, uint32_t Len)
{
    HAL_FLASH_Unlock();
    uint32_t i;
    for(i = 0; i < Len; i++){
            buff[i] = *(__IO uint8_t*)(dest_addr + i);
    }
    HAL_FLASH_Lock();
  /* Return a valid address to avoid HardFault */
  return FLASH_OK;
}

//FLASH写入数据测试
void Flash_WriteData(uint32_t addr,uint16_t *data,uint16_t Size)
{
    //1、解锁FLASH
  HAL_FLASH_Unlock();

    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = addr;
    f.NbPages = 1;
    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);

    //3、对FLASH烧写
	  uint32_t TempBuf = 0;
	  for(uint32_t i = 0;i< Size ;i++)
	 {
		 TempBuf = ~(*(data+i));
		 TempBuf <<= 16;
		 TempBuf += *(data+i); //取反校验
		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD , addr + i * 4, TempBuf);
	 }
  
    //4、锁住FLASH
  HAL_FLASH_Lock();
}

//FLASH读取数据测试,成功返回1
uint8_t Flash_ReadData(uint32_t addr,uint16_t *data,uint16_t Size)
{
  uint32_t temp;
	uint8_t result = 1;
  for(uint32_t i = 0;i< Size ;i++)
	{
			temp = *(__IO uint32_t*)(addr + i * 4);
		  if((uint16_t)temp == (uint16_t)(~(temp>>16)))
			{
				*(data+i) = (uint16_t)temp;
			}
			else
			{
				result = 0;
			}
	}
  return result;
}
