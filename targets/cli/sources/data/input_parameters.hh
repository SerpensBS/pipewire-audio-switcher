#ifndef PAS_CLI_DATA_INPUTPARAMETERS_HH
#define PAS_CLI_DATA_INPUTPARAMETERS_HH

#include "pas-core/command/data/command_type.hh"

namespace pas::cli {

/**
 * @class InputParameters
 * @brief Параметры, передаваемые в программу при запуске.
 *
 */
struct InputParameters {
  // Тип команды
  pas::core::command::CommandType command_type;
};
}  // namespace pas::cli

#endif
