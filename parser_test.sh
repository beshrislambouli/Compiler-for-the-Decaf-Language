#!/bin/bash

# Directory containing the test files
TEST_DIR="./tests/phase1-parser/public/legal"

# Iterate over all .dcf files in the directory
for file in "$TEST_DIR"/*.dcf; do
    # Run the file with ./run.sh
    ./run.sh "$file" -t parse
    
    # Capture the return value
    return_code=$?

    # Check the return code and print the result
    if [ $return_code -eq 0 ]; then
        echo "PASS: $file"
    else
        echo "FAIL: $file"
    fi
done
