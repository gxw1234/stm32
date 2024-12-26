#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config/bsp_config.h"
#include "init/uart_init.h"
#include "init/freertos_init.h"
#include "init/init_task.h"
#include "tasks/led_task.h"
#include <stdio.h>

int main(void)
{
    /* 复位所有外设，初始化Flash接口和Systick */
    HAL_Init();

    /* 配置系统时钟 */
    SystemClock_Config();

    /* 初始化GPIO */
    BSP_GPIO_Init();
    
    /* 初始化UART */
    UART_Init();

    /* 初始化FreeRTOS基础功能 */
    FreeRTOS_Init();

    /* 创建初始化任务，优先级高于LED任务 */
    xTaskCreate(Init_Task, "Init", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    /* 创建LED任务 */
    xTaskCreate(LED_Task, "LED", configMINIMAL_STACK_SIZE * 2, NULL, 1, NULL);
    
    /* 启动调度器 */
    vTaskStartScheduler();
    

}
