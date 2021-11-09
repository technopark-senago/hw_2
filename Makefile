BUILD_DIR=build

CMAKE_DEBUG_FLAGS=-DENABLE_TESTS=ON -DENABLE_CPPCHECK=ON -DENABLE_SANITIZERS=ON
CMAKE_TEST_FLAGS=${CMAKE_DEBUG_FLAGS} -DENABLE_COVERAGE=ON
CMAKE_BUILD_PARALLEL=-DPARALLEL=ON
MAKE_FLAGS=-j$(shell nproc)

sequential: clean
	cmake -B ${BUILD_DIR} ${CMAKE_DEBUG_FLAGS}
	make ${MAKE_FLAGS} -C ${BUILD_DIR} scanner

parallel: clean
	cmake -B ${BUILD_DIR} ${CMAKE_DEBUG_FLAGS} ${CMAKE_BUILD_PARALLEL}
	make ${MAKE_FLAGS} -C ${BUILD_DIR} scanner

benchmark: clean
	cmake -B ${BUILD_DIR}
	make ${MAKE_FLAGS} -C ${BUILD_DIR} benchmark

benchmark-parallel: clean
	cmake -B ${BUILD_DIR} ${CMAKE_BUILD_PARALLEL}
	make ${MAKE_FLAGS} -C ${BUILD_DIR} benchmark

test:
	cmake -B ${BUILD_DIR} ${CMAKE_TEST_FLAGS}
	scan-build make ${MAKE_FLAGS} -C ${BUILD_DIR} all test

clean:
	rm -rf ${BUILD_DIR}
