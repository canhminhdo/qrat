#!/bin/bash

DIR=$1
PROG=$(basename "$DIR")
shift
../src/qrat $DIR/${PROG}.qw -no-banner "$@" < /dev/null > $DIR/${PROG}.out 2>&1
diff $DIR/${PROG}.out $DIR/${PROG}.expected > /dev/null 2>&1
