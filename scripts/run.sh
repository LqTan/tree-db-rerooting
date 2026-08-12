#!/usr/bin/env bash
set -e

NAME="$1"

if [ -z "$NAME" ]; then
    echo "Usage: ./scripts/run.sh <name>"
    exit 1
fi

SOURCE=$(find . \
    -type f \
    -name "${NAME}.cpp" \
    ! -path "./build/*" \
    | head -n 1)

if [ -z "$SOURCE" ]; then
    echo "No source file '${NAME}.cpp' found"
    exit 1
fi

if ! grep -Eq '\bmain[[:space:]]*\(' "$SOURCE"; then
    echo "No main() found in '$SOURCE'"
    exit 1
fi

echo "Source: $SOURCE"

cmake --build build --target "$NAME"

EXECUTABLE=$(find build \
    -type f \
    -name "$NAME" \
    -perm -111 \
    | head -n 1)

if [ -z "$EXECUTABLE" ]; then
    echo "No executable '$NAME' found in build/"
    exit 1
fi

"$EXECUTABLE"