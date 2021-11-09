export GTEST_COLOR=1
make test

readonly GCOVR_FLAGS="--root lib --exclude .*/tests/.*"
if [ "$1" = "html" ]; then
  gcovr ${GCOVR_FLAGS} --html --html-details --output build/coverage.html .
else
  gcovr ${GCOVR_FLAGS} --xml --output build/coverage.xml .
fi
