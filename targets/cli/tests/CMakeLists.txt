cmake_minimum_required(VERSION 4.3.0)
project(pas-cli-tests)

set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)

# Определяем целевую сборку.
set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME} ${SOURCES})

# Определяем видимость директорий.
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/sources)

# Настройки сборки проекта.
target_link_libraries(${TARGET_NAME} PRIVATE pas::project::properties)
set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# Загружаем и собираем внешние библиотеки.
include(${PROJECT_SOURCE_DIR}/cmake/external_libs.cmake)

# Подключаем библиотеки.
target_link_libraries(${TARGET_NAME} PRIVATE gtest gtest_main pas::cli)

# Подключаем тесты.
include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
