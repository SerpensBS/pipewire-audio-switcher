cmake_minimum_required(VERSION 4.3.0)
project(pipewire-audio-switcher)

set(TARGET_NAME ${PROJECT_NAME})

set(PROJECTS_SOURCES_DIR targets)
add_executable(${TARGET_NAME} ${PROJECTS_SOURCES_DIR}/main.cc)

target_include_directories(${TARGET_NAME} PRIVATE ${SOURCES_DIR})

target_link_libraries(${TARGET_NAME} PRIVATE pas-core)

add_subdirectory(${PROJECTS_SOURCES_DIR}/core)
