
#include <string.h>
#include "main.h"
#include "bsp.h"
#include "cmsis_os.h"

/* Size of array containing ADC converted values: set to ADC sequencer number of ranks converted, 
// to have a rank in each address */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)    20)

#define ADC_BIT_RESOLUTION 4095.f

/*
 * [0] = U_PWR_ADC, ADC1_IN4;
 * [1] = Currrent_ADC,ADC1_IN5;
 * [2] = NTC_ADC,ADC1_IN6;
 */
static __IO uint32_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];

/* Variable to report ADC sequencer status */

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
 * ACOK ----> PB9
 */
UINT8 GetACOK(void)
{
    return HAL_GPIO_ReadPin(ACOK_GPIO_Port,ACOK_Pin);
}


/*******************************************************************************/
void SetLED(UINT8 Channel,UINT8 Enable)
{
    if(Channel == 0)
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,Enable?GPIO_PIN_RESET:GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,Enable?GPIO_PIN_RESET:GPIO_PIN_SET);
    }
}

void ToggleLED(UINT8 channel)
{
    if(channel == 0)
        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    else
        HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
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
                        ADCCONVERTEDVALUES_BUFFER_SIZE
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

void StartTim(void)
{
    if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }
    if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }
    if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }
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
    UINT8 i;
    printf("\r\n");
    for (i=0; i<10; i++) {
        //(float) aADCxConvertedValues[i] * 3.3 / 4095;
//        printf("ADC1_CH%d£º%d\r\n", i, aADCxConvertedValues[i]);
    }
//    printf("\r\n");
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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    uint32_t uwDiffCapture;
 
    if ((htim->Instance == TIM2) && (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2))
    {
        uwDiffCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//        printf("TIM2_CH2:%d \r\n", uwDiffCapture);
    }
    else if ((htim->Instance == TIM3) && (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1))
    {
        uwDiffCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//        printf("TIM3_CH1:%d \r\n", uwDiffCapture);
    }
    else if ((htim->Instance == TIM3) && (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2))
    {
        uwDiffCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//        printf("TIM3_CH2:%d \r\n", uwDiffCapture);
    }
}

