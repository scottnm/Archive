Param(
    [switch]$AttachDebugger
    )

<# JSON FORMAT
{
    startTimeOfDayIso8601: +?<iosTime>,
    endTimeOfDayIso8601: +?<iosTime>,
    period: <uint16_t>,
    unit: <hour|millis>
    cancelOnEnd: <true|false>
}
#>

function FailTest($errMsg)
{
    Write-Host -ForegroundColor Red "Fail! $errMsg"
    exit 1;
}

function IntDiv
{
    Param(
        [Parameter(mandatory=$True)]
        [int]$Dividend,

        [Parameter(mandatory=$True)]
        [int]$Divisor
        )

    return [int](($Dividend / $divisor) - 0.5)
}

function RunTest
{
    Param(
        [Parameter(mandatory=$True)]
        [uint16]$Period,

        [Parameter(mandatory=$True)]
        [int]$ExpectedLogCount,

        [switch]$AttachDebugger
        )

    $appDir = Join-Path $env:LOCALAPPDATA -ChildPath "TimeJournal"
    $testConfigFile = Join-Path $appDir -ChildPath "testConfig.json"
    $testLogFile = Join-Path $appDir -ChildPath "testLog.txt"

    # cleanup previous test's log file, if present
    rm $testLogFile -ErrorAction SilentlyContinue

    $totalMillisecondsToRun = $ExpectedLogCount * $Period;
    $millisecondPart = $totalMillisecondsToRun % 1000;
    $secondPart = IntDiv -Dividend $totalMillisecondsToRun -Divisor 1000
    $config = @{
        startTimeOfDayIso8601="+00:00:00.000";
        endTimeOfDayIso8601=("+00:00:{0:d2}.{1:d3}" -f $secondPart,$millisecondPart);
        period=$Period;
        unit="millis";
        cancelOnEnd=$true;
    }
    ConvertTo-Json $config | Out-File $testConfigFile

    $APPNAME= "journal"

    Write-Host "Test: write a log every $Period ms for $totalMillisecondsToRun ms... " -NoNewLine

    if ($AttachDebugger)
    {
        Start-Process -File "windbgx" -ArgumentList "-g",$PSScriptRoot\x64\Debug\$APPNAME.exe,$testConfigFile,$testLogFile
    }
    else
    {
        Start-Process -File $PSScriptRoot\x64\Debug\$APPNAME.exe -ArgumentList $testConfigFile,$testLogFile
    }

    $sleepStart = Get-Date -Format HH:mm:ss.fff
    Start-Sleep -Milliseconds ($totalMillisecondsToRun + ($Period * 1.5))
    $sleepEnd = Get-Date -Format HH:mm:ss.fff

    $runningProcess = Get-Process -Name $APPNAME -ErrorAction SilentlyContinue
    if ($runningProcess)
    {
        # $runningProcess | %{ $_.Kill() }
        FailTest("$APPNAME still running! Should have terminated.")
    }

    if (!(Test-Path $testLogFile))
    {
        FailTest("$testLogFile did not exist")
    }

    $lineCount = (Get-Content $testLogFile).Count
    if ($lineCount -ne $ExpectedLogCount)
    {
        FailTest("Expected $ExpectedLogCount lines. Found $lineCount. Sleep time: $sleepStart => $sleepEnd")
    }

    Write-Host -ForegroundColor Green "Pass!"
}

foreach ($expectedLogCount in 5,3,8)
{
    foreach ($period in 100,250,500)
    {
        RunTest -Period $period -ExpectedLogCount $expectedLogCount -AttachDebugger:$AttachDebugger
    }
}
