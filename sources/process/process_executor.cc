#include "process_executor.hh"

#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/process.hpp>
#include <boost/process/v2/detail/config.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/stdio.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/system/detail/generic_category.hpp>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <expected>
#include <filesystem>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

namespace utility {
auto ProcessExecutor::Execute(const std::filesystem::path& command,
                              const std::vector<std::string>& args,
                              std::chrono::milliseconds timeout) const
    -> std::expected<std::string, Error> {
  boost::asio::io_context ioc;
  boost::asio::readable_pipe rx_pipe{ioc};

  std::string output;
  try {
    boost::process::process proc(
        ioc,
        command.string(),
        args,
        boost::process::process_stdio{.in = {}, .out = rx_pipe, .err = rx_pipe});

    boost::system::error_code rx_pipe_error_code;
    boost::asio::read(rx_pipe, boost::asio::dynamic_buffer(output), rx_pipe_error_code);

    proc.wait();
    int process_exit_code{proc.exit_code()};

    if (rx_pipe_error_code != boost::asio::error::eof) {
      throw boost::process::system_error(rx_pipe_error_code, output);
    }
    if (EXIT_SUCCESS != process_exit_code) {
      rx_pipe_error_code = {process_exit_code, boost::system::generic_category()};
      throw boost::process::system_error(rx_pipe_error_code, output);
    }
  } catch (const boost::process::system_error& exception) {
    return std::unexpected<Error>({.code = exception.code(), .message = std::move(output)});
  } catch (const std::exception& exception) {
    return std::unexpected<Error>(
        {.code = std::make_error_code(std::errc::io_error), .message = exception.what()});
  }

  return output;
}
}  // namespace utility
