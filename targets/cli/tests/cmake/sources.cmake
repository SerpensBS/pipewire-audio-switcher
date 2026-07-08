if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project tests sources directory not defined")
endif()

set(SOURCES ${SOURCES_DIR}/tests/parser/argument_parser_test.cc)
