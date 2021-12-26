#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color
echo -e "${RED}Compiling files.${NC}"; echo ""
make clean
make

echo ""
echo -e "${GREEN}Compilation of files done.${NC}"; echo ""

echo -e "${RED}-----Executing serial LUD on file named \"input.txt\".${NC}"
./serialLUD input.txt

echo -e "${BLUE}Input matrix is: ${NC}"
cat input.txt

echo -e "${BLUE}Lower Matrix is: ${NC}"
cat lowerMatrix

echo -e "${BLUE}Upper Matrix is: ${NC}"
cat upperMatrix

echo -e "${BLUE}PA-LU is: ${NC}"
./verification
cat verificationMatrix

echo -e "${RED}-----Executing parallel LUD on file named \"input.txt\".${NC}"
./parallelLUD input.txt

echo -e "${BLUE}Input matrix is: ${NC}"
cat input.txt

echo -e "${BLUE}Lower Matrix is: ${NC}"
cat lowerMatrix

echo -e "${BLUE}Upper Matrix is: ${NC}"
cat upperMatrix

echo -e "${BLUE}PA-LU is: ${NC}"
./verification
cat verificationMatrix

echo -e "${RED}-----Executing OpenMP LUD on file named \"input.txt\".${NC}"
./ompLUD input.txt

echo -e "${BLUE}Input matrix is: ${NC}"
cat input.txt

echo -e "${BLUE}Lower Matrix is: ${NC}"
cat lowerMatrix

echo -e "${BLUE}Upper Matrix is: ${NC}"
cat upperMatrix

echo -e "${BLUE}PA-LU is: ${NC}"
./verification
cat verificationMatrix

echo -e "${YELLOW}LU Decomposition is calculated using 3 programs i.e. serial, parallel and openMP.${NP}"

