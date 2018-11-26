#!/bin/sh

#Install mpi
sudo apt-get install libopenmpi-dev

#Install petsc
wget http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-3.7.7.tar.gz
tar -xzf petsc-3.7.7.tar.gz
cd petsc-3.7.7
./configure --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --download-f2cblaslapack
make
sudo make install