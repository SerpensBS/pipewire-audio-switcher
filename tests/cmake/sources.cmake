if(NOT DEFINED TEST_SOURCES_DIR)
    message(FATAL_ERROR "Project tests sources directory not defined")
endif()

set(TEST_SOURCES ${TEST_SOURCES_DIR}/tests/utility/process_executor_test.cc)
