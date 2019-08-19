#ifndef _FLASH_H_
#define _FLASH_H_

#include "main.h"
 
#define DEST_ADDR 0x08008000

typedef enum
{
  FLASH_OK       = 0x00U,
  FLASH_ERR      = 0x01U,
  FLASH_CHECK_ERR= 0x02U
} FLASH_StatusTypeDef;

FLASH_StatusTypeDef Flash_Erase(uint32_t Add);

FLASH_StatusTypeDef Flash_Write(uint8_t *src, uint32_t dest_addr, uint32_t Len);

FLASH_StatusTypeDef Flash_Read(uint8_t* buff, uint32_t dest_addr, uint32_t Len);

#endif
