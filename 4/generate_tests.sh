target="/home/menges/repos/advanced_algorithms/cmake-build-release/bin/generator"

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
)

start=$(date +%s%N)

for size in "${sizes[@]}"; do
  for i in {1..5}; do
    $target "$size" > "tests/${size}_${i}.dat"
  done
done

end=$(date +%s%N)
elapsed=$(((end - start)/100000))
echo "Generated tests in ${elapsed}ms"
