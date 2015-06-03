#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: ./compileMacro.sh <macroName>"
  exit 1
fi

mkdir -p pdfDir
clang++ $1 $(root-config --cflags --libs) -Werror -Wall -O2 -o "${1/%.C/}.exe" 