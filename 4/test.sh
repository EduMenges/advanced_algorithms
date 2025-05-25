#!/usr/bin/env bash
generator="/home/menges/repos/advanced_algorithms/cmake-build-gcc/bin/generator"
target="/home/menges/repos/advanced_algorithms/cmake-build-gcc/bin/hungarian_algorithm"

# Your commands here

sizes=(
    1000
    2000
    3000
    4000
    5000
    6000
    7000
    8000
    9000
    10000
    11000
    12000
    13000
    14000
    15000
    16000
    17000
    18000
    19000
    20000
)

echo "n,time,result"

for i in {1..10}; do
        for size in "${sizes[@]}"; do
        (
            $generator $size >"tests/test_${size}"

            start=$(date +%s%N)

            result=$($target <"tests/test_${size}")

            end=$(date +%s%N)
            elapsed=$(((end - start) / 1000000))

            echo "$size,$elapsed,$result"
        ) &
        done
    wait
done
