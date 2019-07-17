
#ifndef _CRC_16_H_
#define _CRC_16_H_

#include "datatype.h"

#define CRC16_SEED 0xFFFF


UINT16 UpdateCrc16(UINT8 ch,UINT16 crc16);
UINT16 UpdateCrc16Buf(const UINT8 *pBuffer, UINT16 len);


#endif

