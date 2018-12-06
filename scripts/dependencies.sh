#!/bin/sh

#Install petsc
PETSC_VERSION="petsc-3.5.4"
wget http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/${PETSC_VERSION}.tar.gz
tar -xzf ${PETSC_VERSION}.tar.gz
cd ${PETSC_VERSION}
./configure --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --download-f2cblaslapack --prefix=/usr/local
make
make PETSC_DIR=`pwd` PETSC_ARCH=arch-linux2-c-debug test
sudo make install