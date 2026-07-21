#include <chrono>
#include <cstdlib>
#include <iostream>
#include <system_error>

#include "commands/cycle_sink_command.hh"
#include "commands/get_sink_command.hh"
#include "controller/menu_controller.hh"
#include "options/command_option.hh"
#include "options/data/command_type.hh"
#include "options/help_option.hh"
#include "pas-core/adapter/wpctl_adapter.hh"

constexpr std::chrono::milliseconds kTimeout{1000};

auto main(int argc, char** argv) -> int {
  pas::cli::controller::MenuController menu{"Pipewire Audio Switcher"};

  pas::cli::options::HelpOption help_option;
  menu.RegisterCommand(help_option);
  pas::cli::options::CommandOption command_option;
  menu.RegisterCommand(command_option);

  auto input_parameters = menu.ProcessArguments(argc, argv);

  switch (input_parameters.command_type) {
    case pas::cli::options::CommandType::Help:
      menu.ShowHelp(std::cout);
      break;
    case pas::cli::options::CommandType::GetSink: {
      pas::core::adapter::WpctlApapter adapter;

      pas::cli::commands::GetSinkCommand::Execute(
          adapter, {.result_stream = std::cout, .error_stream = std::cerr}, kTimeout);
      break;
    }
    case pas::cli::options::CommandType::CycleSink: {
      pas::core::adapter::WpctlApapter adapter;

      pas::cli::commands::CycleSinkCommand::Execute(adapter, std::cerr, kTimeout);
      break;
    }
    case pas::cli::options::CommandType::Undefined:
    default:
      std::cerr << "Unexpected command";
      return std::make_error_code(std::errc::invalid_argument).value();
  }

  return EXIT_SUCCESS;
}
