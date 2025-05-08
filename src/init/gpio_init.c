#include "init/gpio_init.h"

void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIO端口时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE(); // LCD引脚在GPIOG上
    __HAL_RCC_GPIOI_CLK_ENABLE(); // LCD背光在GPIOI上

    /* 配置GPIO引脚：d10 */
    GPIO_InitStruct.Pin = GPIO_PIN_10;            // 选择引脚3
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上拉下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速模式
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* LCD引脚初始化 - 全部设为推挽输出 */
    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_8 | GPIO_PIN_12 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // LCD通信需要高速
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    /* LCD背光引脚初始化 - PI6 */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    
    /* 初始状态设置 */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);   // CS拉高，不选中
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);  // RST拉高
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_RESET); // 背光打开（低电平激活）
}
