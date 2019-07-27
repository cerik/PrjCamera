
#ifndef _I2C_DRV_H_
#define _I2C_DRV_H_
#include "datatype.h"

UINT8 CheckReady();

UINT8 BQ24725_Get(UINT8 *buf,UINT8 len);
UINT8 BQ24725_Set(UINT8 *buf,UINT8 len);
UINT8 SYA1232_Get(UINT8 *buf,UINT8 len);
UINT8 SYA1232_Set(UINT8 *buf,UINT8 len);

#endif
