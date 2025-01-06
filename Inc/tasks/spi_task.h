#ifndef __SPI_TASK_H
#define __SPI_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "init/spi_init.h"

void SPI_Task(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_TASK_H */
