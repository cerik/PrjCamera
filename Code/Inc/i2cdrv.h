
#ifndef _I2C_DRV_H_
#define _I2C_DRV_H_
#include "datatype.h"

typedef UINT8 (*GetFunPtr)(UINT8 *buf,UINT8 len);
typedef UINT8 (*SetFunptr)(UINT8 *buf,UINT8 len);

extern GetFunPtr gI2C_GetFun[2];
extern SetFunptr gI2C_SetFun[2];

UINT8 CheckReady(void);

UINT8 BQ24725_Get(UINT8 *buf,UINT8 len);
UINT8 BQ24725_Set(UINT8 *buf,UINT8 len);
UINT8 SYA1232_Get(UINT8 *buf,UINT8 len);
UINT8 SYA1232_Set(UINT8 *buf,UINT8 len);

#endif
