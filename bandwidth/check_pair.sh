#!/bin/bash

if [ $# != 1 ]
then
    echo "CPUのコア数書いてくれや"
    exit 1
fi

for((i=0;i<$1;i++))
do
    for((j=i+1;j<$1;j++))
    do
        echo -n $i" "$j" "
        numactl -C $i,$j ./test 409600 2 1000 | grep "read band"
    done
done

