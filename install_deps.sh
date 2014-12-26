#!/bin/bash

if [[ -z $1 ]] ; then
    echo "ERROR: specify prefix for deps install"
    exit 1;
fi

PREFIX=$1

# install liboauth
wget http://downloads.sourceforge.net/project/liboauth/liboauth-1.0.3.tar.gz
tar xvf liboauth-1.0.3.tar.gz
cd liboauth-1.0.3/
autoreconf -iv
./configure --prefix=$PREFIX
make
make install

# install json-c
cd ..
git clone https://github.com/json-c/json-c.git
cd json-c
sh autogen.sh
./configure --prefix=$PREFIX
make
make install
