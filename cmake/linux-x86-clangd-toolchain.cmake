# Сообщаем CMake, что это toolchain файл
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Toolchain file для Clang.
set(TOOLCHAIN_DIRECTORY /usr/bin)

# Компиляторы
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIRECTORY}/clang)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIRECTORY}/clang++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

# Утилиты
set(CMAKE_OBJCOPY ${TOOLCHAIN_DIRECTORY}/llvm-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIRECTORY}/llvm-objdump)
set(CMAKE_SIZE ${TOOLCHAIN_DIRECTORY}/llvm-size)
