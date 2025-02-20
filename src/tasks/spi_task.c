#include "tasks/spi_task.h"
#include "main.h"
#include <stdio.h>

/* CS引脚控制宏定义 */
#define SPI_CS_LOW()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define SPI_CS_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)

static volatile uint8_t drdy_flag = 0;  // DRDY中断标志位

/* DRDY中断回调函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_5)
    {
        drdy_flag = 1;  // 简单设置标志位，在主循环中处理
    }
}

/* 配置DRDY引脚为中断模式 */
static void DRDY_INT_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* 配置DRDY引脚(PD5)为中断模式 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;   /* 下降沿触发 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;            /* 上拉，空闲时为高电平 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* 配置NVIC，设置较高优先级 */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void SPI_Transmit(uint8_t data) {
    uint8_t txData[] = {data};  // 发送数据缓冲区
    uint8_t rxData[1];          // 接收数据缓冲区

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
    printf("SPI: TX=0x%02X, RX=0x%02X\n", txData[0], rxData[0]);
}

void SPI_Task(void *argument)
{
    /* 初始化DRDY中断 */
    DRDY_INT_Init();

    uint8_t rxData = 0;
    
    /* 任务循环 */
    while(1)
    {
        SPI_CS_LOW();
        HAL_SPI_Receive(&hspi1, &rxData, 1, 100);
        SPI_CS_HIGH();
        
        printf("spi: 0x%02X\n", rxData);
        
        /* 延时1秒 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
