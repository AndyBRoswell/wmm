# This script MUST be put at the root dir of this project.
# Running this script will DELETE all the dependencies downloaded, built and installed, and then REDOWNLOAD, REBUILD and REINSTALL them again.

cwd=$PWD
shsd=$cwd/$(dirname $0)
prjd=$shsd
thirdd=$shsd/3rd
buildd=$thirdd/build
dld=$thirdd/download
instd=$thirdd/install/GCC

echo $cwd $shsd $prjd $thirdd $buildd $dld $instd
