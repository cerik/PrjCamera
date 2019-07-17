//============================================================================
// File Description:
//
//============================================================================
// Log:
//============================================================================

#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#ifndef __FUNCTION__
#define __FUNCTION__  __FILE__
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef         unsigned char  UINT8;
typedef           signed char  INT8;
typedef        unsigned short  UINT16;
typedef          signed short  INT16;
typedef          unsigned int  UINT32;
typedef            signed int  INT32;
typedef                 float  FLOAT32;
typedef                double  FLOAT64;
typedef         unsigned long long  UINT64;
typedef           signed long long INT64;

typedef volatile unsigned char  VUINT8;
typedef volatile   signed short VINT16;
typedef volatile unsigned short VUINT16;
typedef volatile   signed int   VINT32;
typedef volatile unsigned int   VUINT32;

typedef enum {FAILED=0,PASSED=1}TestStatus;
typedef enum {FALSE=0,TRUE=1} BOOL;

typedef union tagDataBytes {
    INT16   i16Val[4];
    UINT16  ui16Val[4];
    INT32   i32Val[2];
    UINT32  ui32Val[2];
    FLOAT64 f64Val;
    UINT64  ui64Val;
    INT64   i64Val;
    UINT8   ucBytes[8];
}DATABYTES;

#define ERR_OK          0
#define ERR_IOFAIL      -1
#define ERR_OV          -2
#define ERR_RXEMPTY     -3
#define ERR_TGAP        -4
#define ERR_CRC         -5
#define ERR_FRAME       -6
#define ERR_FAIL        -7
#define ERR_LEN         -8


#define NULLCHAR  '\0'

#define MAXUINT8  0xFF
#define MAXUINT16 0xFFFF
#define MAXUINT32 0xFFFFFFFF

typedef enum{GPIO_IN_MODE=0,GPIO_OUT_MODE=1} GPIOMODE;
typedef struct{
    INT32  ms;
    UINT32 t0,t1;
}tagCounter;


#endif
