@echo off

echo [v_diff] Removing build\windows directory...
rd /s /q .\build\windows\ 2> NUL

echo [v_diff] Creating a new build\windows directory...
md .\build\windows\

echo [v_diff] Compiling v_diff source code...
gcc -o .\build\windows\v_diff.exe .\v_diff_0_1\src\*.c .\v_diff_0_1\src\**\*.c

echo [v_diff] Compilation success! You can now run build\windows\v_diff.exe
pause