# This script MUST be put at the root dir of this project.
# Remember to run "set-executionpolicy bypass process" and select "Yes" or "Yes to All" before running scripts in PowerShell v7.2.2 and earlier.
# Running this script will DELETE all the dependencies downloaded, built and installed, and then REDOWNLOAD, REBUILD and REINSTALL them again.

param(
    [switch] $Help,
    [switch] $NoDownload
)

if ($Help) {
    echo @"

Usage
    Download, build and install 3rd-party libraries.

    Parameters:
        -Help
            Show this help. No tasks will be run with this flag.
        -NoDownload
            Skip download. Use this when you want to keep the source code downloaded during the last run. The script will directly start to run cmake.

"@
    exit
}

$cwd = $pwd
$shsd = $psscriptroot
$prjd = $shsd
$3rdd = "$shsd\3rd"
$buildd = "$3rdd\build"
$dld = "$3rdd\download"
$instd = "$3rdd\install\MSVC"

# clear previous builds and dependencies
cd $prjd
# if (Test-Path 3rd) { rm -r 3rd }
# mkdir 3rd
# mkdir 3rd/build
# mkdir 3rd/download
# mkdir 3rd/install
if ((test-path 3rd) -eq $false) { mkdir 3rd }
cd 3rd

# lib information:
    # 0. lib name
    # 1. download link
    # 2. extra cmake flags
    # 3. cmake prefix path
# $LibCount = 6;

# $Lib = [string[, ]]::New($LibCount, 4)
# $Lib[0, 0] = 'mongo-c-driver-1.22.1'
# $Lib[0, 1] = 'https://github.com/mongodb/mongo-c-driver/archive/refs/tags/1.22.1.zip'
# $Lib[0, 2] = "-DBUILD_VERSION='1.22.1'"
# $Lib[0, 3] = ""

# $mongocbase = $Lib[0, 0]
# $Lib[1, 0] = 'mongo-cxx-driver-r3.6.7'
# $Lib[1, 1] = 'https://github.com/mongodb/mongo-cxx-driver/archive/refs/tags/r3.6.7.zip'
# $Lib[1, 2] = "-DBUILD_VERSION='3.6.7'"
# $Lib[1, 3] = "$instd/$mongocbase"

# $Lib[2, 0] = 'simdjson-1.0.2'
# $Lib[2, 1] = 'https://github.com/simdjson/simdjson/archive/refs/tags/v1.0.2.zip'
# $Lib[2, 2] = ""
# $Lib[2, 3] = ""

# $Lib[3, 0] = 'yyjson-0.4.0'
# $Lib[3, 1] = 'https://github.com/ibireme/yyjson/archive/refs/tags/0.4.0.zip'
# $Lib[3, 2] = ""
# $Lib[3, 3] = ""

# $Lib[4, 0] = 'rapidjson-1.1.0'
# $Lib[4, 1] = 'https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.zip'
# $Lib[4, 2] = ""
# $Lib[4, 3] = ""

# $Lib[5, 0] = 'duckx-1.2.2'
# $Lib[5, 1] = 'https://github.com/amiremohamadi/DuckX/archive/refs/tags/v1.2.2.zip'
# $Lib[5, 2] = ""
# $Lib[5, 3] = ""

$Lib = @(
    @{
        Name = 'mongo-c-driver-1.22.1'
        Link = 'https://github.com/mongodb/mongo-c-driver/archive/refs/tags/1.22.1.zip'
        Flags = "-DBUILD_VERSION='1.22.1'"
        PrefixPath = ""
    },
    @{
        Name = 'mongo-cxx-driver-r3.6.7'
        Link = 'https://github.com/mongodb/mongo-cxx-driver/archive/refs/tags/r3.6.7.zip'
        Flags = "-DBUILD_VERSION='3.6.7'"
        PrefixPath = "$instd/mongo-c-driver-1.22.1"
    },
    @{
        Name = 'simdjson-1.0.2'
        Link = 'https://github.com/simdjson/simdjson/archive/refs/tags/v1.0.2.zip'
        Flags = ""
        PrefixPath = ""
    },
    # @{
    #     Name = 'yyjson-0.4.0'
    #     Link = 'https://github.com/ibireme/yyjson/archive/refs/tags/0.4.0.zip'
    #     Flags = ""
    #     PrefixPath = ""
    # },
    @{
        Name = 'rapidjson-1.1.0'
        Link = 'https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.zip'
        Flags = ""
        PrefixPath = ""
    },
    @{
        Name = 'duckx-1.2.2'
        Link = 'https://github.com/amiremohamadi/DuckX/archive/refs/tags/v1.2.2.zip'
        Flags = ""
        PrefixPath = ""
    }
    # @{
    #     Name = 'googletest'
    #     Link = 'https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip'
    #     Flags = ""
    #     PrefixPath = ""
    # }
)

# download sources
# cd $dld

