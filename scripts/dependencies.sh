#!/bin/sh

#Install mpi
sudo apt-get install libopenmpi-dev openmpi-bin

#Install petsc
wget http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-3.7.7.tar.gz
tar -xzf petsc-3.7.7.tar.gz
cd petsc-3.7.7
./configure --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --download-f2cblaslapack --prefix=/usr/lib
make
#TODO can we make this path relative?
make PETSC_DIR=/home/travis/build/LittleHuba/ns-eof/petsc-3.7.7 PETSC_ARCH=arch-linux2-c-debug test
sudo make install