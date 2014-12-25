#!/bin/bash

# install liboauth
wget http://downloads.sourceforge.net/project/liboauth/liboauth-1.0.3.tar.gz
tar xvf liboauth-1.0.3.tar.gz
cd liboauth-1.0.3/
autoreconf -iv
./configure
make
make install --prefix="/usr"

# install json-c
cd ..
git clone https://github.com/json-c/json-c.git
cd json-c
sh autogen.sh
./configure
make
make install --prefix="/usr"
