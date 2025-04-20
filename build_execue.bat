@echo off
setlocal EnableDelayedExpansion

:: Set paths and variables
set BUILD_DIR=build
set EXECUTABLE_NAME=execue.exe
set CMAKE_GENERATOR=Ninja

:: Ensure required tools are in the system PATH
echo Ensuring all required tools are in the system PATH...

:: Check if CMake is available
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: CMake is not installed. Please install CMake first.
    exit /b 1
)

:: Check if Ninja is available (or use default build system)
where ninja >nul 2>nul
if %errorlevel% neq 0 (
    set CMAKE_GENERATOR=Unix Makefiles
    echo Using default build system instead of Ninja.
)

:: Check for C++ compiler
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: C++ compiler is not available. Please install g++ or MSVC.
    exit /b 1
)

:: Ensure vcpkg (or other package managers) is available if using external libraries
where vcpkg >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: vcpkg is not installed. Proceeding without external dependencies.
) else (
    echo Resolving dependencies using vcpkg...
    call vcpkg install --recurse
)

:: Create build directory if it doesn't exist
if not exist %BUILD_DIR% (
    echo Creating build directory...
    mkdir %BUILD_DIR%
)

:: Change to build directory
cd %BUILD_DIR%

:: Run CMake configuration (clears cache, sets up project)
echo Running CMake configuration...
cmake .. -G "%CMAKE_GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

:: Ignore errors and proceed to next step
if %errorlevel% neq 0 (
    echo CMake configuration failed. Cleaning up and aborting.
    exit /b 1
)

:: Build the project (ignores errors automatically)
echo Building the project...
cmake --build . --config Release

:: If build fails, clean up and stop
if %errorlevel% neq 0 (
    echo Build failed. Cleaning up and exiting.
    rd /s /q %BUILD_DIR%
    exit /b 1
)

:: Attempt to run the executable
echo Attempting to run %EXECUTABLE_NAME%...

:: Ignore errors from the execution of the program
%BUILD_DIR%\%EXECUTABLE_NAME% >nul 2>nul
if %errorlevel% neq 0 (
    echo Program failed to execute properly.
    rd /s /q %BUILD_DIR%
    exit /b 1
)

:: If everything worked, clean up build files
echo Cleaning up temporary files...
rd /s /q %BUILD_DIR%

:: Completion message
echo Build and execution successful. Executable is located at %BUILD_DIR%\%EXECUTABLE_NAME%.

exit /b 0
