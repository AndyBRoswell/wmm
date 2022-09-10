# This script MUST be put at the root dir of this project.
# Remember to run "set-executionpolicy bypass process" and select "Yes" or "Yes to All" before running scripts in PowerShell v7.2.2 and earlier.
# Running this script will DELETE all the dependencies downloaded, built and installed, and then REDOWNLOAD, REBUILD and REINSTALL them again.

$cwd = $pwd
$shsd = $psscriptroot
$prjd = $shsd
$3rdd = "$shsd\3rd"
$buildd = "$3rdd\build"
$dld = "$3rdd\download"
$instd = "$3rdd\install\MSVC"

# clear previous builds and dependencies
cd $prjd
if (Test-Path build) { rm -r build } # also delete the previous builds of the entire project
if (Test-Path 3rd) { rm -r 3rd }
mkdir 3rd
mkdir 3rd/build
mkdir 3rd/download
mkdir 3rd/install

# web client for downloads
$WebClient = New-Object System.Net.WebClient

# lib information:
    # 0. lib name
    # 1. download link
    # 2. extra cmake flags
    # 3. cmake prefix path
$LibCount = 6;

$Lib = [string[, ]]::New($LibCount, 4)
$Lib[0, 0] = 'mongo-c-driver-1.21.0'
$Lib[0, 1] = 'https://github.com/mongodb/mongo-c-driver/archive/refs/tags/1.21.0.zip'
$Lib[0, 2] = "-DBUILD_VERSION='1.21.0'"
$Lib[0, 3] = ""

$mongocbase = $Lib[0, 0]
$Lib[1, 0] = 'mongo-cxx-driver-r3.6.7'
$Lib[1, 1] = 'https://github.com/mongodb/mongo-cxx-driver/archive/refs/tags/r3.6.7.zip'
$Lib[1, 2] = "-DBUILD_VERSION='3.6.7'"
$Lib[1, 3] = "$instd/$mongocbase"

$Lib[2, 0] = 'simdjson-1.0.2'
$Lib[2, 1] = 'https://github.com/simdjson/simdjson/archive/refs/tags/v1.0.2.zip'
$Lib[2, 2] = ""
$Lib[2, 3] = ""

$Lib[3, 0] = 'yyjson-0.4.0'
$Lib[3, 1] = 'https://github.com/ibireme/yyjson/archive/refs/tags/0.4.0.zip'
$Lib[3, 2] = ""
$Lib[3, 3] = ""

$Lib[4, 0] = 'rapidjson-1.1.0'
$Lib[4, 1] = 'https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.zip'
$Lib[4, 2] = ""
$Lib[4, 3] = ""

$Lib[5, 0] = 'duckx-1.2.2'
$Lib[5, 1] = 'https://github.com/amiremohamadi/DuckX/archive/refs/tags/v1.2.2.zip'
$Lib[5, 2] = ""
$Lib[5, 3] = ""

# download sources
cd $dld

for ($i = 0; $i -lt $LibCount; ++$i) {
    Write-Host "Downloading $($Lib[$i, 0]) from $($Lib[$i, 1]) ..."
    $WebClient.DownloadFile($($Lib[$i, 1]), "$pwd/$($Lib[$i, 0]).zip")
    Write-Host "Decompressing $($Lib[$i, 0]).zip ..."
    if (Test-Path $Lib[$i, 0]) { rm -r $Lib[$i, 0] } # delete previous extraction result
    Expand-Archive -LiteralPath "$($Lib[$i, 0]).zip" -DestinationPath .
}

Write-Host "Download and extract dependencies completed."

# start to build dependencies
cd $buildd

$BuildType = ,'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel'
$CommonCFlags = '-DCMAKE_C_STANDARD=17'
$CommonCPPFlags = '-DCMAKE_CXX_STANDARD=20'
# $CommonCPPFlags = '-DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_FLAGS="/Zc:__cplusplus"' # if using MSVC

for ($i = 0; $i -lt $LibCount; ++$i) {
    for ($j = 0; $j -lt $BuildType.Length; ++$j) {
        $InstallPrefix = "$instd/$($Lib[$i, 0])/$($BuildType[$j])"
        if ($Lib[$i, 3] -ne "") { $PrefixPathFlag = "-DCMAKE_PREFIX_PATH=$($Lib[$i, 3])/$($BuildType[$j])" }
        else { $PrefixPathFlag = "" }

        mkdir $Lib[$i, 0]
        cd $Lib[$i, 0]
        Write-Host "Running cmake for $($Lib[$i, 0]) using config $($BuildType[$j]) ..."
        cmake $dld/$($Lib[$i, 0]) $CommonCFlags $CommonCPPFlags $($Lib[$i, 2]) -DCMAKE_BUILD_TYPE="$($BuildType[$j])" -DCMAKE_INSTALL_PREFIX="$InstallPrefix" $PrefixPathFlag `
            -DCMAKE_CXX_FLAGS="/Zc:__cplusplus /EHsc /wd4996" # if using MSVC
        Write-Host "Building $($Lib[$i, 0]) using config $($BuildType[$j]) ..."
        cmake --build . --config $($BuildType[$j]) -j
        Write-Host "Installing $($Lib[$i, 0]) using config $($BuildType[$j]) ..."
        cmake --install . --config $($BuildType[$j]) --prefix $InstallPrefix
        cd ..
        rm -r $Lib[$i, 0]
    }
}

cd $cwd
