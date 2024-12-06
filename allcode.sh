#!/bin/bash

# Set the root directory and output file
ROOT_DIR="."  # Change to your project root directory
OUTPUT_FILE="all_code.txt"     # Desired output file
EXCLUDE_DIR="build"            # Directory to exclude from the search

# Clear the output file if it exists
> "$OUTPUT_FILE"

# Find and append all .hpp and .cpp files, excluding the build directory and json.hpp
find "$ROOT_DIR" \( -path "$ROOT_DIR/$EXCLUDE_DIR" -prune \) -o \( \( -name "*.hpp" -o -name "*.cpp" \) -a ! -name "json.hpp" \) -type f -print | while read -r file; do
    echo "// File: $file" >> "$OUTPUT_FILE"
    cat "$file" >> "$OUTPUT_FILE"
    echo -e "\n\n" >> "$OUTPUT_FILE"  # Add spacing between files
done

echo "All headers and source files have been appended to $OUTPUT_FILE"
