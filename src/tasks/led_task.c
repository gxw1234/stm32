#include "tasks/led_task.h"
#include "main.h"
#include <stdio.h>
void LED_Task(void *pvParameters)
{
    static uint32_t count = 0;
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_10);
        // printf("LED Toggle Count---------: %lu\r\n", ++count);
        vTaskDelay(500);
    }
}
