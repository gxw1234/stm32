#include "init/init_task.h"
#include "main.h"
#include <stdio.h>

void Init_Task(void *pvParameters)
{
    printf("System Initialized!\r\n");
    
    vTaskDelete(NULL);
}
