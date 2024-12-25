cmake_minimum_required(VERSION 3.15)

# Toolchain settings must be set before project()
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

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
    src/bsp_config.c
    src/startup_stm32h750xx.s
)

# Include directories
set(EXTRA_INCLUDES
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
)

# Include FreeRTOS configuration
include(${CMAKE_SOURCE_DIR}/freertos_config.cmake)

# Add all include directories
include_directories(${EXTRA_INCLUDES})

# Add definitions
add_definitions(
    -DUSE_HAL_DRIVER
    -DSTM32H750xx
)

# Create executable
add_executable(${PROJECT_NAME}.elf 
    ${PROJECT_SOURCES}
    ${HAL_SOURCES}
    ${CMSIS_SOURCES}
    ${FREERTOS_SOURCES}
)

# Create binary and hex files
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
)
