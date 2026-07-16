#!/bin/bash
set -euo pipefail

PRESET_NAME=""

# Парсим входящие аргументы.
parse_args() {
	local -A args=()
	local preset=""

	while [[ $# -gt 0 ]]; do
		case "$1" in
		--*=*)
			local key="${1%%=*}"
			local value="${1#*=}"
			args["$key"]="$value"
			shift
			;;
		--*)
			local key="$1"
			args["$key"]="$2"
			shift 2
			;;
		--help)
			echo "Скрипт для запуска проекта ожидает следующие параметры:"
			echo "--preset - конфигурация debug/release"
			echo ""
			echo "Пример вызова: './run_tests.sh --preset release'"
			exit 0
			;;
		*)
			echo "Неизвестный аргумент: $1"
			exit 1
			;;
		esac
	done

	preset="${args[--preset]:-}"

	# Проверка обязательных аргументов
	local required=("--preset")
	local missing=()

	for arg in "${required[@]}"; do
		[[ -z "${args[$arg]:-}" ]] && missing+=("$arg")
	done

	if [[ ${#missing[@]} -gt 0 ]]; then
		echo "Ошибка: обязательные аргументы не указаны: ${missing[*]}"
		echo "Использование: $0 --preset=<тип>"
		exit 1
	fi

	PRESET_NAME="$preset"
}
# Парсинг аргументов.
parse_args "$@"

# Собираем проект.
./scripts/build.sh --preset=${PRESET_NAME} --target=pas-core-tests
./scripts/build.sh --preset=${PRESET_NAME} --target=pas-cli-tests

# Запускаем тесты.
./build/${PRESET_NAME}/targets/core/tests/pas-core-tests --gtest_color=yes
./build/${PRESET_NAME}/targets/cli/tests/pas-cli-tests --gtest_color=yes
