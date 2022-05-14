# This script MUST be put at the root dir of this project.
# Running this script will DELETE all the dependencies downloaded, built and installed, and then REDOWNLOAD, REBUILD and REINSTALL them again.

cwd=$PWD
shsd=$cwd/$(dirname $0)
prjd=$shsd
thirdd=$shsd/3rd
buildd=$thirdd/build
dld=$thirdd/download
instd=$thirdd/install/GCC

# clear previous builds and dependencies
cd $prjd
if [ -d build ]; then
    rm -rf build
fi
if [ -d 3rd ]; then
    rm -rf 3rd
fi
mkdir 3rd
mkdir 3rd/build
mkdir 3rd/download
mkdir 3rd/install

# lib information:
    # 0. lib name
    # 1. download link
    # 2. extra cmake flags
    # 3. cmake prefix path
declare -A Lib
LibCount=6

Lib[0, 1]='mongo-c-driver-1.21.0'
echo ${Lib[0, 1]}
