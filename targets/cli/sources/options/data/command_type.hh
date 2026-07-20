#ifndef PAS_CLI_OPTIONS_DATA_COMMANDTYPE_HH
#define PAS_CLI_OPTIONS_DATA_COMMANDTYPE_HH

#include <cstdint>

namespace pas::cli::options {

/**
 * @brief Тип команды.
 */
enum class CommandType : uint8_t { Undefined = 0, Help, GetSink, CycleSink };

}  // namespace pas::cli::options

#endif
