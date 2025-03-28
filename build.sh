#!/bin/bash
set -e 

mkdir -p build 
cd build
cmake .. -DPYTHON_EXECUTABLE=$(which python3)
make
cd ..

RUN="false"
DEBUG="false"

while [[ $# -gt 0 ]]; do
  case "$1" in
    -r|--run)
      RUN="true"
      shift
      ;;
    -d|--debug)
      DEBUG="true"
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
elif [ "$DEBUG" == "true" ]; then
  echo -e "Starting LLDB with Shkyera Engine"
  lldb ./build/bin/ShkyeraEngine
fi
