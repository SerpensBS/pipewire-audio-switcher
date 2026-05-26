Проект предназначен для переключения источника звука в Linux средствами PipeWire.

## Зависимости

Сборка и запуск на более ранних версиях утилит не проверялась.

**Общие:**
- Make >= 4.4.1
- CMake >=4.3.0
- Clangd >=22.1.0

**Тесты:**
- GNU coreutils >= 9.11

## Сборка

```bash
cmake --preset=release
cmake --build --preset=release
```

## Запуск тестов

```bash
ctest --output-on-failure -C Release --test-dir build/release/tests
```
