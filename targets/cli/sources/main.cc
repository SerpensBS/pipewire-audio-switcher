#include <iostream>

#include "parser/argument_parser.hh"

auto main(int argc, char** argv) -> int {
  std::cout << "Argc = " << argc;
  return 0;
  pas::cli::parser::CLIParser cli_parser;
  auto input_parameters = cli_parser.ParseParameters(argc, argv);

  return 0;
}
