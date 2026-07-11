#ifndef PAS_CLI_MENU_MENUCONTROLLER_HH
#define PAS_CLI_MENU_MENUCONTROLLER_HH

#include <boost/program_options/options_description.hpp>
#include <map>
#include <ostream>
#include <string>

#include "options/data/input_parameters.hh"
#include "options/interfaces/ioption.hh"

namespace pas::cli::controller {

class MenuController {
 public:
  explicit MenuController(const std::string& app_description);

  void RegisterCommand(options::IOption& option);

  auto ProcessArguments(int argc, const char* const* argv) const -> options::InputParameters;

  void ShowHelp(std::ostream& stream) const;

 private:
  boost::program_options::options_description description_;
  std::map<std::string, options::IOption&> commands_;
};
}  // namespace pas::cli::controller

#endif
