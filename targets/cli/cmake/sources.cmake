if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project sources directory not defined")
endif()

set(SOURCES ${SOURCES_DIR}/parser/argument_parser.cc)
