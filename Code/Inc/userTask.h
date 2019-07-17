#ifndef  _FREERTOS_FUN_H_
#define _FREERTOS_FUN_H_

void CreateUserTask(void);

void SetComMsgEvent(void);

void TaskCmdHandle(void const * argument);
void TaskDBGather(void const * argument);

void SetCanMsgEvent(void);
void SetComMsgEvent(void);

#endif
