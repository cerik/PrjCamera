
#include "sysdb.h"
#include "i2cdrv.h"

#include "UsartDriver.h"
#include "flash.h"
#include "bsp.h"

#define flash_len 20
#define value_len 20

tSysDB  g_SysDB = { 0 };
unsigned int f_len = 0;
STRUCT_FLASH StructFlash[flash_len];

unsigned char BQ24725_Config[4][3] = {
	{0x12, 0x22, 0x9B}, 
	{0x14, 0x00, 0x08}, 
	{0x15, 0x00, 0x10}, 
	{0x3F, 0x00, 0x10}
};

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

    UINT8 buf[value_len] = {0};
    char StrBuf[value_len] = {0};
    char tmp[value_len];
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
            
            if(StrBuf[0] == CHIP_SYA1232) 
            {
                if(StrBuf[2] == 0x52) //Read
                {
                    SYA1232_Get(buf, StrBuf[1], StrBuf[3]);
                    tmp[0] = StrBuf[0];
                    tmp[1] = StrBuf[1];
                    for(j = 0; j < StrBuf[3]; j++)
                    {
                        tmp[j + 2] = buf[j];
                    }
                    write(USART1_ID, (char*)tmp, StrBuf[3] + 2);
                } 
                else if (StrBuf[2] == 0x57) //Write
                {
                    for(i = 0; i < StrBuf[3]; i++)
                    {
                        buf[i] = StrBuf[ 4 + i ];
                    }
                    SYA1232_Set(buf, StrBuf[1], StrBuf[3]);
                }
            }
            else if(StrBuf[0] == CHIP_BQ24725)
            {
                
                if(StrBuf[2] == 0x52) //Read
                {
                    BQ24725_Get(buf, StrBuf[1], StrBuf[3]);
                    tmp[0] = StrBuf[0];
                    tmp[1] = StrBuf[1];
                    for(j = 0; j < StrBuf[3]; j++)
                    {
                        tmp[j + 2] = buf[j];
                    }
                    write(USART1_ID, (char*)tmp, StrBuf[3] + 2);
                } 
                else if (StrBuf[2] == 0x57) //Write
                {
                    for(i = 0; i < StrBuf[3]; i++)
                    {
                        buf[i] = StrBuf[ 4 + i ];
                    }
                    BQ24725_Set(buf, StrBuf[1], StrBuf[3]);
                    BQ24725_Flash_Save(StrBuf);
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

void BQ24725_Flash_Already(void)
{
    /*
    HAL_FLASH_Unlock();
    Flash_Erase(DEST_ADDR);
    HAL_FLASH_Lock();
    */
    UINT8 flashBuff[flash_len];
    
    Flash_Read(flashBuff, DEST_ADDR, sizeof(flashBuff));
    
    if(flashBuff[0] == CHIP_BQ24725)
    {
        for(int i=0; i<flash_len; i+=sizeof(STRUCT_FLASH)) {
            if(flashBuff[i] == CHIP_BQ24725) {
                UINT8 bBuf[2];
                for(int j=0; j<2; j++) {
                    bBuf[j] = flashBuff[i+j+3];
                    StructFlash[f_len].mValue[j] = bBuf[j];
                }
                
                BQ24725_Set(bBuf, flashBuff[i+1], flashBuff[i+2]);
                
                StructFlash[f_len].mChip = CHIP_BQ24725;
                StructFlash[f_len].mCmd = flashBuff[i+1];
                StructFlash[f_len].mLen = flashBuff[i+2];
                f_len += 1;
            }
        }
    }
    else 
    {
        for (int i = 0; i < 4; i ++) {
            UINT8 sBuf[2];
            for (int j = 0; j < 2; j ++) {
                sBuf[j] = BQ24725_Config[i][j+1];
            }
            BQ24725_Set(sBuf, BQ24725_Config[i][0], 2);
        }
    }
    
}

void BQ24725_Flash_Save(char *StrBuf) {
    int index = f_len;
    
    for(int j=0; j<f_len; ++j) {
        if(StructFlash[j].mChip == CHIP_BQ24725 && StructFlash[j].mCmd == StrBuf[1]) {
            index = j;
        }
    }
    
    for(int i = 0; i < StrBuf[3]; i++)
    {
        StructFlash[index].mValue[i] = StrBuf[ 4 + i ];
    }
    
    StructFlash[index].mChip = CHIP_BQ24725;
    StructFlash[index].mCmd = StrBuf[1];
    StructFlash[index].mLen = StrBuf[3];

    Flash_Write((uint8_t *)&StructFlash, DEST_ADDR, sizeof(StructFlash));
    
    if(index == f_len) {
        f_len += 1;
    }
}

void SYA1232_Flash_Save(void) {
    UINT8 fbBuf[12] = {0X00,0X00};
    UINT8 ffBuf[12] = {0X00};
    SYA1232_Set(fbBuf,0xBF,2);
    SYA1232_Set(ffBuf,0xB5,1);
}
/* function code end */



