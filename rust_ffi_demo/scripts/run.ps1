$TargetDir = "$PSScriptRoot\..\target\"

ls $TargetDir\demo_lib_test*.exe | % { & $_.PSPath }
