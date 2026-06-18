@echo off

REM ********************
REM GENERATE BUILD FILES
REM ********************

REM To clear and create build directory
rmdir /s /q build
mkdir build
cd build

REM To create the Visual Studio project files
cmake .. ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
    
if errorlevel 1 pause & exit /b 1
    
    
    
REM *************
REM BUILD PROJECT
REM *************
    
REM Build Debug and Release configurations
cmake --build . --config Debug
cmake --build . --config Release   

pause