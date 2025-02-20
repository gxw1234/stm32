#include "tasks/spi_task.h"
#include "main.h"
#include <stdio.h>
/* CS引脚控制宏定义 */
#define SPI_CS_LOW()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define SPI_CS_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)

void SPI_Task(void *argument)
{
    uint8_t txData[] = {0x55, 0xAA, 0x12, 0x34}; // 发送数据缓冲区
    uint8_t rxData[4]; // 接收数据缓冲区

    /* 任务循环 */
    while(1)
    {
        /* 拉低CS开始传输 */
        SPI_CS_LOW();
        
        /* 同时发送和接收数据 */
        if(HAL_SPI_TransmitReceive(&hspi1, txData, rxData, sizeof(txData), 100) != HAL_OK)
        {
            Error_Handler();
        }
        
        /* 拉高CS结束传输 */
        SPI_CS_HIGH();
        
        /* 打印发送和接收的数据 */
        printf("SPI: TX=0x%02X%02X%02X%02X, RX=0x%02X%02X%02X%02X\n",
               txData[0], txData[1], txData[2], txData[3],
               rxData[0], rxData[1], rxData[2], rxData[3]);

        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
