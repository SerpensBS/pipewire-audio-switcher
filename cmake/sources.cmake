if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project sources directory not defined")
endif()

set(SOURCES
    ${SOURCES_DIR}/utility/process_executor.cc
    ${SOURCES_DIR}/filesystem/file_reader.cc
    ${SOURCES_DIR}/parser/toml_parser.cc
)
