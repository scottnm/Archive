setlocal

REM setup our path vars
REM set pathscript="%~dp0\paths.bat"
REM call %pathscript%

REM set sources=^
REM "%srcpath%\main.cpp" ^
REM "%srcpath%\email.cpp" 

REM compile
REM cl /EHsc /Wall /WX /std:c++14 /Fo:"%objpath%" src\main.cpp src\email.cpp /Fe:"%exepath%"
REM cl /EHsc /Wall /WX /std:c++14 /Fo.\obj\ src\main.cpp src\email.cpp /Fe.\bin\time_pest.exe
REM cl /EHsc /Wall /WX /std:c++14 src\main.cpp src\email.cpp 
cl /EHsc /std:c++14 src\main.cpp src\email.cpp 
