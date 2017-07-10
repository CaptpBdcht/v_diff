#!/bin/bash

echo [v_diff] Removing build/linux directory...
rm -r ./build/linux/ 2> /dev/null

echo [v_diff] Creating a new build/linux directory...
mkdir -p ./build/linux/

echo [v_diff] Compiling v_diff source code...
gcc -o ./build/linux/v_diff ./v_diff_0_1/src/*.c ./v_diff_0_1/src/**/*.c

echo [v_diff] Compilation success! You can now run build/linux/v_diff
read -rsp $'Press enter to continue...\n'