#!/bin/sh

#Install mpi
sudo apt-get install -y libopenmpi-dev openmpi-bin libboost-test-dev

#Install petsc
PETSC_VERSION="petsc-3.5.4"
if [[ ! -d ${PETSC_VERSION} ]]; then
    wget http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/${PETSC_VERSION}.tar.gz
    tar -xzf ${PETSC_VERSION}.tar.gz
    cd ${PETSC_VERSION}
    ./configure --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --download-f2cblaslapack --prefix=/usr/local
    make
    #TODO can we make this path relative?
    make PETSC_DIR=/home/travis/build/LittleHuba/ns-eof/${PETSC_VERSION} PETSC_ARCH=arch-linux2-c-debug test
else
    cd ${PETSC_VERSION}
fi

sudo make install