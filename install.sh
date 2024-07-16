# This script compiles and install the http_client library. 
# Use it only if your Operating System is Linux and you have the gcc compiler installed.

if [[ "$1" == "" ]]; then
  echo "Usage: ./install.sh <library_name>"
  exit 1
fi

directory="."
all_files=""

readarray -t files < <(find "$directory" -type f \( -name "*.c" \))

for file in "${files[@]}"; do
  gcc -c -fpic $file -o $file.o -I.
  if [[ $? != 0  ]]; then
    echo "Error compiling position independent code."
    exit 1
  fi
  all_files+="$file.o "
done

gcc -shared -o lib$1.so $all_files
if [[ $? != 0 ]]; then
  echo "Error compiling shared object."
  exit 1
fi

for file in "${files[@]}"; do
  rm -r $file.o
done

echo "Compilation of $1.so finished."

sudo cp lib$1.so /usr/lib

if [[ $? != 0 ]]; then
  echo "Error copying shared object to /usr/lib."
  exit 1
fi

sudo cp $1.h /usr/include

if [[ $? != 0 ]]; then
  echo "Error copying header file to /usr/include."
  exit 1
fi

echo "Installation of $1.so finished."

