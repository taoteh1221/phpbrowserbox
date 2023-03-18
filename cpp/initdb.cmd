@echo off

rem change drive
cd /D "%~dp0"

rem change directory
cd "%~dp0..\"


SET maincwd=%~dp0
cd "%maincwd%"

cd ..

set "maincwd=%cd%"

cd bin\mysql\bin

if exist ..\data goto EXITMENU

mysqld --initialize-insecure --user=mysql

:EXITMENU
