#!/bin/bash

#Normalize our location
TOOLDIR=`dirname "$0"`
cd $TOOLDIR

cd ../build/

rm -rf *

mkdir build32
mkdir build64

cd build32
cmake ../.. -DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32
make

cd ../build64
cmake ../.. -DCMAKE_C_FLAGS=-m64 -DCMAKE_CXX_FLAGS=-m64
make