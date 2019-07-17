
#ifndef _BSP_H_
#define _BSP_H_

#include "datatype.h"

//
//BaseClk base clk = 1MHz.
//
#define TIMER_CNT_CLK 1000000 

//
// PWM Freq = BaseClk/ (TIMER_PWM_CLK+1) = 1MHz / 100 = 10KHz.
// PWM Freq = BaseClk/ 80 = 12.5KHz.
//
#define TIMER_PWM_CLK (80-1) 


void SetPWM(UINT8 Channel, UINT8 precent);
void SetMosPower(UINT8 enable);
void SetSysPower(UINT8 enable);
void StartAdc(void);

float GetPwrSupply(void);
float GetCurrentSupply(void);
float GetNtcTemperature(void);

UINT8 GetCanAddr(void);
UINT8 GetKeySts(void);

void ProcessADCData(void);

BOOL SendComMsg(UINT8 *pBuffer,UINT16 len);

#endif
