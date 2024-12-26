#include "init/uart_init.h"
#include "config/retarget.h"
#include <stdio.h>

/* UART句柄声明 */
UART_HandleTypeDef huart1;

/* UART初始化函数 */
void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* 初始化串口和重定向 */
void UART_Init(void)
{
    /* 初始化UART */
    MX_USART1_UART_Init();
    
    /* 初始化printf重定向 */
    RetargetInit(&huart1);
}

/* 获取UART句柄 */
UART_HandleTypeDef* UART_GetHandle(void)
{
    return &huart1;
}
