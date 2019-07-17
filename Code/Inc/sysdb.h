
#ifndef _SYSDB_H_
#define _SYSDB_H_

#include "datatype.h"

typedef enum {
    NONE=0
}tPtcDatType;

#pragma pack(1)
typedef union{
    UINT8 data[16];
    
    struct{
        UINT8 datCfg:4;
        UINT8       :4;
        UINT8 other[7];
    }Frm1;
   
    struct{
        UINT8 mDat1;
    }Frm2;
    
    struct {
        UINT8 mDat1;
    }Frm3;
    struct {
        UINT8 mDat1;
    }Frm4;
    
    struct {
        UINT8 mDat1;
    }Frm5;
}tDevAckMsg;

typedef struct{
    UINT16  header; //0xAA55
    UINT8   cmd;
    UINT8   tag;
    UINT16  crc;
}tHostReqMsg;
#pragma pack()

typedef struct{
    UINT16     Cfg1;
    UINT8      Cfg2;
    UINT8      Cfg3;
}tSysDB;

extern tSysDB  g_SysDB;

#endif
