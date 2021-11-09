CPPLINT_FLAGS="--recursive --quiet --headers=h --extensions=c,h --exclude=build/*"
CPPLINT_FILTER="-readability/casting,-whitespace/line_length,-build/include_subdir,-build/header_guard"
CPPLINT_FLAGS="${CPPLINT_FLAGS} --filter=${CPPLINT_FILTER}"
cpplint ${CPPLINT_FLAGS} .
