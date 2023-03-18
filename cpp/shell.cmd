@echo off
rem change drive
cd /D "%~dp0"

rem change directory
cd "%~dp0..\"


SET maincwd=%~dp0
cd "%maincwd%"

cd ..

set "maincwd=%cd%"

cd www

color 1F
mode con cols=98 lines=30
title PHPBrowserBox v5.0 Shell
setlocal EnableExtensions EnableDelayedExpansion

echo.
echo Setting environment for using PHPBrowserBox for Windows

rem Set environment variables
SET PATH=%maincwd%\bin\php;%maincwd%\bin\mysql\bin;%maincwd%\bin\apache\bin;%maincwd%\bin\support\bin;%maincwd%\bin\support\scripts;%maincwd%;%PATH%

:======================================================================================================================================================
:MAINMENU
echo.
set "cwd=%cd%"

echo %USERNAME%@%COMPUTERNAME% %cwd%

cmd
rem set /p "cmd=# "
rem if /I "%cmd%" == "exit" goto EXITMENU else goto EXECMENU

goto EXITMENU


:EXECMENU
%cmd%
goto MAINMENU

:EXITMENU
rem pause
color
