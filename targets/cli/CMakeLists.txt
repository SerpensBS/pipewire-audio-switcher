cmake_minimum_required(VERSION 4.3.0)
project(pas-cli)

# Подключаем исходники проекта.
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
include(${PROJECT_SOURCE_DIR}/cmake/sources.cmake)

set(TARGET_NAME ${PROJECT_NAME}-obj)
add_library(${TARGET_NAME} OBJECT ${SOURCES})
add_library(pas::cli ALIAS ${TARGET_NAME})

# Настройки сборки проекта.
target_link_libraries(${TARGET_NAME} PRIVATE pas::project::properties)
set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# Внутренние директории.
target_include_directories(${TARGET_NAME} PUBLIC ${SOURCES_DIR})

# Подключаем внешние библиотеки.
target_link_libraries(${TARGET_NAME} PUBLIC pas::core)
target_link_libraries(${TARGET_NAME} PRIVATE CLI11::CLI11)

# Создаем исполняемый файл.
add_executable(${PROJECT_NAME} ${SOURCES_DIR}/main.cc)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCES_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE pas::cli)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES CXX_EXTENSIONS OFF
               OUTPUT_NAME ${CMAKE_PROJECT_NAME} # Собираем бинарник с именем
                                                 # корневого проекта
)

add_subdirectory(tests)
add_subdirectory(benchmarks)
