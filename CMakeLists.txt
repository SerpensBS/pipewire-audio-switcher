cmake_minimum_required(VERSION 4.3.0)
project(pipewire-audio-switcher)

set(PROJECTS_SOURCES_DIR targets)

# Настройки сборки проекта.
include(cmake/options.cmake)

add_subdirectory(${PROJECTS_SOURCES_DIR}/cli)
add_subdirectory(${PROJECTS_SOURCES_DIR}/core)
