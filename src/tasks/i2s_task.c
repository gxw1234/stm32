#include "tasks/i2s_task.h"
#include "init/i2s_init.h"
#include <math.h>

#define SAMPLE_RATE     44100   // 采样率
#define FREQUENCY       1000    // 正弦波频率 (1kHz)
#define BUFFER_SIZE     64      // 减小缓冲区大小以提高速度

// 生成正弦波数据
static void GenerateSineWave(uint16_t *buffer, uint16_t size)
{
    static float phase = 0.0f;
    const float amplitude = 32767.0f;  // 16位有符号数的最大值
    const float step = 2.0f * M_PI * FREQUENCY / SAMPLE_RATE;

    for (uint16_t i = 0; i < size; i++)
    {
        // 生成正弦波
        float sample = amplitude * sinf(phase);
        
        // 转换为16位无符号数据
        buffer[i] = (uint16_t)((int16_t)sample + 32768);
        
        // 更新相位
        phase += step;
        if (phase >= 2.0f * M_PI)
        {
            phase -= 2.0f * M_PI;
        }
    }
}

void I2S_Task(void *argument)
{
    uint16_t tx_buffer[BUFFER_SIZE];

    while (1)
    {
        // 生成正弦波数据
        GenerateSineWave(tx_buffer, BUFFER_SIZE);

        // 通过I2S发送数据，减少超时时间
        if (HAL_I2S_Transmit(&hi2s2, (uint16_t*)tx_buffer, BUFFER_SIZE, 100) != HAL_OK)
        {
            Error_Handler();
        }
        
        // 减少延时时间
        taskYIELD();  // 立即让出CPU，不等待
    }
}
