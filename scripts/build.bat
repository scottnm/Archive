@echo off
setlocal

REM Compile
cl -std:c++14 /EHs src\*.cpp -Fo:obj\ -Fe:bin\time_pest.exe
