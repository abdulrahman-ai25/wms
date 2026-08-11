@echo off
:: 1. Create build directory with GCC / MinGW if not generated
if not exist build (
    cmake -S . -B build -G "MinGW Makefiles"
)

:: 2. Build the project
cmake --build build

:: 3. Run the application
if exist build\WMS.exe (
    .\build\WMS.exe
)