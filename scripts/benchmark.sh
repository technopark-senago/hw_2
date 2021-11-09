EXPECTED_ARGS=2
if [ "$#" -ne "$EXPECTED_ARGS" ]; then
    echo "Expected two arguments [PATH_TO_DIR] [WORD]"
else
    for d in $(ls -rc $1) ; do
        ./build/benchmark "$1/$d" "$2"
    done
fi
