
#include "sysdb.h"
#include "i2cdrv.h"

#include "UsartDriver.h"
#include "flash.h"
#include "bsp.h"

#define flash_len 20

tSysDB  g_SysDB = { 0 };
unsigned int f_len = 0;
STRUCT_FLASH StructFlash[flash_len];

static void restart_usart(UART_HandleTypeDef *huart)
{
    if(huart == 0)
    {
        return;
    }
    
    if((huart->ErrorCode |= HAL_UART_ERROR_ORE) != RESET)
    {
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->RxState = HAL_UART_STATE_BUSY_RX;
    
        //SET_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
        //SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

        /* Process Unlocked */
        __HAL_UNLOCK(huart);

        /* Enable the UART Parity Error Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_PE);

        /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

        /* Enable the UART Data Register not empty Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        return;
    }
}

void usart1_receive_task(void const* arg)
{
    unsigned int data_len = 0;
    unsigned int i = 0;
    char data = 0;
    UINT8 buf[6] = {0};
    char StrBuf[10] = {0};
    char tmp[10];
    int j = 0;

    while(1)
    {
        osDelay(8);
        restart_usart(&huart1);
    
        data_len = readBuffLen(USART1_ID);
        if(data_len > 0)
        {
            for( i = 0; i < data_len; ++i )
            {
                data = read_char(USART1_ID);
                
                if(i < 10) 
                {
                    StrBuf[i] = data;
                }
                
                #if DEBUG
                write_char(USART1_ID, data);
                #endif
            }
            
            if(StrBuf[1] == 0x52) //Read
            {
                BQ24725_Get(buf, StrBuf[0], StrBuf[2]);
                tmp[0] = StrBuf[0];
                for(j = 0; j < StrBuf[2]; j++)
                {
                    tmp[j + 1] = buf[j];
                }
                write(USART1_ID, (char*)tmp, StrBuf[2] + 1);
            } 
            else if (StrBuf[1] == 0x57) //Write
            {
                int index = f_len;
                for(int j=0; j<sizeof(StructFlash); ++j) {
                    if(StructFlash[j].mChip == CHIP_BQ24725 && StructFlash[j].mCmd == StrBuf[0]) {
                        index = j;
                    }
                }
                
                for(i = 0; i < StrBuf[2]; i++)
                {
                    buf[i] = StrBuf[ 3 + i ];
                    StructFlash[index].mValue[i] = buf[i];
                }
                
                BQ24725_Set(buf, StrBuf[0], StrBuf[2]);
                
                StructFlash[index].mChip = CHIP_BQ24725;
                StructFlash[index].mCmd = StrBuf[0];
                StructFlash[index].mLen = StrBuf[2];

                Flash_Write((uint8_t *)&StructFlash, DEST_ADDR, sizeof(StructFlash));
                
                if(index == f_len) {
                    f_len += 1;
                }
            }
        }
    }
}
/* function code end */

void usart1_send_task(void const* arg)
{
    
    uint16_t data_len = 0;
    LoopQueue* sendQueue;
    unsigned int i = 0;
    char send_buff[200];
    TickType_t old_time = 0;
    old_time = xTaskGetTickCount();
    
    while(1)
    {
        ToggleLED(1);
        
        osDelay(1000);
        
        while(huart1.gState == HAL_UART_STATE_BUSY_TX && xTaskGetTickCount() - old_time <= 3);
        
        sendQueue = getUsartSendLoopQueue(USART1_ID); /* get send queue */
        
        if(sendQueue != NULL)
        {
            data_len = writeBuffLen(USART1_ID); /* send queue data count */

            if(data_len > 0)
            {
                if(data_len >= 200)
                {
                    data_len = 200;
                }
                
                for( i = 0; i < data_len; ++i)
                {
                    send_buff[i] = read_element_loop_queue(sendQueue);
                    printf("%c", send_buff[i]);
                }
                //printf("\r\n");
                
                //HAL_UART_Transmit_DMA(&huart1, (uint8_t *)send_buff, (uint16_t)data_len); /* DMA send	*/
            }
        }

    }
    
}

void startFlashHandle(void)
{
    UINT8 flashBuff[flash_len];
    
    Flash_Read(flashBuff, DEST_ADDR, sizeof(flashBuff));
    
    for(int i=0; i<flash_len; i+=sizeof(STRUCT_FLASH)) {
        
        UINT8 buf[6];
        for(int j=0; j<6; j++) {
            buf[j] = flashBuff[i+j+3];
        }
        
        if(flashBuff[i] == 0x01) {
           BQ24725_Set(buf, flashBuff[i+1], flashBuff[i+2]); 
        } else if (flashBuff[i] == 0x02) {
            SYA1232_Set(buf, flashBuff[i+1], flashBuff[i+2]);
        }
        
        //printf("sFlash.mChip:0x%02x \r\n", flashBuff[i]);
        //printf("sFlash.mCmd:0x%02x \r\n", flashBuff[i+1]);
        //printf("sFlash.mValue:0x%02x \r\n\r\n", flashBuff[i+3]);
    }
    
}
/* function code end */



