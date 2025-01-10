#include "init/i2s_init.h"
#include "main.h"

I2S_HandleTypeDef hi2s2;

void I2S2_Init(void)
{
    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_MASTER_FULLDUPLEX;  // 全双工模式
    hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;    // I2S飞利浦标准
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;    // 16位数据格式
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE; // 启用MCLK输出
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_44K;      // 44.1KHz采样率
    hi2s2.Init.CPOL = I2S_CPOL_LOW;
    hi2s2.Init.FirstBit = I2S_FIRSTBIT_MSB;
    hi2s2.Init.WSInversion = I2S_WS_INVERSION_DISABLE;
    hi2s2.Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
    hi2s2.Init.MasterKeepIOState = I2S_MASTER_KEEP_IO_STATE_DISABLE;

    if (HAL_I2S_Init(&hi2s2) != HAL_OK)
    {
        Error_Handler();
    }
}
