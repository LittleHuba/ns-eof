#!/usr/bin/env bash

if command -v module 1>/dev/null 2>&1; then
    # Load modules here if needed
    module unload boost
    module load boost/1.61_icc
fi

export CC=gcc
export CXX=g++
export BOOST_ROOT=${BOOST_BASE}

#BUILD_TYPE="Debug"
#BUILD_TYPE="RelWithDebInfo"
BUILD_TYPE="Release"

if [[ "$1" != "" ]]; then
    BUILD_TYPE="$1"
fi

SRC_DIR="$(pwd)"
BUILD_DIR="${SRC_DIR}/cmake-build-${CC}"
TARGET="ns"

mkdir -p ${BUILD_DIR}
pushd ${BUILD_DIR}
cmake -DPETSC_DIR=${PETSC_DIR} -DPETSC_ARCH=${PETSC_ARCH} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -G "CodeBlocks - Unix Makefiles" ${SRC_DIR}
cmake --build ${BUILD_DIR} --target ${TARGET} -- -j 2
popd
ln -sf ${BUILD_DIR}/src/${TARGET} ${SRC_DIR}/.

#eof
