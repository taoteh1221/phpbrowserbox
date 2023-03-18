@echo off 

SET cccwd=%~dp0

set "bbcomposer=%cccwd%composer.phar"

php "%bbcomposer%" "%*"
