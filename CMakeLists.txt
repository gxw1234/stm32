cmake_minimum_required(VERSION 3.15)

# Toolchain settings must be set before project()
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set toolchain paths
set(TOOLCHAIN_PATH "D:/tool/cmake/cmake_zip/arm-gnu-toolchain")
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc.exe")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy.exe")
set(CMAKE_SIZE "${TOOLCHAIN_PATH}/bin/arm-none-eabi-size.exe")

project(gxw_build_product C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# MCU settings
set(CPU_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
set(COMMON_FLAGS "${CPU_FLAGS} -Wall -ffunction-sections -fdata-sections -g -O0")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")

# Project settings
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H750VBTx_FLASH.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -specs=nano.specs -specs=nosys.specs -T${LINKER_SCRIPT} -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")

# HAL Library sources
file(GLOB_RECURSE HAL_SOURCES 
    "Drivers/STM32H7xx_HAL_Driver/Src/*.c"
)

# Remove template files
list(FILTER HAL_SOURCES EXCLUDE REGEX ".*template\\.c$")

# CMSIS sources
set(CMSIS_SOURCES
    "Drivers/CMSIS/Device/ST/STM32H7xx/Source/Templates/system_stm32h7xx.c"
)

# Project sources
set(PROJECT_SOURCES
    src/main.c
    src/startup_stm32h750xx.s
    src/stm32h7xx_hal_msp.c
    src/config/bsp_config.c
    src/config/retarget.c
    src/init/init_task.c
    src/init/uart_init.c
    src/init/freertos_init.c
    src/tasks/led_task.c
)

# Include directories
set(EXTRA_INCLUDES
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Inc/config
    ${CMAKE_SOURCE_DIR}/Inc/init
    ${CMAKE_SOURCE_DIR}/Inc/tasks
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
)

# Definitions
add_definitions(-DUSE_HAL_DRIVER -DSTM32H750xx)

# Create executable
add_executable(${PROJECT_NAME}.elf 
    ${PROJECT_SOURCES}
    ${HAL_SOURCES}
    ${CMSIS_SOURCES}
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE ${EXTRA_INCLUDES})

# Create binary and hex files
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
)
