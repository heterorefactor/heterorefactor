#!/bin/bash -

SIZE=$1
SIZE_PER_CHAR=$((SIZE / 26))

echo 26
for CHAR in {a..z}; do
    head -c ${SIZE_PER_CHAR} < /dev/zero | tr '\0' $CHAR
    echo
done
for CHAR in {a..z}; do
    head -c ${SIZE_PER_CHAR} < /dev/zero | tr '\0' $CHAR
done
echo
