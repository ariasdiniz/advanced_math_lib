#!/bin/bash

# This script compiles and install the amath library. 
# Use it only if your Operating System is Unix-based and you have the gcc compiler installed.

check_error () {
  if [[ $? != 0 ]]; then
    echo $1
    exit 1
  fi
}

lib_name='amath'

make -s clean
make -s

echo "Compilation of lib$lib_name.so finished."

sudo cp lib$lib_name.so /usr/lib
check_error "Error copying shared object to /usr/lib." 

rm -r lib$lib_name.so
sudo cp $lib_name.h /usr/include
check_error "Error copying header file to /usr/include."

sudo cp $lib_name /usr/local/bin
check_error "Error copying executable to /usr/local/bin."

make -s clean

echo "Installation of lib$lib_name finished."

