#include "init/i2c_init.h"
#include "main.h"

I2C_HandleTypeDef hi2c1;

void I2C_Init(void)
{
    /* I2C1 配置 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x10C0ECFF;      // 100KHz速度
    hi2c1.Init.OwnAddress1 = 0;          // 自身地址1
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;  // 7位地址模式
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // 禁用双地址模式
    hi2c1.Init.OwnAddress2 = 0;          // 自身地址2
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // 禁用广播呼叫模式
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     // 禁用时钟延展

    /* 初始化I2C */
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}
