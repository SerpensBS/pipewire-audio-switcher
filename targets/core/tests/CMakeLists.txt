cmake_minimum_required(VERSION 4.3.0)
project(pas-core-tests)

set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)

# Определяем целевую сборку.
set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME} ${SOURCES})

# Определяем видимость директорий.
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/sources)

# Загружаем и собираем внешние библиотеки.
include(${PROJECT_SOURCE_DIR}/cmake/external_libs.cmake)

# Подключаем библиотеки.
target_link_libraries(${TARGET_NAME} PRIVATE gtest gtest_main pas-core)

# Подключаем тесты.
include(CTest)
include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
