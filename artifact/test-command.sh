#!/bin/zsh
/usr/bin/time -al qcheck << EOF
load teleport.qw .
quit .
EOF