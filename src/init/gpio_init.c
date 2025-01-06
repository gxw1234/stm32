#include "init/gpio_init.h"

void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIO端口时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /* 配置GPIO引脚：PE3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;            // 选择引脚3
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上拉下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速模式
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
