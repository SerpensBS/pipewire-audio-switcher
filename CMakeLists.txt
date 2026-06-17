cmake_minimum_required(VERSION 4.3.0)
project(pipewire-audio-switcher)

# Подключаем исходники проекта.
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)

set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME} ${SOURCES_DIR}/main.cc ${SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE ${SOURCES_DIR})

include(cmake/external_libs.cmake)
target_link_libraries(${TARGET_NAME} PRIVATE tomlplusplus::tomlplusplus)

add_subdirectory(tests)
