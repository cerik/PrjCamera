
#include <string.h>
#include "main.h"
#include "bsp.h"
#include "cmsis_os.h"

/* Size of array containing ADC converted values: set to ADC sequencer number of ranks converted, 
// to have a rank in each address */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)    10)

#define ADC_BIT_RESOLUTION 4095.f

/*
 * [0] = U_PWR_ADC, ADC1_IN4;
 * [1] = Currrent_ADC,ADC1_IN5;
 * [2] = NTC_ADC,ADC1_IN6;
 */
static __IO uint32_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];

/* Variable to report ADC sequencer status */
//static uint8_t ubSequenceCompleted = RESET;     /* Set when all ranks of the sequence have been converted */

static float lgCvtFalue[3];


/*******************************************************************************
 * @Description:
 *   Set duty cycle for a special channel.
 * @Parameters:
 *   channel ---- target channel will be set,value=[1~6];
 *        [0] = TIM1_CH1:PWM_PUMP
 *        [1] = TIM3_CH3:PWM_PAS_1
 *        [2] = TIM3_CH4 = PWM_PAS_2
 *        [3] = TIM4_CH3 = PWM_PEDAL
 *        [4] = TIM4_CH4 = PWM_DEFROST
 *        [5] = TIM2_CH1 = PWM_DRIVER
 *   precent ---- duty cycle, value=[0~100]%
 * @Return: none;
 */
void SetPWM(uint8_t channel, uint8_t precent)
{
    //TIM_OC_InitTypeDef sConfig;
    HAL_StatusTypeDef  mSts = HAL_OK;
#if 0
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

    /* Set the pulse value for channel 1 */
    sConfig.Pulse = (TIMER_PWM_CLK+1) * precent / 100;
    switch(channel)
    {
    case 1://TIM1_CH1 = PWM_PUMP
        mSts =  HAL_TIM_PWM_ConfigChannel(&htim1, &sConfig, TIM_CHANNEL_1);
        if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
        {
            Error_Handler();/* PWM Generation Error */
        }
        break;
    case 2://TIM3_CH3 = PWM_PAS_1
        mSts =  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfig, TIM_CHANNEL_3);
        if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    case 3://TIM3_CH4 = PWM_PAS_2
        mSts =  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfig, TIM_CHANNEL_4);
        if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK)
        {
            Error_Handler();/* PWM Generation Error */
        }
        break;
    case 4://TIM4_CH3 = PWM_PEDAL
        mSts =  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfig, TIM_CHANNEL_3);
        if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3) != HAL_OK)
        {
            Error_Handler();/* PWM Generation Error */
        }
        break;
    case 5://TIM4_CH4 = PWM_DEFROST
        mSts =  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfig, TIM_CHANNEL_4);
        if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4) != HAL_OK)
        {
            Error_Handler();/* PWM Generation Error */
        }
        break;
    case 6://TIM2_CH1 = PWM_DRIVER
        mSts = HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1);
        if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
        {
            Error_Handler();/* PWM Generation Error */
        }
        break;
    default:
        mSts = HAL_ERROR;
        break;
    }
#endif
    if (mSts != HAL_OK)
    {
        Error_Handler();/* Configuration Error */
    }
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /* Configure the SysTick to have interrupt in 1ms time basis*/
  if (HAL_SYSTICK_Config(SystemCoreClock / (1000U / HAL_TICK_FREQ_100HZ)) > 0U)
  {
    return HAL_ERROR;
  }

  /* Configure the SysTick IRQ priority */
  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
  {
    HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
    uwTickPrio = TickPriority;
  }
  else
  {
    return HAL_ERROR;
  }

  /* Return function status */
  return HAL_OK;
}

/*******************************************************************************
 * MOS_POW ----> PB2
 */
void SetMosPower(uint8_t enable)
{
   // HAL_GPIO_WritePin(MOS_POW_GPIO_Port,MOS_POW_Pin,enable?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/*******************************************************************************
 * 12V_P_ON ----> PB5
 */
void SetSysPower(uint8_t enable)
{
   //HAL_GPIO_WritePin(V12_P_ON_GPIO_Port,V12_P_ON_Pin,enable?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/*******************************************************************************
 * KEY ----> PA1
 */
uint8_t GetKeySts(void)
{
    return 0;//HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin);
}


/*******************************************************************************
 * @Description:
 *   Start adc device to sample analog input signal.
 */
void StartAdc(void)
{
    /* Start ADC conversion on regular group with transfer by DMA */
    if (HAL_ADC_Start_DMA(&hadc1,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE*1
                       ) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }
  
    /* Run the ADC calibration */  
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        /* Calibration Error */
        Error_Handler();
    }
    
    /* Start ADC conversion */
    /* Since sequencer is enabled in discontinuous mode, this will perform    */
    /* the conversion of the next rank in sequencer.                          */
    /* Note: For this example, conversion is triggered by software start,     */
    /*       therefore "HAL_ADC_Start()" must be called for each conversion.  */
    /*       Since DMA transfer has been initiated previously by function     */
    /*       "HAL_ADC_Start_DMA()", this function will keep DMA transfer      */
    /*       active.                                                          */
    HAL_ADC_Start(&hadc1);
}

float GetPwrSupply(void)
{
    return 0.0;//lgCvtFalue[ADC_POWER_SUPPLY];
}

float GetCurrentSupply(void)
{
    return 0.0;//lgCvtFalue[ADC_CURRENT_SUPPLY];
}

BOOL SendComMsg(UINT8 *pBuffer,UINT16 len)
{
    HAL_StatusTypeDef rst;
    rst = HAL_UART_Transmit(&huart1,pBuffer,len,1000);
    if(rst == HAL_OK) return TRUE;
    else return FALSE;
}

void ProcessADCData(void)
{
}

//--------------------------------------------------------------------
//
// Interrupt handler
//
//--------------------------------------------------------------------

/*******************************************************************************
 * @brief  Conversion complete callback in non blocking mode
 * @param  AdcHandle : AdcHandle handle
 * @note   This example shows a simple way to report end of conversion
 *         and get conversion result. You can add your own implementation.
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
    /* Report to main program that ADC sequencer has reached its end */
    ProcessADCData();
}

/*******************************************************************************
 * @brief  Conversion DMA half-transfer callback in non blocking mode 
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    ProcessADCData();
}

/*******************************************************************************
 * @brief  ADC error callback in non blocking mode
 *        (ADC conversion with interruption or transfer by DMA)
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    /* In case of ADC error, call main error handler */
    Error_Handler();
}

