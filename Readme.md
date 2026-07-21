***ДИСКЛЕЙМЕР***: *это учебно-развлекательный проект, созданный для веселья, а также для того, чтобы закрыть одну из проблем на моей домашней системе - переключение источника звука одним нажатием клавиши и получению сведений об активном аудиоустройстве. Не все технические решения в нем принимались исходя из максимальной эффективности конечного бинарника, однако данный код полностью удовлетворяет моим личным требованиям к производительности и трудится на моей домашней машине каждый день.*

---
# PipeWire Audio Switcher

Проект предназначен для переключения источника звука в Linux средствами PipeWire.

## Зависимости

Сборка и запуск на более ранних версиях утилит не проверялась.

**Общие:**
- Make >= 4.4.1
- CMake >=4.3.0
- Clangd >=22.1.0
- Git >= 2.54.0

**Тесты/Бенчмарки:**
- GNU coreutils >= 9.11

**Приложение:**
- PipeWire >= 1.6.7

## Сборка

```bash
cmake --preset=release
cmake --build --preset=release
```

## Запуск тестов

```bash
build/release/targets/core/tests/pas-core-tests
build/release/targets/cli/tests/pas-cli-tests
```

## Запуск benchmark'а

```bash
build/release/targets/core/benchmarks/pas-core-benchmark
build/release/targets/cli/benchmarks/pas-cli-benchmark
```

## Запуск исполняемого файла
```bash
build/release/targets/cli/pipewire-audio-switcher --help
```

## CI

В папке `scripts` лежат скрипты для сборки и запуска различных частей проекта - это может пригодиться для интеграции с командами IDE или процессом CI/CD. Справку каждого скрипта можно получить по команде `--help`

- **build.sh** - скрипт для сборки цели
- **run_benchmarks.sh** - собирает и запускает все benchmark'и проекта
- **run_tests.sh** - собирает и запускает все тесты проекта

Примеры использования:
```bash
scripts/build.sh --target pas-cli --preset release
scripts/run_tests.sh --preset debug
scripts/run_benchmarks.sh --preset release
```
