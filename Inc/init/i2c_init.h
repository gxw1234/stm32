#ifndef __I2C_INIT_H
#define __I2C_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void I2C_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_INIT_H */
