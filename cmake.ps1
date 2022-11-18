param(
    [string] $QtBinDir,
    [string] $InstallPrefix = "install",
    [string[]] $BuildTypes,
    [switch] $Help
)

set usage -Option Constant -Value @"

Usage
    Build and install Writing Materials Manager (WMM) in command-line interface (CLI).

    Parameters:
        QtBinaryDir
            The binary directory of Qt. This is determined by the path you installed the Qt binaries and the compiler (MSVC / G++) you choosed.
        InstallPrefix
            The install prefix of each build type of WMM.
        BuildTypes
            The build types to be built and installed.
        Help
            Show this.

    Examples:
        ./cmake -QtBinDir "C:/Qt/6.4.0/msvc2019_64"

"@

if ([string]::IsNullOrEmpty($QtBinDir) -or $Help) {
    echo $usage
    exit
}

$cwd = $pwd
$shsd = $PSScriptRoot

cd $shsd
if ($BuildTypes -eq $null) { $BuildTypes = 'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel' }
if ((test-path build) -eq $false) { mkdir build }
cd build
foreach ($BuildType in $BuildTypes) {
    if ((test-path $BuildType) -eq $false) { mkdir $BuildType }
    cd $BuildType
    cmake ../.. -DCMAKE_BUILD_TYPE=$BuildType -DCMAKE_PREFIX_PATH="$QtBirDir;$shsd/3rd/install/MSVC/googltest-main/$BuildType"
    cmake --build . --config $BuildType -j
    $prefix = (split-path -path $InstallPrefix -IsAbsolute) ? "$cwd/" : ""
    $suffix = ($BuildTypes.Count -gt 1) ? "/$BuildType" : ""
    cmake --install . --config $BuildType --prefix "$prefix$InstallPrefix$suffix"
    cd ..
}

cd $cwd
