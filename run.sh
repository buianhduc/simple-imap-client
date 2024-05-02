#!/bin/zsh

make clean
make
./fetchmail -u test@comp30023 -p pass -f "Test" retrieve unimelb-comp30023-2024.cloud.edu.au