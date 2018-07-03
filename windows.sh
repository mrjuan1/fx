#!/bin/bash
gcc *.c -o a.exe -DTITLE="\"FX\"" -O3 -pipe -m64 -ftree-vectorize -ffast-math -funroll-loops -lmingw32 -lSDL2main -lSDL2 -lm -mwindows
