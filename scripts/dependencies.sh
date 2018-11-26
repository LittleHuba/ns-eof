#!/bin/sh
sudo apt-get install openmp libopenmpi-dev
./configure --with-cc=mpicc --with-cxx=mpicxx --with-fc=0 --download-f2cblaslapack
make
sudo make install