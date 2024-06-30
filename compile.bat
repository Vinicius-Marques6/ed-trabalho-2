@echo off

echo Compilando...

gcc -Wall -pedantic -g -o main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/avl.c ./*.c

if %ERRORLEVEL% neq 0 (
    echo Houve um erro na compilação.
    pause
    exit /B 1
)

START main.exe municipios.json