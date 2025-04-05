#!/bin/bash

# Run the DecafCompiler with all passed arguments
# ./cmake-build/src/DecafCompiler "$@"

valgrind --leak-check=full --track-origins=yes ./cmake-build/src/DecafCompiler "$@"

