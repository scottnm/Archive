Param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration="Debug"
    )

MSBuild journal.sln -verbosity:m -p:Configuration=$Configuration
