# TODO(15): Provide the following features in a more guided installation
# TODO(16): Support creation of local AppData folder
# TODO(17): Support creation of config file

$appDataFolder = Join-Path $env:LOCALAPPDATA -ChildPath "TimeJournal"
if (!(Test-Path $appDataFolder))
{
    Write-Host "Setting up app folder @ " -NoNewLine
    Write-Host -ForegroundColor Cyan $appDataFolder
    mkdir $appDataFolder | Out-Null
}
