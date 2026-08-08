#!/usr/bin/env bash
set -e

EXECUTABLE=$(find build -type f -name "tree_dp" -perm -111 | head -n 1)

if [ -z "$EXECUTABLE" ]; then
    echo "No executable 'tree_dp' found in build/"
    exit 1
fi

"$EXECUTABLE"