#!/bin/bash

# Check if at least one argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <executable> [args...]"
    exit 1
fi

# Extract the executable name
executable="$1"
shift # Remove the executable from the argument list
args=("$@") # Store remaining arguments in an array

# Loop through the traces and run the specified executable with the remaining arguments for each trace
for trace in traces/*.dpc.gz; do
    zcat "$trace" | "$executable" "$@"
    echo "Command executed: $executable ${args[*]} <$trace"
    echo "-----------------------------------------"
done
