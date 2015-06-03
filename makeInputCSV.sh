#!/bin/bash

if [ $# -ne 1 ]
then 
  echo "Usage: ./makeInputCSV.sh <fileName>"
  exit 1
fi


ls /Users/cfmcginn/Projects/TMVASpread/statsPull/csvDir/*.csv > $1