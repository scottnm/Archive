@echo off
setlocal

REM Compile
cl -analyze -std:c++14 /DEBUG:FULL /W4 /WX src\*.cpp -Fo:obj\ -Fe:bin\
