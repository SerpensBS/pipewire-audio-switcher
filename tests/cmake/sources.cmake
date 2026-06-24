if(NOT DEFINED TEST_SOURCES_DIR)
    message(FATAL_ERROR "Project tests sources directory not defined")
endif()

set(TEST_SOURCES
    ${TEST_SOURCES_DIR}/tests/utility/process_executor_test.cc
    ${TEST_SOURCES_DIR}/tests/filesystem/file_reader_test.cc
    ${TEST_SOURCES_DIR}/tests/parser/toml_parser_test.cc
    ${TEST_SOURCES_DIR}/utility/temp_file.cc
)
