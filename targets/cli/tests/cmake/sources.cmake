if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project tests sources directory not defined")
endif()

set(SOURCES
    ${SOURCES_DIR}/tests/controller/menu_controller_test.cc
    ${SOURCES_DIR}/tests/options/help_option_test.cc
    ${SOURCES_DIR}/tests/options/command_option_test.cc
)
