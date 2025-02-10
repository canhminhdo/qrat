#!/bin/zsh

# Function to generate random binary numbers
generate_random_binaries() {
    local N=$1
    local seed=$2
    local items=$3
    local max_number=$((2 ** N - 1))
  
    # Set the seed for reproducibility
    RANDOM=$seed

    # Initialize an empty array to store binary representations
    local binaries=()

    for i in {1..$items}
    do
        # Generate random number between 0 and max_number
        local random_number=$((RANDOM % (max_number + 1)))
        
        # Convert the number to binary and pad to N bits
        local binary=$(printf "%0${N}d" $(echo "obase=2; $random_number" | bc))
        
        # Output the binary representation
        binaries+=("$binary")
    done
    # Return the array (using echo to return the result)
    echo "${binaries[@]}"
}