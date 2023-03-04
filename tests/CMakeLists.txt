set(
    TEST_DIRS
    bitonic_test
)

foreach(TEST_DIR ${TEST_DIRS})
    add_subdirectory(${TEST_DIR})
endforeach()

list(APPEND TESTS bitonic_test)

set(TESTS "${TESTS}" PARENT_SCOPE)