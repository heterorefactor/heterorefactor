#!/bin/bash -

SIZE=$(echo $1 | awk '{ print int(($1/16)^(1/2)) }')
VALUERANGE=$((SIZE * 100))

echo $SIZE $SIZE $SIZE
shuf -i 1-$VALUERANGE -n $((SIZE * SIZE))
shuf -i 1-$VALUERANGE -n $((SIZE * SIZE))
