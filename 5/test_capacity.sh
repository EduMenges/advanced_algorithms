source ./paths.sh

n=$((10000))
max_capacity=$((n*255))
samples=$((10))
increment=$((max_capacity / (samples)))

echo "capacity,${header}"

for ((i=1; i < samples; i++)); do
  for j in {1..5}; do
    (
    capacity=$((i*increment))
    result=$($generator "$n" "$capacity" | $target)
    echo "$capacity,$result"
    )&
  done
  wait
done
