#ifndef __ADC_INIT_H
#define __ADC_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_adc_ex.h"

/* ADC GPIO引脚定义 */
#define ADC1_GPIO_PORT               GPIOA
#define ADC1_GPIO_PIN               GPIO_PIN_0
#define ADC1_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* ADC时钟使能宏定义 */
#define ADC1_CLK_ENABLE()           __HAL_RCC_ADC12_CLK_ENABLE()

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

void ADC_Init(void);
uint32_t ADC1_GetValue(void);
uint32_t ADC2_GetValue(void);
uint32_t ADC3_GetValue(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_INIT_H */
