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

directory="."
all_files=""

readarray -t files < <(find "$directory" -type f \( -name "*.c" \))

for file in "${files[@]}"; do
  gcc -c -fpic $file -o $file.o -I.
  check_error "Error compiling position independent code."
  all_files+="$file.o "
done

gcc -shared -o lib$lib_name.so $all_files
check_error "Error creating shared object."

for file in "${files[@]}"; do
  rm -r $file.o
done

echo "Compilation of lib$lib_name.so finished."

sudo cp lib$lib_name.so /usr/lib
check_error "Error copying shared object to /usr/lib." 

rm -r lib$lib_name.so
sudo cp $lib_name.h /usr/include
check_error "Error copying header file to /usr/include."

echo "Installation of lib$lib_name finished."

