@echo off
if not exist dist\win-unpacked\app mkdir dist\win-unpacked\app
cp -r app/* dist/win-unpacked/app
cp -r src/license.txt dist/win-unpacked/license.txt
