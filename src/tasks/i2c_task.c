#include "tasks/i2c_task.h"
#include "init/i2c_init.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#define I2C_DEVICE_ADDR    0x60    
#define I2C_REG_ADDR_01       0x01   
#define I2C_REG_ADDR_00       0x00    
#define I2C_DATA_VALUE     0b11100000    



void I2C_Task(void *argument)
{
    uint8_t data[2];
    HAL_StatusTypeDef status;
    data[0] = I2C_REG_ADDR_01;    
    data[1] = I2C_DATA_VALUE;  
    vTaskDelay(pdMS_TO_TICKS(1000));
    while(1)
    {
       
        status = HAL_I2C_Master_Transmit(&hi2c1, I2C_DEVICE_ADDR << 1, data, 2, HAL_MAX_DELAY);
        
        if (status != HAL_OK)
        {
           
            printf("I2C write error\n");
        }
        else
        {
            printf("I2C write success\n");
        }

        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
