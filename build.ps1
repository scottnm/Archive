Param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration="Debug",

    [switch]$Clean
    )

$targetArg = if ($Clean) { "-t:Clean" } else { "" }
MSBuild journal.sln -verbosity:normal -p:Configuration=$Configuration $targetArg
