#!/bin/bash

INPUT_DIR="./public-tests/phase3-codegen/private/input"
OUTPUT_DIR="./public-tests/phase3-codegen/private/output"

run_test() {
    local input_file="$1"
    local filename
    filename=$(basename "$input_file")
    local expected_output_file="$OUTPUT_DIR/$filename.out"

    ./run.sh -t assembly "$input_file" -o out.s > out.tmp
    gcc -O0 -no-pie out.s -o exc

    ./exc > actual_output.txt
    exit_code=$?

    if [[ $exit_code -eq 0 ]] && diff -q actual_output.txt "$expected_output_file" > /dev/null; then
        echo "$filename: PASS"
    else
        echo "$filename: FAIL"
    fi
}

if [[ $# -eq 1 ]]; then
    input_file="$INPUT_DIR/$1"
    if [[ -f "$input_file" ]]; then
        run_test "$input_file"
    else
        echo "Test file '$1' not found in $INPUT_DIR"
        exit 1
    fi
else
    for input_file in "$INPUT_DIR"/*; do
        run_test "$input_file"
    done
fi
