#include <benchmark/benchmark.h>

#include <array>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <map>
#include <string>
#include <string_view>

#include "controller/menu_controller.hh"
#include "options/command_option.hh"
#include "options/data/command_type.hh"
#include "options/data/input_parameters.hh"
#include "options/help_option.hh"

namespace {

class MenuControllerBenchmark : public ::benchmark::Fixture {};

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(MenuControllerBenchmark, Create)(benchmark::State& state) {
  constexpr auto kArgs = std::to_array<const char*>({"Benchmark", "--command=get-sink"});
  for (auto current_state : state) {
    pas::cli::controller::MenuController menu_controller("Benchmark");

    pas::cli::options::HelpOption help_option;
    menu_controller.RegisterCommand(help_option);
    pas::cli::options::CommandOption command_option;
    menu_controller.RegisterCommand(command_option);

    auto input_parameters = menu_controller.ProcessArguments(kArgs.size(), kArgs.data());
    benchmark::DoNotOptimize(input_parameters);
  }
}

// NOLINTNEXTLINE(cert-err58-cpp)
BENCHMARK_F(MenuControllerBenchmark, CreateNative)(benchmark::State& state) {
  constexpr auto kArgs = std::to_array<const char*>({"Benchmark", "--command=get-sink"});
  static const std::map<const std::string_view, pas::cli::options::CommandType> command_type_map{
      {"get-sink", pas::cli::options::CommandType::GetSink}};

  for (auto current_state : state) {
    boost::program_options::options_description desc{"Benchmark"};
    desc.add_options()("help,h", "Help screen")("command,c",
                                                boost::program_options::value<std::string>(),
                                                "Command to be executed by the application");

    boost::program_options::command_line_parser parser(kArgs.size(), kArgs.data());
    parser.options(desc);
    parser.style(boost::program_options::command_line_style::unix_style |
                 boost::program_options::command_line_style::allow_long_disguise);

    boost::program_options::variables_map variables_map;
    boost::program_options::store(parser.run(), variables_map);
    boost::program_options::notify(variables_map);

    pas::cli::options::InputParameters input_parameters{};

    if (variables_map.contains("help")) {
      input_parameters.command_type = pas::cli::options::CommandType::Help;
    } else if (variables_map.contains("command")) {
      const auto& value = variables_map.at("command");

      input_parameters.command_type = command_type_map.at(value.as<const std::string&>());
    }

    benchmark::DoNotOptimize(input_parameters);
  }
}
}  // namespace
