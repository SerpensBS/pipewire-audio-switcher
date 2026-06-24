cmake_minimum_required(VERSION 4.3.0)
project(pipewire-audio-switcher-tests)

# Добавляем исходники.
set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/sources)
include(${CMAKE_SOURCE_DIR}/cmake/sources.cmake)

set(TEST_SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)

# Определяем целевую сборку.
set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME} ${SOURCES} ${TEST_SOURCES})

# Определяем видимость директорий.
target_include_directories(
    ${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/sources
                           ${CMAKE_SOURCE_DIR}/sources
)

# Загружаем и собираем внешние библиотеки.
include(${CMAKE_SOURCE_DIR}/cmake/external_libs.cmake)
include(../cmake/external_libs.cmake)

# Подключаем библиотеки.
target_link_libraries(
    ${TARGET_NAME} PRIVATE gtest gtest_main tomlplusplus::tomlplusplus
                           Boost::process
)

# Подключаем тесты.
include(CTest)
include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
