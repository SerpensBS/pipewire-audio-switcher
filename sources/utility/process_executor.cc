#include "process_executor.hh"

#include <chrono>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <string>
#include <vector>

namespace utility {
auto ProcessExecutor::Execute(const std::filesystem::path& command,
                              const std::vector<std::string>& args,
                              std::size_t max_return_buffer_size,
                              std::chrono::milliseconds timeout) const
    -> std::expected<std::string, Error> {
  return std::unexpected<Error>({});
}
}  // namespace utility
