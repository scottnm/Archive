@echo off
start %windir%\system32\cmd.exe /S /C "cd %cd% && .\scott_chat.exe server scott1"
start %windir%\system32\cmd.exe /S /C "cd %cd% && .\scott_chat.exe client scott2"
