#!/bin/bash

if [ "$1" == "legal" ]; then
    directory="./public-tests/phase2-semantics/private/legal"
    expected_exit_code=0
elif [ "$1" == "illegal" ]; then
    directory="./public-tests/phase2-semantics/private/illegal"
    expected_exit_code=1
else
    echo "Usage: $0 [legal|illegal]"
    exit 1
fi

for file in "$directory"/*.dcf; do
    file_name=$(basename "$file")
    ./run.sh "$file" -t inter > out.tmp
    exit_code=$?

    if [ "$1" == "legal" ] && [ $exit_code -eq 0 ]; then
        echo "$file_name: PASS"
    elif [ "$1" == "illegal" ] && [ $exit_code -ne 0 ]; then
        echo "$file_name: PASS"
    else
        echo "$file_name: FAIL"
    fi
done
