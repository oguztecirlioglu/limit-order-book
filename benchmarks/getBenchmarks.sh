if [ -z "$1" ]; then
    echo "No argument supplied for name of benchmark files."
    exit 1
fi

fileName = $1

perf stat -e cache-references,cache-misses,cycles,instructions,branch-misses -o "$1_perf.txt" ../build/clob
