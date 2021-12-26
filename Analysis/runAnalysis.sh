#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m' # No Color
echo -e "${RED}Compiling files.${NC}"; echo ""
make clean
make

echo ""
echo -e "${GREEN}Compilation of files done.${NC}"; echo ""

echo -e "${GREEN}Analysing for pthread on different matrix and different thread.${NC}"

./parallelLUD

echo -e "${GREEN}Plotting graph for pthread.${NC}"

python3 plotGraph.py &

echo -e "${GREEN}Analysing for openMP on different matrix and different thread.${NC}"

./ompLUD

echo -e "${GREEN}Plotting graph for openMP.${NC}"

python3 plotGraphOMP.py
