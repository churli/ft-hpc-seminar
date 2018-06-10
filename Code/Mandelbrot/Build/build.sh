#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --target mandelbrot_exe -- -j 2

exit $?

#eof

