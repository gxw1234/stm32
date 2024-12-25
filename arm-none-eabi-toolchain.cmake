set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Set the path to your toolchain
set(TOOLCHAIN_PATH "D:/tool/cmake/cmake_zip/arm-gnu-toolchain")

# The triplet to use for the target
set(TARGET_TRIPLET "arm-none-eabi")

# Set the compilers
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-g++.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-gcc.exe")

# Set the compiler flags
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
set(CMAKE_ASM_FLAGS_INIT "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")

# Only search for libraries and includes in the toolchain
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
