#!/bin/sh

rm -rf vendor
mkdir -p vendor/

cd vendor
git clone https://github.com/emoon/minifb.git
cd minifb
mkdir build
cd build
cmake ../
make
