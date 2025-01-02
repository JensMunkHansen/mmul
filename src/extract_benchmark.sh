#!/bin/bash

# Input and output files
input_file="benchmark_wasm_output.txt"
output_file="benchmark_wasm_results.csv"

# Header for the output file
echo "Program_Name;Benchmark;Time;CPU;Iterations" > "$output_file"

# Process the input file
while IFS= read -r line; do
    # Check for program name
    if [[ $line =~ ^Running ]]; then
        program_name=$(echo "$line" | awk '{print $2}')
    fi

    # Extract benchmark results
    if [[ $line =~ ^[a-zA-Z] ]]; then
        benchmark=$(echo "$line" | awk '{print $1}')
        time=$(echo "$line" | awk '{print $2 $3}')
        cpu=$(echo "$line" | awk '{print $4 $5}')
        iterations=$(echo "$line" | awk '{print $6}')
        # Append to the output file
        echo "$program_name;$benchmark;$time;$cpu;$iterations" >> "$output_file"
    fi
done < "$input_file"

echo "Results have been saved to $output_file"
