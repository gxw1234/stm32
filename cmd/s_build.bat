@echo off
D:
cd D:\STM32OBJ\demo\build

:: Set environment variables
set "ARM_TOOLCHAIN_PATH=D:\tool\cmake\cmake_zip\arm-gnu-toolchain"
set "PATH=%PATH%;D:\tool\cmake\cmake_zip\ninja"

:: Configure CMake if build directory is empty or reconfiguration is needed
if not exist "build.ninja" (
    echo Configuring CMake...
    cmake -G Ninja ..
)

:: Build
ninja
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Memory usage:
    arm-none-eabi-size gxw_build_product.elf
    echo.
    echo Memory usage percentage:
    echo Flash ^(128KB total^): text + data = 36560 bytes ^(28.42%%^)
    echo RAM ^(1024KB total^): data + bss = 38952 bytes ^(3.80%%^)
) else (
    echo Build failed!
)