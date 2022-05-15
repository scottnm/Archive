@echo off
setlocal

REM -------
REM Compile
REM -------

REM set SIMPLE_RUN=defined

if defined SIMPLE_RUN (
    set SUBSYSTEM=CONSOLE
    set RUN_ENV_DEF=-DSIMPLE_RUN
) else (
    set SUBSYSTEM=WINDOWS
)

set DEVPATH=C:\Users\Scott\Dev
REM set SDLPATH=%DEVPATH%\SDL2\2.0.8
mkdir bin
mkdir obj
set OPT=Debug

cl -W4 -WX -TP -EHsc ^
%RUN_ENV_DEF% ^
/I"SRC\GSL\include" ^
src\*.cpp ^
-Fo:obj\ ^
/link ^
user32.lib ^
gdi32.lib ^
/SUBSYSTEM:%SUBSYSTEM% ^
/OUT:bin\life.exe ^


REM --------------
REM Copy over dlls
REM --------------
REM dll copies here...
