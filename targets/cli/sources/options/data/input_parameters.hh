#ifndef PAS_CLI_OPTIONS_DATA_INPUTPARAMETERS_HH
#define PAS_CLI_OPTIONS_DATA_INPUTPARAMETERS_HH

#include "options/data/command_type.hh"

namespace pas::cli::options {
/**
 * @class InputParameters
 * @brief Параметры, передаваемые в программу при запуске.
 *
 */
struct InputParameters {
  // Тип команды
  pas::cli::options::CommandType command_type;
};
}  // namespace pas::cli::options

#endif
