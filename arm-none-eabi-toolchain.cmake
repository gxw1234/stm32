set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Get toolchain path from environment variable
if(DEFINED ENV{ARM_TOOLCHAIN_PATH})
    set(TOOLCHAIN_PATH $ENV{ARM_TOOLCHAIN_PATH})
else()
    message(FATAL_ERROR "Please set ARM_TOOLCHAIN_PATH environment variable")
endif()

# The triplet to use for the target
set(TARGET_TRIPLET "arm-none-eabi")

# Set the compilers
if(CMAKE_HOST_WIN32)
    set(EXE_SUFFIX ".exe")
else()
    set(EXE_SUFFIX "")
endif()

set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-gcc${EXE_SUFFIX}")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-g++${EXE_SUFFIX}")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-gcc${EXE_SUFFIX}")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-objcopy${EXE_SUFFIX}")
set(CMAKE_SIZE "${TOOLCHAIN_PATH}/bin/${TARGET_TRIPLET}-size${EXE_SUFFIX}")

# Set the compiler flags
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")
set(CMAKE_ASM_FLAGS_INIT "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")

# Only search for libraries and includes in the toolchain
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
