#!/bin/zsh

# # # # # # # # # # # # # # # # # # # # # # # # # 
#  ./test-grover.sh -n 10 -i 100                #
#  -n <N>                                       #
#  -i <items>                                   #
#  -s <seed>                                    #
# # # # # # # # # # # # # # # # # # # # # # # # #

source test-random-generator.sh

# Default parameters: number of qubits, number of random solutions, and random seed
N=5
items=10
seed=37

# Get parameters from command line
while getopts n:i:s: flag
do
    case "${flag}" in
        n) N=${OPTARG};;
        i) items=${OPTARG};;
        s) seed=${OPTARG};;
    esac
done

echo "Running Grover's algorithm with $N qubits and $items random solutions, using the random seed $seed."

# generate solutions and programs
sols=$(generate_random_binaries $N $seed $items)
node generator.js "$sols"

# run the tests
DIR=$(pwd)
for i in {1..$items}
do
echo "Running test for grover-${N}-${i}.qw"
cd $DIR/grover-$N && qrat -no-banner grover-${N}-${i}.qw > output-${N}-${i}.txt 2>&1 << EOF
quit .
EOF
done

# analyze the results
maxTime=-1
maxStates=0
for i in {1..$items}
do
    solution=`sed -E -n '/^(Solution)/p' output-$N-$i.txt`
    if [ -z "$solution" ]; then
        echo "Error: No solution found in output-$N-$i.txt"
        continue
    fi
    prob=`cd $DIR/grover-$N && sed -E -n '/^(probability)/p' output-$N-$i.txt | head -n 1 | awk '{print $2}' | sed 's/,//'`
    if (( $(echo "$prob < 0.99" | bc -l) )); then
        echo "Probability $prob is not greater than 0.99"
    fi
    time=`cd $DIR/grover-$N && sed -E -n '/^(states)/p' output-$N-$i.txt | head -n 1 | awk -F'[()]' '{print $2}' | awk '{print $1}' | sed 's/ms//'`
    states=`cd $DIR/grover-$N && sed -E -n '/^(states)/p' output-$N-$i.txt | tail -n 1 | awk '{print $2}'`
    if [ "$time" -gt "$maxTime" ]; then
        maxTime="$time"
        maxStates="$states"
        echo "[Last] Output-$N-${i}.txt"
    fi
done

# clean up
rm -rf $DIR/grover-$N

# print the results
echo "[Longest] states: $maxStates in time ${maxTime}ms"