#!/bin/sh

echo "[ INFO ] Starting example1.c compilation to e1."
gcc example1.c -o e1 -lsha1 || { echo "[FAILED] Compilation of example1.c failed."; exit 1; }
echo "[  OK  ] Example1 compilation completed successfuly."

echo "[ INFO ] Starting example2.c compilation to e2."
gcc example2.c -o e2 -lsha1
echo "[  OK  ] Example2 compilation completed successfuly."
