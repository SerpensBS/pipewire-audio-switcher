cmake_minimum_required(VERSION 4.3.0)
project(pas-core)

# Подключаем исходники проекта.
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)

set(TARGET_NAME ${PROJECT_NAME})
add_library(${TARGET_NAME} STATIC ${SOURCES})
add_library(pas::core ALIAS ${TARGET_NAME})

# Настройки сборки проекта.
target_link_libraries(${TARGET_NAME} PRIVATE pas::project::properties)
set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# Открываем директорию с исходниками.
set(INCLUDES_DIR ${PROJECT_SOURCE_DIR}/includes)
target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDES_DIR})

# Внутренние директории.
target_include_directories(
    ${TARGET_NAME} PRIVATE ${SOURCES_DIR} ${INCLUDES_DIR}/${TARGET_NAME}
)

# Подключаем внешние библиотеки.
include(${PROJECT_SOURCE_DIR}/cmake/external_libs.cmake)
target_link_libraries(${TARGET_NAME} PRIVATE tomlplusplus::tomlplusplus)

add_subdirectory(tests)
