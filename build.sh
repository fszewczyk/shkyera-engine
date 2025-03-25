#!/bin/bash
set -e 

mkdir -p build 
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make
cd ..

RUN="false"

while [[ $# -gt 0 ]]; do
  case "$1" in
    -r|--run)
      RUN="true"
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit
      ;;
  esac
done
shift "$(($OPTIND -1))"

if [ "$RUN" == "true" ]; then
  echo -e "Starting Shkyera Engine"
  ./build/bin/ShkyeraEngine
fi
