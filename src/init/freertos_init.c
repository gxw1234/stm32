#include "init/freertos_init.h"
#include <stdio.h>

/* FreeRTOS空闲任务内存分配回调函数 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                 StackType_t **ppxIdleTaskStackBuffer,
                                 uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* FreeRTOS定时器任务内存分配回调函数 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                  StackType_t **ppxTimerTaskStackBuffer,
                                  uint32_t *pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/* RTOS启动完成钩子函数 */
void vApplicationDaemonTaskStartupHook(void)
{
    printf("RTOS Daemon Task Started!\r\n");
}

/* 初始化FreeRTOS */
void FreeRTOS_Init(void)
{
    /* 这里只初始化FreeRTOS的基础功能 */
    /* 具体任务创建和启动由main完成 */
}
