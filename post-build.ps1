param( # command-line args
    [string] $QtBinaryDir,
    [string] $BuildDir = 'out/build',
    [string] $BuildType = 'Release'
)

set usage -option constant -value @"

Usage
    Copy the essentials to the build directory.

    Parameters:
        QtBinaryDir
            The binary directory of Qt. This is determined by the path you installed the Qt binaries and the compiler (MSVC / G++) you choosed.
        BuildDir (default: out/build)
            The build directory of this program.
        BuildType (default: Release)
            Debug, Release, RelWithDebInfo, MinSizeRel.
        
    Examples:
        .\post-build.ps1 -QtBinaryDir C:\Qt\6.4.0\msvc2019_64\bin -BuildDir out/build/x64-Debug -BuildType Debug
        ./post-build -qtbinarydir c:/qt/6.4.0/msvc2019_64/bin -builddir out/build/x64-release

"@

set SupportedBuildType -option constant -value @('Debug','Release','RelWithDebInfo','MinSizeRel')

if ([string]::IsNullOrEmpty($QtBinaryDir)) {
    echo 'The binary directory of Qt specified by the argument QtBinaryDir is illegal.'
    echo $usage
    exit
}
if ((test-path $BuildDir) -eq $false) {
    mkdir $BuildDir
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
$buildd = $BuildDir                 # binary directory of this software

# post-build begins here
echo 'Post-Build: Copying essentials to build directory ...'
# Enter the directory of script (aka the root directory of this software) for the convenience of subsequent operations. This also avoided wrong behaviors when the working directory of the moment when this script was run wasn't the directory it locates.
cd $shsd

# Python related code, themes and test data
cp -r py $buildd/py
cp -r src/thm $buildd/src/thm
cp -r test $buildd/test

# 3rd libraries
cp $3rdd/mongo-c-driver-1.22.1/$BuildType/bin/bson-1.0.dll $buildd/bson-1.0.dll
cp $3rdd/mongo-c-driver-1.22.1/$BuildType/bin/mongoc-1.0.dll $buildd/mongoc-1.0.dll
cp $3rdd/mongo-cxx-driver-r3.6.7/$BuildType/bin/bsoncxx.dll $buildd/bsoncxx.dll
cp $3rdd/mongo-cxx-driver-r3.6.7/$BuildType/bin/mongocxx.dll $buildd/mongocxx.dll
# Windeployqt doesn't support RelWithDebInfo and MinSizeRel yet. Use Release provisionally.
if ($BuildType -ieq 'debug') { & $QtBinaryDir/windeployqt --debug --libdir $buildd $buildd --verbose 0 }
else { & $QtBinaryDir/windeployqt --release --libdir $buildd $buildd --verbose 0 }

echo 'Post-build process completed.'
cd $cwd # restore the last working directory
