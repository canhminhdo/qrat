#!/bin/bash

PROG=$1
../src/qrat $PROG/${PROG}.qw -no-banner > $PROG/${PROG}.out 2>&1
diff $PROG/${PROG}.out $PROG/${PROG}.expected > /dev/null 2>&1