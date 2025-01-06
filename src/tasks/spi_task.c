#include "tasks/spi_task.h"
#include "main.h"
#include <stdio.h>
/* CS引脚控制宏定义 */
#define SPI_CS_LOW()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define SPI_CS_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)

void SPI_Task(void *argument)
{
    uint8_t txData[] = {0x55, 0xAA, 0x12, 0x34}; // 示例发送数据

    /* 任务循环 */
    while(1)
    {
        /* 拉低CS开始传输 */
        SPI_CS_LOW();
        
        /* 发送数据 */
        if(HAL_SPI_Transmit(&hspi1, txData, sizeof(txData), 100) != HAL_OK)
        {
            Error_Handler();
        }
        
        /* 拉高CS结束传输 */
        SPI_CS_HIGH();
        
        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("spi-------send--\n");
    }
}
