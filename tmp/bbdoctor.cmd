@echo off
rem change drive
cd /D "%~dp0"

rem change directory
cd "%~dp0..\"


SET maincwd=%~dp0
cd "%maincwd%"

cd ..

set "maincwd=%cd%"


color 1F
mode con cols=98 lines=30
title PHPBrowserBox v5.0 Troubleshooting
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

echo.
echo === Checking Application Version
echo PHPBrowserBox 5.1.0 (built: March 20 2023 21:19:11) (ZTS Visual C++ 2019 x64)
echo Copyright (c) The Africoders Network
echo PHPBrowserBox v5.1.0, Copyright (c) Africoders Network



echo.
echo === Checking Apache Version
httpd -v

echo.
echo === Checking PHP Version
php --version

echo.
echo === Checking MySQL Version
mysql --version

echo.
echo === Testing Apache Config
httpd -t
echo.

pause
color
