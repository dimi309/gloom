@echo off
setlocal enabledelayedexpansion
set args_ok=false
set opengl_ok=false
set opengl_cmake_def=

if /I "%~1" == "debug" (
set args_ok=true
set BUILDTYPE=Debug
)
if /I "%~1" == "release" (
set args_ok=true
set BUILDTYPE=Release
)
if /I "%~2" == "" set opengl_ok=true
if /I "%~2" == "opengl" (
set opengl_ok=true
set opengl_cmake_def=-DSMALL3D_OPENGL=ON
)
if not "%opengl_ok%" == "true" set args_ok=false

if "%args_ok%" == "false" (
echo Please indicate build type: debug or release, followed by opengl if you would like to also prepare OpenGL-related libraries.
endlocal & exit /b 1
)

echo Entering small3d directory (must have been copied here)...
cd small3d\scripts
if %errorlevel% neq 0 endlocal & exit /b %errorlevel%
call build-mingw %1 %2
if %errorlevel% neq 0 endlocal & exit /b %errorlevel%
cd ..\..

if /I "%~1" == "debug" set CMAKE_DEFINITIONS=-DCMAKE_BUILD_TYPE=Debug
if /I "%~1" == "release" set CMAKE_DEFINITIONS=-DCMAKE_BUILD_TYPE=Release

if /I "%~2" == "opengl" set CMAKE_DEFINITIONS=%CMAKE_DEFINITIONS% -DSMALL3D_OPENGL=ON

REM Not building ball because it needs to be skipped for OpenGL
REM and continuing in a .bat for loop would produce a lot more
REM code
for %%A in (avoidthebug, chasethegoat, frogremixed, gloom) do (
cd %%A
if exist deps rmdir /Q /S deps
mkdir deps
cd deps
for %%B in (include, lib, shaders) do (
xcopy ..\..\small3d\build\%%B %%B\ /s /e /y
if "!errorlevel!" neq "0" endlocal & exit /b !errorlevel!
)
cd ..
if exist build rmdir /Q /S build
mkdir build
cd build
cmake .. -G"MinGW Makefiles" %CMAKE_DEFINITIONS%
if "!errorlevel!" neq "0" endlocal & exit /b !errorlevel!
cmake --build .
if "!errorlevel!" neq "0" endlocal & exit /b !errorlevel!
cd ..\..
)

@echo all games built successfully

@endlocal
