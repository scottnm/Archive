@echo off
start %windir%\system32\cmd.exe /S /C "cd %cd% && .\scott_chat.exe server"
start %windir%\system32\cmd.exe /S /K "cd %cd% && .\scott_chat.exe client"
