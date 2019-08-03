
#ifndef _SYSDB_H_
#define _SYSDB_H_

#include "datatype.h"

#ifdef __CC_ARM
#pragma anon_unions
#endif

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

#pragma pack()

typedef struct{
    UINT16     Cfg1;
    UINT8      Cfg2;
    UINT8      Cfg3;
}tSysDB;

extern tSysDB  g_SysDB;

#endif
