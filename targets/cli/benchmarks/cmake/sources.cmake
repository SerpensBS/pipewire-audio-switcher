if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project sources directory not defined")
endif()

set(SOURCES ${SOURCES_DIR}/benchmarks/controller/menu_controller_benchmark.cc)
