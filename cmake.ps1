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
            The binary directory (ABSOLUTE) of Qt. This is determined by the path you installed the Qt binaries and the compiler (MSVC / G++) you choosed.
        InstallPrefix
            The install prefix (absolute or relative) of each build type of WMM.
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
$depd = "$shsd/3rd/install/MSVC"

cd $shsd
if ($BuildTypes -eq $null) { $BuildTypes = 'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel' }
if ((test-path build) -eq $false) { mkdir build }
cd build
foreach ($BuildType in $BuildTypes) {
    if ((test-path $BuildType) -eq $false) { mkdir $BuildType }
    cd $BuildType
    $CMakePrefixPath = "$QtBinDir;" + "$depd/googletest-main/$BuildType;" + "$depd/mongo-c-driver-1.22.2/$BuildType/lib;" + "$depd/mongo-cxx-driver-r3.7.0/$BuildType/lib;" + "$depd/duckx-1.2.2/$BuildType/lib;"
    echo $CMakePrefixPath
    cmake ../.. -DCMAKE_BUILD_TYPE=$BuildType -DCMAKE_PREFIX_PATH="$CMakePrefixPath"
    cmake --build . --config $BuildType -j
    $prefix = (split-path -path $InstallPrefix -IsAbsolute) ? "$cwd/" : ""
    $suffix = ($BuildTypes.Count -gt 1) ? "/$BuildType" : ""
    cmake --install . --config $BuildType --prefix "$prefix$InstallPrefix$suffix"
    cd ..
}

cd $cwd
