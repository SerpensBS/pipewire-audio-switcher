Проект предназначен для переключения источника звука в Linux средствами PipeWire.

## Зависимости

Сборка и запуск на более ранних версиях утилит не проверялась.

**Общие:**
- Make >= 4.4.1
- CMake >=4.3.0
- Clangd >=22.1.0
- Git >= 2.54.0

**Тесты:**
- GNU coreutils >= 9.11

## Сборка

```bash
cmake --preset=release
cmake --build --preset=release
```

## Запуск тестов

```bash
build/release/targets/core/tests/pas-core-tests
```

## Запуск benchmark'а
```bash
build/release/targets/core/benchmarks/pas-core-benchmark
```

## CI
В папке `scripts` лежат скрипты для сборки и запуска различных частей проекта - это может пригодиться для интеграции с командами IDE или процессом CI/CD. Справку каждого скрипта можно получить по команде `--help`

- **build.sh** - скрипт для сборки цели
- **run_benchmarks.sh** - собирает и запускает все benchmark'и проекта
- **run_tests.sh** - собирает и запускает все тесты проекта
