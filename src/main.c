#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_config.h"

/* 任务函数原型声明 */
void LED_Task(void *pvParameters);

/* FreeRTOS静态内存分配 */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

/* FreeRTOS空闲任务内存分配回调函数 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                 StackType_t **ppxIdleTaskStackBuffer,
                                 uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

int main(void)
{
    /* 复位所有外设，初始化Flash接口和Systick */
    HAL_Init();

    /* 配置系统时钟 */
    SystemClock_Config();

    /* 初始化所有配置的外设 */
    BSP_GPIO_Init();

    /* 创建LED任务 */
    xTaskCreate(LED_Task, "LED", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /* 启动调度器 */
    vTaskStartScheduler();

    /* 正常情况下不会执行到这里，因为调度器会接管控制权 */
    while (1) {}
}

void LED_Task(void *pvParameters)
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);  // 翻转LED引脚状态
        vTaskDelay(pdMS_TO_TICKS(500));         // 延时500毫秒
    }
}
