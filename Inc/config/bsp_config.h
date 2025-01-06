#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

/* 函数声明 */
void SystemClock_Config(void);  // 系统时钟配置
void Error_Handler(void);       // 错误处理函数

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CONFIG_H */
