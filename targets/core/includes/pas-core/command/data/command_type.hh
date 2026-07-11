#ifndef PAS_CORE_COMMANDS_DATA_COMMANDTYPE_HH
#define PAS_CORE_COMMANDS_DATA_COMMANDTYPE_HH

#include <cstdint>

namespace pas::core::command {

/**
 * @brief Тип команды.
 */
enum class CommandType : uint8_t { Undefined = 0, GetSink };

}  // namespace pas::core::command

#endif
