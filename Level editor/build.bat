@echo off
:: Batch script to build the executable and create the installer for the level editor

:: Reusable message types printed to console
set "_pr_exc_=[PROGRAM_EXCEPTION]"
set "_pr_inf_=[PROGRAM_INFO]"
set "_pr_cln_=[PROGRAM_CLEANUP]"
set "_cm_inf_=[CMAKE_INFO]"
set "_cm_exc_=[CMAKE_EXCEPTION]"
set "_cm_suc_=[CMAKE_SUCCESS]"
set "_cp_inf_=[CPACK_INFO]"
set "_cp_exc_=[CPACK_EXCEPTION]"
set "_cp_suc_=[CPACK_SUCCESS]"

set "documentsPath=%USERPROFILE%\Documents\Level editor"
set "outPath=%~dp0out"
set "vsPath=%~dp0.vs"

set "buildPath=%~dp0build"

:: Can not run build.bat without administrative privileges
NET SESSION >nul 2>&1
if %errorlevel% neq 0 (
	echo %_pr_exc_% This script requires administrative privileges. Please run as administrator.
	pause
	exit /b 1
)

:menu
cls

echo Level editor setup

echo.

echo ========================================================

echo.

echo Write the number of your choice to choose the action.
echo.
echo 1. Reconfigure CMake
echo 2. Build Level editor
echo 3. Exit
echo.
echo 9. Clean Visual Studio (DELETES OUT AND .VS FOLDERS)
echo 0. Clean Level editor (DELETES BUILD AND LEVEL EDITOR DOCUMENTS FOLDERS)
echo.
set /p choice="Choice: "

:: Process user input
if "%choice%"=="1" goto cmake
if "%choice%"=="2" goto build
if "%choice%"=="3" exit

if "%choice%"=="9" goto cleanvs
if "%choice%"=="0" goto cleanpr

echo %_pr_exc_% Invalid choice! Please enter a valid number.
pause
goto menu

:cmake
:: Change to the script directory
cd /d "%~dp0"
	
:: Clean the build directory before configuration
if exist "%buildPath%" (
	echo %_pr_cln_% Deleted folder: build
	rd /s /q "%buildPath%"
)
mkdir "%buildPath%"
cd "%buildPath%"

echo %_cm_inf_% Started CMake configuration.

:: Configure the project (Release build)
cmake -DCMAKE_BUILD_TYPE=Release ..

if %errorlevel% neq 0 (
	echo %_cm_exc_% CMake configuration failed.
) else (
	echo %_cm_suc_% Cmake configuration succeeded!
)
	
pause
goto menu

:build
:: Change to the script directory
cd /d "%~dp0"
	
if not exist "%buildPath%" (
	echo Did not find build folder. Please run 'Reconfigure CMake' before building.
) else (
	cd "%buildPath%"
		
	:: Build the project
	echo %_cm_inf_% Started build generation.
	cmake --build . --config Release
	
	if %errorlevel% neq 0 (
		echo %_cm_exc_% Build failed because Level editor.exe did not get generated properly.
	) else (
		echo %_cm_suc_% Build succeeded!
	)
)

pause
goto menu

:cleanvs
:: Change to the script directory
cd /d "%~dp0"
	
echo %_pr_inf_% Running vs clean...
if not exist "%vsPath%" (
	if not exist "%outPath%" (
		echo %_pr_cln_% There are no Visual Studio folders to remove.
		pause
		goto menu
	)
)

if exist "%vsPath%" (
	echo %_pr_cln_% Deleted folder: .vs
	rd /s /q "%vsPath%"
)
echo "%outPath%"
if exist "%outPath%" (
	echo %_pr_cln_% Deleted folder: out
	rd /s /q "%outPath%"
)
	
pause
goto menu

:cleanpr
:: Change to the script directory
cd /d "%~dp0"
	
if not exist "%buildPath%" (
	if not exist "%documentsPath%" (
		echo %_pr_cln_% There are no level editor folders to remove.
		pause
		goto menu
	)
)

if exist "%buildPath%" (
	echo %_pr_cln_% Deleted folder: build
	rd /s /q "%buildPath%"
)
if exist "%documentsPath%" (
	echo %_pr_cln_% Deleted folder: Documents/Level editor
	rd /s /q "%documentsPath%"
)
	
pause
goto menu