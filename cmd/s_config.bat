@echo off
D:
cd D:\STM32OBJ\demo\build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-toolchain.cmake ..
pause
