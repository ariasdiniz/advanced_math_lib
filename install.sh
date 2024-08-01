#!/bin/bash

# This script compiles and install the http_client library. 
# Use it only if your Operating System is Linux and you have the gcc compiler installed.

check_error () {
  if [[ $? != 0 ]]; then
    echo $1
    exit 1
  fi
}

if [[ "$1" == "" ]]; then
  echo "Usage: ./install.sh <library_name>"
  exit 1
fi

directory="."
all_files=""

readarray -t files < <(find "$directory" -type f \( -name "*.c" \))

for file in "${files[@]}"; do
  gcc -c -fpic $file -o $file.o -I.
  check_error "Error compiling position independent code."
  all_files+="$file.o "
done

gcc -shared -o lib$1.so $all_files
check_error "Error creating shared object."

for file in "${files[@]}"; do
  rm -r $file.o
done

echo "Compilation of $1.so finished."

sudo cp lib$1.so /usr/lib
check_error "Error copying shared object to /usr/lib." 

rm -r lib$1.so
sudo cp $1.h /usr/include
check_error "Error copying header file to /usr/include."

echo "Installation of $1.so finished."

