if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project sources directory not defined")
endif()

set(SOURCES
    ${SOURCES_DIR}/benchmarks/adapter/wpctl_adapter_benchmark.cc
    ${SOURCES_DIR}/benchmarks/filesystem/file_reader_benchmark.cc
    ${SOURCES_DIR}/benchmarks/parsers/toml_parser_benchmark.cc
    ${SOURCES_DIR}/benchmarks/process/process_executor_benchmark.cc
    ${SOURCES_DIR}/utility/temp_file.cc
)
