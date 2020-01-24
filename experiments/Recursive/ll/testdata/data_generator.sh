#!/bin/bash -

SIZE=$(($1 - 1))
VALUERANGE=$((SIZE * 100))

echo $SIZE
shuf -i 1-$VALUERANGE -n $SIZE
