#ifndef  _FREERTOS_FUN_H_
#define _FREERTOS_FUN_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"  
void CreateUserTask(void);

void TaskCmdHandle(void const * argument);
void TaskDBGather(void const * argument);

void SetCanMsgEvent(void);
void SetComMsgEvent(void);

#endif
