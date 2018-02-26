setlocal

REM setup our path vars
.\paths.bat

REM compile
cl /EHsc /std:c++14 /Fo:%objpath% main.cpp /Fe:%buildpath%\time_pest.exe
