if(NOT DEFINED SOURCES_DIR)
    message(FATAL_ERROR "Project sources directory not defined")
endif()

set(SOURCES
    ${SOURCES_DIR}/controller/menu_controller.cc
    ${SOURCES_DIR}/options/help_option.cc
    ${SOURCES_DIR}/options/command_option.cc
)
