@echo off
setlocal

:: Variables
set BUILD_DIR=build
set EXECUTABLE=%BUILD_DIR%\bin\main.exe
set SOURCE_FILE=src\main.cpp

:: Default target
if "%1"=="" (
    call :all
    exit /b
)

if "%1"=="all" (
    call :all
    exit /b
)

if "%1"=="build" (
    call :build
    exit /b
)

if "%1"=="run" (
    call :run
    exit /b
)

if "%1"=="clean" (
    call :clean
    exit /b
)

goto :eof

:all
call :build
call :run
goto :eof

:build
echo Building the project using CMake...
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_POLICY_VERSION_MINIMUM="3.5" -S "%CD%" -B "%BUILD_DIR%"
cmake --build "%BUILD_DIR%"
goto :eof

:run
if exist "%EXECUTABLE%" (
    echo Running the executable...
    "%EXECUTABLE%"
) else (
    echo Executable not found. Please build the project first.
)
goto :eof

:clean
echo Cleaning up...
rmdir /s /q %BUILD_DIR%
goto :eof