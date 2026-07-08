#ifndef PAS_CORE_CLI_INPUTPARAMETERS_HH
#define PAS_CORE_CLI_INPUTPARAMETERS_HH

#include "pas-core/command/data/command_type.hh"

namespace pas::core::cli {

/**
 * @class InputParameters
 * @brief Параметры, передаваемые в программу при запуске.
 *
 */
struct InputParameters {
  // Тип команды
  command::CommandType command_type;
};
}  // namespace pas::core::cli

#endif
