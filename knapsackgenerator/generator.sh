#!/bin/bash

if [[ $# -ne 4 ]] ; then
  echo "Supplied $# arguments! 4 needed:"
  echo "Usage: $0 <num_items> <coeff_range> <num_instances> <dest_folder>"
  exit 1
fi

EXEC=./bin/generator

if [[ ! -f $EXEC ]] ; then
  make  || { echo 'Make failed!' ; exit 2; }
fi

NUM_ITEMS=$1
COEFF_RANGE=$2
TYPE=1
MAX_INSTANCE=$3
NUM_SERIES=1000

NAME_PREFIX="$4/test_${NUM_ITEMS}_"
NAME_SUFFIX=".txt"

echo "Generating..."
for i in $(seq 0 $MAX_INSTANCE); do
  $EXEC $NUM_ITEMS $COEFF_RANGE $TYPE $i $NUM_SERIES | tail --lines=+2 > ${NAME_PREFIX}${i}${NAME_SUFFIX}
done
echo "Done"
