
#ifndef _SYSDB_H_
#define _SYSDB_H_

#include "datatype.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"


#ifdef __CC_ARM
#pragma anon_unions
#endif

#define CHIP_SYA1232 0x01
#define CHIP_BQ24725 0x02

typedef enum {
    NONE=0
}tPtcDatType;

#pragma pack(1)

typedef struct{
    UINT16  mHeader; //0xAA55
    UINT8   mCmd;
    UINT8   mRorW;
    UINT8   mChip;
    UINT8   mValue;
    UINT16  mRegAddr;
    
    UINT16  crc;
}tToDevMsg;

typedef struct{
    UINT16 mHeader; //0xAA55
    UINT8  mCmd;

    UINT8  mChip;
    UINT8  mValue;
    
    UINT16  crc;
}tToHostMsg;

typedef struct{
    UINT8  mChip;
    UINT8  mCmd;
    UINT8  mLen;
    UINT8  mValue[2];
}STRUCT_FLASH;

#pragma pack()

typedef struct{
    UINT16     Cfg1;
    UINT8      Cfg2;
    UINT8      Cfg3;
}tSysDB;

extern tSysDB  g_SysDB;

#define DEBUG 0   /* 选择打开Debug调试模式  串口接收数据->将数据返回 */
void usart1_receive_task(void const* arg);
void usart1_send_task(void const* arg);
	
void app_run(void);   /* 所有任务初始化 */

void BQ24725_Flash_Already(void);
void BQ24725_Flash_Save(char *StrBuf);
void SYA1232_Flash_Save(void);
		
extern IWDG_HandleTypeDef hiwdg;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim2;
#endif
