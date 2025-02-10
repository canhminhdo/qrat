#!/bin/zsh

start=0
end=99

#  run 100 times
for i in {$start..$end}
do
qrat -no-banner "-random-seed=${i}" > output-${i}.txt 2>&1 << EOF
load teleport.qw .
quit .
EOF
done

# analyze the results
maxTime=-1
maxStates=0
for i in {$start..$end}
do
    solution=`sed -E -n '/^(Solution)/p' output-$i.txt`
    if [ -z "$solution" ]; then
        echo "Error: No solution found in output-$i.txt"
        continue
    fi
    line=`sed -E -n '/^(states)/p' output-$i.txt | tail -n 1`
    time=`echo $line | awk -F'[()]' '{print $2}' | awk '{print $1}' | sed 's/ms//'`
    states=`echo $line | awk '{print $2}'`
    if [ "$time" -gt "$maxTime" ]; then
        maxTime="$time"
        maxStates="$states"
        echo "[Last] Output-${i}.txt"
    fi
done

# clean up
rm -rf output-*.txt

# print the results
echo "[Longest] states: $maxStates in time ${maxTime}ms"