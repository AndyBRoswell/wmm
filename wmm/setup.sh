# This script MUST be put at the root dir of this project.
# Running this script will DELETE all the dependencies downloaded, built and installed, and then REDOWNLOAD, REBUILD and REINSTALL them again.

cwd=$PWD
shsd=$cwd/$(dirname $0)
prjd=$shsd
${3rdd}=$shsd/3rd
buildd=${3rdd}/build
dld=${3rdd}/download
instd=${3rdd}/install/GCC

echo $cwd $shsd $prjd ${3rdd} $buildd $dld $instd
