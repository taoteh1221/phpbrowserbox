@echo off
@echo off
rem change drive
cd /D "%~dp0"

rem change directory
cd "%~dp0..\"



SET maincwd=%~dp0

set "cwd=%cd%"


set "apache=%cwd%\bin\apache\bin\httpd.exe"
set "logs=%cwd%\tmp\startup.txt"

del "%logs%" /f

"%apache%" -M  |find /i "Loaded" > "%logs%"

