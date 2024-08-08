#!/bin/bash

set -ue

brk=0

TEST_DIR="test"
RUN_DIR=$(mktemp -d)
BINARY="bin/dynamic_connectivity_example"
OFFLINE_BINARY="bin/offline"
GENERATOR="$TEST_DIR/gen.py"
BAD_INPUT="$TEST_DIR/bad.in"

make test

echo "Run dir : $RUN_DIR"

while [ $brk -eq 0 ]
    do
    echo 0
    python3 "$GENERATOR" > "$RUN_DIR/in.in"
    echo -e "---\nOffline"
    /bin/time -v "$OFFLINE_BINARY" > "$RUN_DIR/offline.out" < "$RUN_DIR/in.in"
    echo -e "---\nMine"
    /bin/time -v "$BINARY" > "$RUN_DIR/mine.out" < "$RUN_DIR/in.in"
    diff "$RUN_DIR/offline.out" "$RUN_DIR/mine.out" > /dev/null
    if [ $? -ne 0 ]
    then
        brk=1
        cp "$RUN_DIR/in.in" "$BAD_INPUT"
        echo "Outputs differ, bad input saved to $BAD_INPUT"
    fi
done

rm -r RUN_DIR