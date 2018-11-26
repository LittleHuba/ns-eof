#!/bin/sh

mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make boost_test
cd test
./boost_test