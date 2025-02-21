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
        drdy_flag = 1;  // 设置标志位，表示可以读取数据
        printf("DRDY triggered, ready to read data\n");
    }
}

/* EXTI9_5中断服务函数 */
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);  // 调用HAL库中断处理函数
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

    /* 发送复位命令 */
    SPI_Transmit(0x06);  // RESET命令
    vTaskDelay(pdMS_TO_TICKS(1));  // 延迟1ms确保复位完成

    // /* 配置寄存器0: PGA=1, 连续转换模式 */
    // SPI_Transmit(0x40);  // WREG命令，写寄存器0
    // SPI_Transmit(0x01);  // 配置数据
    // vTaskDelay(pdMS_TO_TICKS(1));

    // /* 配置寄存器1: 20SPS，正常模式 */
    // SPI_Transmit(0x41);  // WREG命令，写寄存器1
    // SPI_Transmit(0x04);  // 配置数据
    // vTaskDelay(pdMS_TO_TICKS(1));

    // /* 配置寄存器2: 外部参考 */
    // SPI_Transmit(0x42);  // WREG命令，写寄存器2
    // SPI_Transmit(0x40);  // 配置数据
    // vTaskDelay(pdMS_TO_TICKS(1));

    // /* 配置寄存器3: IDAC关闭，DRDY模式 */
    // SPI_Transmit(0x43);  // WREG命令，写寄存器3
    // SPI_Transmit(0x00);  // 配置数据
    // vTaskDelay(pdMS_TO_TICKS(1));

    /* 发送启动命令 */
    SPI_Transmit(0x08);  // START/SYNC命令
    SPI_Transmit(0x04);
    SPI_Transmit(0x10);
    SPI_Transmit(0x00);

    vTaskDelay(pdMS_TO_TICKS(1));



    /* 任务循环 */
    while(1)
    {   
        if(drdy_flag)
        {
            printf("DRDY triggered\n");
            drdy_flag = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