# for ($i = 0; $i -lt $LibCount; ++$i) {
#     Write-Host "Downloading $($Lib[$i, 0]) from $($Lib[$i, 1]) ..."
#     $WebClient.DownloadFile($($Lib[$i, 1]), "$pwd/$($Lib[$i, 0]).zip")
#     Write-Host "Decompressing $($Lib[$i, 0]).zip ..."
#     if (Test-Path $Lib[$i, 0]) { rm -r $Lib[$i, 0] } # delete previous extraction result
#     Expand-Archive -LiteralPath "$($Lib[$i, 0]).zip" -DestinationPath .
# }

if ($NoDownload -eq $false) {
    if ((test-path download) -eq $false) { mkdir download }
    cd download
    $WebClient = New-Object System.Net.WebClient # web client for downloads
    for ($i = 0; $i -lt $Lib.Count; ++$i) {
        echo "Downloading $($Lib[$i]['Name']) from $($Lib[$i]['Link']) ..."
        $WebClient.DownloadFile($Lib[$i]['Link'], "$pwd/$($Lib[$i]['Name']).zip")
        echo "Decompressing $($Lib[$i]['Name']).zip ..."
        if (Test-Path $Lib[$i]['Name']) { rm -r $Lib[$i]['Name'] } # delete previous extraction result
        Expand-Archive -LiteralPath "$($Lib[$i]['Name']).zip" -DestinationPath .
    }
    cd ..
    echo "Dependencies download and extraction completed."
}

# start to build dependencies
# cd $buildd
if ((test-path build) -eq $false) { mkdir build }
cd build

$BuildType = ,'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel'
$CommonCFlags = '-DCMAKE_C_STANDARD=17'
$CommonCPPFlags = '-DCMAKE_CXX_STANDARD=20'
# $CommonCPPFlags = '-DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_FLAGS="/Zc:__cplusplus"' # if using MSVC

# for ($i = 0; $i -lt $LibCount; ++$i) {
#     for ($j = 0; $j -lt $BuildType.Length; ++$j) {
#         $InstallPrefix = "$instd/$($Lib[$i, 0])/$($BuildType[$j])"
#         if ($Lib[$i, 3] -ne "") { $PrefixPathFlag = "-DCMAKE_PREFIX_PATH=$($Lib[$i, 3])/$($BuildType[$j])" }
#         else { $PrefixPathFlag = "" }

#         mkdir $Lib[$i, 0]
#         cd $Lib[$i, 0]
#         Write-Host "Running cmake for $($Lib[$i, 0]) using config $($BuildType[$j]) ..."
#         cmake $dld/$($Lib[$i, 0]) $CommonCFlags $CommonCPPFlags $($Lib[$i, 2]) -DCMAKE_BUILD_TYPE="$($BuildType[$j])" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" $PrefixPathFlag `
#             # -G "Visual Studio 17 2022" -A x64 `
#             -DCMAKE_CXX_FLAGS="/Zc:__cplusplus /EHsc /wd4996 /wd4244 /wd4267 /wd4146" # if using MSVC
#         Write-Host "Building $($Lib[$i, 0]) using config $($BuildType[$j]) ..."
#         cmake --build . --config $($BuildType[$j]) -j
#         Write-Host "Installing $($Lib[$i, 0]) using config $($BuildType[$j]) ..."
#         cmake --install . --config $($BuildType[$j]) --prefix $InstallPrefix
#         cd ..
#         rm -r $Lib[$i, 0]
#     }
# }

for ($i = 0; $i -lt $Lib.Count; ++$i) {
    for ($j = 0; $j -lt $BuildType.Count; ++$j) {
        $InstallPrefix = "$instd/$($Lib[$i]['Name'])/$($BuildType[$j])"
        if ($Lib[$i]['PrefixPath'] -ne "") { $PrefixPathFlag = "-DCMAKE_PREFIX_PATH=$($Lib[$i]['PrefixPath'])/$($BuildType[$j])" }
        else { $PrefixPathFlag = "" }

        mkdir $Lib[$i]['Name']
        cd $Lib[$i]['Name']
        echo "Running cmake for $($Lib[$i]['Name']), using build type $($BuildType[$j]) ..."
        cmake $dld/$($Lib[$i]['Name']) $CommonCFlags $CommonCPPFlags $($Lib[$i]['Flags']) -DCMAKE_BUILD_TYPE="$($BuildType[$j])" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" $PrefixPathFlag `
            -DCMAKE_CXX_FLAGS="/Zc:__cplusplus /EHsc /wd4996 /wd4244 /wd4267 /wd4146 /wd4018" # if using MSVC
            # -G "Visual Studio 17 2022" -A x64 `
        echo "Building $($Lib[$i]['Name']), using build type $($BuildType[$j]) ..."
        cmake --build . --config $($BuildType[$j]) -j
        echo "Installing $($Lib[$i]['Name']), using build type $($BuildType[$j]) ..."
        cmake --install . --config $($BuildType[$j]) --prefix $InstallPrefix
        cd ..
        rm -r $Lib[$i]['Name']
    }
}

cd $cwd
