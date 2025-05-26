#!/usr/bin/env bash
generator="../cmake-build-release/bin/Release/generator.exe"
target="../cmake-build-release/bin/Release/hungarian_algorithm.exe"

# Your commands here

sizes=(
  21000
  22000
  23000
  24000
  25000
  26000
  27000
  28000
  29000
  30000
)

echo "n,time,loop_iters,relaxations,heap_ops,result"

for i in {1..10}; do
        for size in "${sizes[@]}"; do
        (
            $generator "$size" >"tests/test_${size}"

            result=$($target <"tests/test_${size}")

            echo "$size,$result"
        ) &
        done
    wait
done
