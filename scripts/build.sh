#!/bin/bash
set -euo pipefail

PRESET_NAME=""
TARGET_NAME=""

# Парсим входящие аргументы.
parse_args() {
	local -A args=()
	local preset=""
	local target=""

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
			echo "Скрипт для сборки проекта ожидает следующие параметры:"
			echo "--target - цель сборки"
			echo "--preset - конфигурация debug/release"
			echo ""
			echo "Пример вызова: './build.sh --target pipewire-audio-switcher --preset release'"
			exit 0
			;;
		*)
			echo "Неизвестный аргумент: $1"
			exit 1
			;;
		esac
	done

	preset="${args[--preset]:-}"
	target="${args[--target]:-}"

	# Проверка обязательных аргументов
	local required=("--preset" "--target")
	local missing=()

	for arg in "${required[@]}"; do
		[[ -z "${args[$arg]:-}" ]] && missing+=("$arg")
	done

	if [[ ${#missing[@]} -gt 0 ]]; then
		echo "Ошибка: обязательные аргументы не указаны: ${missing[*]}"
		echo "Использование: $0 --preset=<тип> --target=<цель>"
		exit 1
	fi

	PRESET_NAME="$preset"
	TARGET_NAME="$target"
}
# Парсинг аргументов.
parse_args "$@"

# Конфигурация проекта.
echo "Конфигурация:"
cmake --preset ${PRESET_NAME} .

# Копируем compile_commands.json.
echo ""
echo "Копируем compile_commands.json:"
ln -sf ./build/${PRESET_NAME}/compile_commands.json ./compile_commands.json

# Собираем проект.
echo ""
echo "Сборка проекта ${TARGET_NAME}:"
cmake --build --preset ${PRESET_NAME} --target ${TARGET_NAME}
