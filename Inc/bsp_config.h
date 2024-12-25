#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

#include "stm32h7xx_hal.h"

/* 函数声明 */
void SystemClock_Config(void);  // 系统时钟配置
void Error_Handler(void);       // 错误处理函数
void BSP_GPIO_Init(void);      // GPIO初始化

#endif /* BSP_CONFIG_H */
