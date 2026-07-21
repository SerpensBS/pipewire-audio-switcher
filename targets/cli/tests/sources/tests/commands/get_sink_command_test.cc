#include "commands/get_sink_command.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <expected>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/adapter/interfaces/iwpctl_adapter.hh"
#include "pas-core/utility/error.hh"

namespace {

class MockWpctlAdapter : public pas::core::adapter::IWpctlApapter {
 public:
  // NOLINTNEXTLINE(modernize-use-trailing-return-type)
  MOCK_METHOD((std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>),
              GetSinks,
              (std::chrono::milliseconds timeout),
              (const, override));

  // NOLINTNEXTLINE(modernize-use-trailing-return-type)
  MOCK_METHOD((std::expected<void, pas::core::utility::Error>),
              SetSink,
              (std::size_t, std::chrono::milliseconds timeout),
              (const, override));
};

class GetSinkCommandTest : public ::testing::Test {
 protected:
  static constexpr std::chrono::milliseconds kTimeout{1000};

  MockWpctlAdapter adapter_;
};
}  // namespace

TEST_F(GetSinkCommandTest, ExecuteSuccess) {
  std::vector<pas::core::adapter::Sink> sinks = {
      {.id = 1, .name = "First Sink", .volume = 1.0, .is_active = false},
      {.id = 2, .name = "Second Sink", .volume = 1.0, .is_active = true}};

  const auto& active_sink = sinks.back();

  std::stringstream json_result_stream;
  json_result_stream
      << R"({"id":)" << active_sink.id << R"(,"name":")"
      << active_sink.name
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
      << R"(","volume":)" << active_sink.volume * 100 << "}\n";

  const std::string expected_result = json_result_stream.str();

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{sinks}));

  std::stringstream error_stream;
  std::stringstream result_stream;
  pas::cli::commands::GetSinkCommand::Streams streams{.result_stream = result_stream,
                                                      .error_stream = error_stream};

  pas::cli::commands::GetSinkCommand::Execute(adapter_, streams, kTimeout);

  EXPECT_TRUE(error_stream.view().empty());
  EXPECT_EQ(expected_result, result_stream.str());
}

TEST_F(GetSinkCommandTest, CallWpctlFailure) {
  pas::core::utility::Error error{
      .code = std::make_error_code(std::errc::no_such_file_or_directory),
      .message = "no such file or directory"};

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{
              std::unexpected<pas::core::utility::Error>{error}}));

  std::stringstream error_stream;
  std::stringstream result_stream;
  pas::cli::commands::GetSinkCommand::Streams streams{.result_stream = result_stream,
                                                      .error_stream = error_stream};

  pas::cli::commands::GetSinkCommand::Execute(adapter_, streams, kTimeout);

  EXPECT_TRUE(error_stream.view().contains(error.message));
  EXPECT_TRUE(result_stream.view().empty());
}

TEST_F(GetSinkCommandTest, NoActiveSinkFailure) {
  std::vector<pas::core::adapter::Sink> sinks = {
      {.id = 1, .name = "First Sink", .volume = 1.0, .is_active = false},
      {.id = 2, .name = "Second Sink", .volume = 1.0, .is_active = false}};

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{sinks}));

  std::stringstream error_stream;
  std::stringstream result_stream;
  pas::cli::commands::GetSinkCommand::Streams streams{.result_stream = result_stream,
                                                      .error_stream = error_stream};

  pas::cli::commands::GetSinkCommand::Execute(adapter_, streams, kTimeout);

  EXPECT_FALSE(error_stream.view().empty());
  EXPECT_TRUE(result_stream.view().empty());
}
