#!/bin/sh

mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make Test
cd test
./Test