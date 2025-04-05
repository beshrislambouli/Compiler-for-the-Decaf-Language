#!/bin/bash

# Check if a parameter (maximum number of files) is passed
max_files=$1
# Initialize a counter for processed files
file_count=0

# Directory paths
input_dir="./tests/phase1-scanner/public/input"
output_dir="./output/phase1-scanner/public/ans"
expected_dir="./tests/phase1-scanner/public/output"

# Create output directory if it doesn't exist
mkdir -p "$output_dir"

# Loop over each .dcf file in the input directory
for input_file in "$input_dir"/*.dcf; do

    if [ "$file_count" -ge "$max_files" ]; then
        break
    fi

    # Extract the file name without path and extension
    file_name=$(basename "$input_file" .dcf)

    # Construct the output file path
    output_file="$output_dir/ans_${file_name}.out"

    # Run the run.sh script with input and output files
    ./run.sh "$input_file" -t scan --output "$output_file"
    return_value=$?

    # Check if the file name contains "invalid"
    if [[ "$file_name" == *"invalid"* ]]; then
        # If the file name contains "invalid", check the return value of run.sh
        if [ "$return_value" -eq 0 ]; then
            echo "$file_name: FAIL"
        else
            echo "$file_name: PASS"
        fi
    else
        # For other files, run diff to compare the output with the expected file
        diff_output=$(diff "$output_file" "$expected_dir/${file_name}.out")

        # Print the result
        if [ -z "$diff_output" ]; then
            echo "$file_name: PASS"
        else
            echo "$file_name: FAIL"
        fi
    fi

    ((file_count++))
done
