#!/bin/bash

if [[ $# -ne 5 ]] ; then
  echo "Supplied $# arguments! 5 needed:"
  echo "Usage: $0 <num_items> <coeff_range> <max_weight> <num_instances> <dest_folder>"
  exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

EXEC="$DIR/bin/generator"
if [[ ! -e "$EXEC" ]] ; then
  make  || { echo 'Make failed!' ; exit 2; }
fi
EXEC=$(printf %q "$EXEC")

NUM_ITEMS=$1
COEFF_RANGE=$2
TYPE=1
MAX_WEIGHT=$3
MAX_INSTANCE=$4
NUM_SERIES=1000

NAME_PREFIX="$5/test_${NUM_ITEMS}_"
NAME_SUFFIX=".txt"

echo "Generating..."
for i in $(seq 0 $MAX_INSTANCE); do
  eval "$EXEC" $NUM_ITEMS $COEFF_RANGE $TYPE $i $NUM_SERIES $MAX_WEIGHT | tail --lines=+2 > ${NAME_PREFIX}${i}${NAME_SUFFIX}
done
echo "Done"
