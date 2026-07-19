#include "process/process_executor.hh"

#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/readable_pipe.hpp>
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
#include <future>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "utility/error.hh"

namespace pas::core::process {
auto ProcessExecutor::Execute(const std::filesystem::path& command,
                              const std::vector<std::string>& args,
                              std::chrono::milliseconds timeout) const
    -> std::expected<std::string, utility::Error> {
  boost::asio::io_context ioc;
  boost::asio::readable_pipe rx_pipe{ioc};
  std::string output;

  try {
    boost::process::process process(
        ioc,
        command.string(),
        args,
        boost::process::process_stdio{.in = {}, .out = rx_pipe, .err = rx_pipe});

    auto read_pipe_future = std::async(std::launch::async, [&]() -> boost::system::error_code {
      boost::system::error_code rx_pipe_error_code;
      boost::asio::read(rx_pipe, boost::asio::dynamic_buffer(output), rx_pipe_error_code);
      return rx_pipe_error_code;
    });

    auto future_status = read_pipe_future.wait_for(timeout);

    if (std::future_status::timeout == future_status) {
      process.terminate();
      throw boost::process::system_error(std::make_error_code(std::errc::timed_out), output);
    }

    const int process_exit_code{process.wait()};
    const auto rx_pipe_error_code = read_pipe_future.get();

    if (EXIT_SUCCESS != process_exit_code) {
      return std::unexpected<utility::Error>(
          {.code = {process_exit_code, boost::system::generic_category()},
           .message = std::move(output)});
    }
    if (rx_pipe_error_code != boost::asio::error::eof) {
      throw boost::process::system_error(rx_pipe_error_code, output);
    }
  } catch (const boost::process::system_error& exception) {
    return std::unexpected<utility::Error>({.code = exception.code(), .message = exception.what()});
  } catch (const std::exception& exception) {
    return std::unexpected<utility::Error>(
        {.code = std::make_error_code(std::errc::io_error), .message = exception.what()});
  }

  return output;
}
}  // namespace pas::core::process
