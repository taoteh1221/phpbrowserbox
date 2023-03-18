@echo off
rem change drive
cd /D "%~dp0"

rem change directory
cd "%~dp0..\"



SET maincwd=%~dp0

set "cwd=%cd%"

set "phpbrowserbox=%cd%"

echo Launch App

set "apache=%cwd%\bin\apache\bin\httpd.exe"
set "mysql=%cwd%\bin\mysql\bin\mysqld.exe"

set "ports=%cwd%\tmp\ports.txt"

echo Apache: %apache%
echo MySQL: %mysql%

REM Delete and recreate firewall rules
netsh advfirewall firewall delete rule name="Apache HTTP Server" program="%apache%"
netsh advfirewall firewall delete rule name="mysqld.exe" program="%mysql%"

netsh advfirewall firewall add rule name="Apache HTTP Server" dir=in action=allow program="%apache%" enable=yes
netsh advfirewall firewall add rule name="mysqld.exe" dir=in action=allow program="%mysql%" enable=yes

rem scan ports
netstat -an |find /i "listening" > "%ports%"

:EXITAPP
rem ping 1.0.0.1 -n 5 -w 1000 > NUL
{
	"url": "http://localhost:%apache_port%/",
	"url2": "http://localhost/laravel8/public",
	"apache_port": "%apache_port%",
	"mysql_port": "%mysql_port%"
}

