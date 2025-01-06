#ifndef __SPI_INIT_H
#define __SPI_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"

/* SPI句柄定义 */
extern SPI_HandleTypeDef hspi1;

/* 函数声明 */
void SPI1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_INIT_H */
