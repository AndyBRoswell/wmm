param( # command-line args
    [string] $QtBinDir,
    [string] $BinDir,
    [string] $BuildType = 'Release'
)

set usage -option constant -value @"

Usage
    Copy the essentials to the build directory.

    Parameters:
        QtBinaryDir
            The binary directory of Qt. This is determined by the path you installed the Qt binaries and the compiler (MSVC / G++) you choosed.
        BinDir
            The build directory of this program.
        BuildType (default: Release)
            Debug, Release, RelWithDebInfo, MinSizeRel.
        
    Examples:
        .\post-build.ps1 -QtBinDir C:\Qt\6.4.0\msvc2019_64\bin -BinDir out/build/x64-Debug/bin -BuildType Debug
        .\post-build.ps1 -QtBinDir C:\Qt\6.4.0\msvc2019_64\bin -BinDir out/build/x64-Release/bin -BuildType Release
        .\post-build.ps1 -QtBinDir C:\Qt\6.4.0\msvc2019_64\bin -BinDir out/build/x64-RelWithDebInfo/bin -BuildType RelWithDebInfo
        .\post-build.ps1 -QtBinDir C:\Qt\6.4.0\msvc2019_64\bin -BinDir out/build/x64-MinSizeRel/bin -BuildType MinSizeRel

    Notes:
        1. By default PowerShell treats commands, path names and parameter names case-insensitively.
        2. On Windows, you can use / instead of \ as the delimiter of hierachical directories in a path name.

"@

set SupportedBuildType -option constant -value @('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')

if ([string]::IsNullOrEmpty($QtBinDir)) {
    echo 'The binary directory of Qt specified by the argument QtBinaryDir is illegal.'
    echo $usage
    exit
}
if ((test-path $BinDir) -eq $false) {
    mkdir $BinDir
}
$BuildTypeIsIllegal = $true
foreach ($t in $supportedbuildtype) {
    if ($BuildType -ieq $t) { 
        $BuildType = $t
        $BuildTypeIsIllegal = $false
        break
    }
}
if ($BuildTypeIsIllegal -eq $true) {
    echo 'Argument BuildType is illegal.'
    echo $usage
    exit
}

# shorthands
$cwd = $pwd                         # save the current working directory for the restoration of working directory at the end
$shsd = $psscriptroot               # path of this post-build script
$3rdd = "$shsd/3rd/install/MSVC"    # path of 3rd dependencies
$bind = $BinDir                     # binary directory of this software

# post-build begins here
echo 'Post-Build: Copying essentials to build directory ...'
# Enter the directory of script (aka the root directory of this software) for the convenience of subsequent operations. This also avoided wrong behaviors when the working directory of the moment when this script was run wasn't the directory it locates.
cd $shsd

# Python related code, themes and test data
cp -r py $bind/py
cp -r src/thm $bind/thm
cp -r src/ts $bind/ts
cp -r test $bind/test

# 3rd libraries
cp $3rdd/mongo-c-driver-1.22.1/$BuildType/bin/bson-1.0.dll $bind/bson-1.0.dll
cp $3rdd/mongo-c-driver-1.22.1/$BuildType/bin/mongoc-1.0.dll $bind/mongoc-1.0.dll
cp $3rdd/mongo-cxx-driver-r3.6.7/$BuildType/bin/bsoncxx.dll $bind/bsoncxx.dll
cp $3rdd/mongo-cxx-driver-r3.6.7/$BuildType/bin/mongocxx.dll $bind/mongocxx.dll
cp $3rdd/googletest-main/$BuildType/bin/gtest_main.dll $bind/gtest_main.dll
cp $3rdd/googletest-main/$BuildType/bin/gtest.dll $bind/gtest.dll
# Windeployqt doesn't support RelWithDebInfo and MinSizeRel yet. Use Release provisionally.
if ($BuildType -ieq 'debug') { & $QtBinDir/windeployqt --debug --libdir $bind $bind --verbose 0 }
else { & $QtBinDir/windeployqt --release --libdir $bind $bind --verbose 0 }

echo 'Post-build process completed.'
cd $cwd # restore the last working directory
