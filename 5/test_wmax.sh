source ./paths.sh

samples=$((10))
n=$((10000))
increment=$((100))
capacity=$((n * samples * increment / 2 ))

echo "max_weight,$header"

for ((i = 1; i < samples; i++)); do
    max_weight=$((i*increment))
    for j in {1..3}; do
    (
        result=$($generator "$n" "$capacity" "$max_weight" | $target)
        echo "$max_weight,$result"
    ) &
    done
    wait
done
