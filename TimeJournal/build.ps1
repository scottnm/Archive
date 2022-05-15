Param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration="Debug",
    [switch]$NugetRestore,
    [switch]$Clean
    )

$restoreArg = if ($NugetRestore) { "-t:restore" } else {}
$targetArg = if ($Clean) { "-t:Clean" } else { "" }

MSBuild -m journal.sln -verbosity:normal -p:Configuration=$Configuration $targetArg $restoreArg
