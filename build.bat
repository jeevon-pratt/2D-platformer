@echo off

REM ********************
REM INSTALL DEPENDENCIES
REM ********************

set VCPKG_ROOT=C:\vcpkg

%VCPKG_ROOT%\vcpkg install sdl2:x64-windows
%VCPKG_ROOT%\vcpkg install sdl2-image:x64-windows
%VCPKG_ROOT%\vcpkg install sdl2-ttf:x64-windows
%VCPKG_ROOT%\vcpkg install jsoncpp:x64-windows

if errorlevel 1 pause & exit /b 1



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
    
REM Build Debug configuration
cmake --build . --config Debug
cmake --build . --config Release   

pause