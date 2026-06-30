if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project tests sources directory not defined")
endif()

set(SOURCES
    ${SOURCES_DIR}/tests/filesystem/file_reader_test.cc
    ${SOURCES_DIR}/tests/parser/toml_parser_test.cc
    ${SOURCES_DIR}/utility/temp_file.cc
)
