#include "tasks/adc_task.h"
#include "init/adc_init.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void ADC_Task(void *argument)
{
    uint32_t adc1_value;
    uint32_t voltage_mv;  // 电压值(mV)

    /* 等待系统稳定 */
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        /* 读取ADC值 */
        adc1_value = ADC1_GetValue();
        
        /* 转换为电压值(mV) */
        voltage_mv = (adc1_value * 3300) / 65535;  // 转换为毫伏
        
        /* 打印结果 */
        printf("ADC1(PA0) Raw: %lu, Voltage: %lu.%03lu V\n", 
               adc1_value, 
               voltage_mv / 1000,    // 整数部分
               voltage_mv % 1000);   // 小数部分
        
        /* 延时 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
