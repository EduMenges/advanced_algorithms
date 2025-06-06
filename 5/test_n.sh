source ./paths.sh

samples=$((20))
increment=$((1000))
max_weight=255
capacity=$((max_weight * increment * samples / 2))

echo "n,$header"

for ((i = 1; i <= samples; i++)); do
    size=$((i*increment))
    for j in {1..5}; do
    (
        result=$($generator "$size" "$capacity" "$max_weight" | $target)
        echo "$size,$result"
    ) &
    done
    wait
done
