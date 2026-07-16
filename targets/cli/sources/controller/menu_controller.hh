#ifndef PAS_CLI_MENU_MENUCONTROLLER_HH
#define PAS_CLI_MENU_MENUCONTROLLER_HH

#include <boost/program_options/options_description.hpp>
#include <map>
#include <ostream>
#include <string>
#include <type_traits>

#include "options/base_option.hh"
#include "options/data/input_parameters.hh"
#include "options/interfaces/ioption.hh"

namespace pas::cli::controller {

/**
 * @class MenuController
 * @brief Контроллер консольного меню.
 *
 */
class MenuController {
 public:
  /**
   * @param app_description Описание приложения.
   */
  explicit MenuController(const std::string& app_description);

  /**
   * @brief Регистрация команды.
   *
   * @tparam T Тип данных значения опции
   * @param option Опция
   */
  template <typename ValueT>
  void RegisterCommand(options::BaseOption<ValueT>& option);

  /**
   * @brief Обработка аргументов командной строки.
   *
   * @param argc Количество аргументов
   * @param argv Аргументы.
   *
   * @return Параметры аргументов
   */
  auto ProcessArguments(int argc, const char* const* argv) const -> options::InputParameters;

  /**
   * @brief Меню помощи.
   *
   * @param stream Поток, в который будет выведена справочная информация
   */
  void ShowHelp(std::ostream& stream) const;

 private:
  // Опции
  boost::program_options::options_description description_;
  // Словарь опций.
  std::map<std::string, options::IOption&> commands_;
};

template <typename T>
void MenuController::RegisterCommand(options::BaseOption<T>& option) {
  if constexpr (std::is_void<T>()) {
    description_.add_options()(
        std::format("{},{}", option.GetName(), option.GetShortCommand()).c_str(),
        option.GetDescription().c_str());
  } else {
    description_.add_options()(
        std::format("{},{}", option.GetName(), option.GetShortCommand()).c_str(),
        boost::program_options::value<T>(),
        option.GetDescription().c_str());
  }

  commands_.emplace(option.GetName(), option);
}
}  // namespace pas::cli::controller

#endif
