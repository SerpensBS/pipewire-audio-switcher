#include "commands/cycle_sink_command.hh"

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

class CycleSinkCommandTest : public ::testing::Test {
 protected:
  static constexpr std::chrono::milliseconds kTimeout{1000};

  MockWpctlAdapter adapter_;
};
}  // namespace

TEST_F(CycleSinkCommandTest, ExecuteSuccess) {
  std::vector<pas::core::adapter::Sink> sinks = {
      {.id = 1, .name = "First Sink", .volume = 1.0, .is_active = false},
      {.id = 2, .name = "Second Sink", .volume = 1.0, .is_active = true}};

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{sinks}));

  EXPECT_CALL(adapter_, SetSink(sinks.front().id, kTimeout))
      .WillOnce(::testing::Return(std::expected<void, pas::core::utility::Error>{}));

  std::stringstream error_stream;
  pas::cli::commands::CycleSinkCommand::Execute(adapter_, error_stream, kTimeout);

  EXPECT_TRUE(error_stream.view().empty());
}

TEST_F(CycleSinkCommandTest, OnlyOneSinkExit) {
  std::vector<pas::core::adapter::Sink> sinks = {
      {.id = 1, .name = "First Sink", .volume = 1.0, .is_active = true}};

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{sinks}));

  std::stringstream error_stream;
  pas::cli::commands::CycleSinkCommand::Execute(adapter_, error_stream, kTimeout);

  EXPECT_TRUE(error_stream.view().empty());
}

TEST_F(CycleSinkCommandTest, CallWpctlFailure) {
  pas::core::utility::Error error{
      .code = std::make_error_code(std::errc::no_such_file_or_directory),
      .message = "No such file or directory"};

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{
              std::unexpected<pas::core::utility::Error>{error}}));

  std::stringstream error_stream;
  pas::cli::commands::CycleSinkCommand::Execute(adapter_, error_stream, kTimeout);

  EXPECT_FALSE(error_stream.view().empty());
  EXPECT_TRUE(error_stream.view().contains(error.message));
}

TEST_F(CycleSinkCommandTest, SetSinkFailue) {
  std::vector<pas::core::adapter::Sink> sinks = {
      {.id = 1, .name = "First Sink", .volume = 1.0, .is_active = false},
      {.id = 2, .name = "Second Sink", .volume = 1.0, .is_active = true}};

  pas::core::utility::Error error{
      .code = std::make_error_code(std::errc::no_such_file_or_directory),
      .message = "No such file or directory"};

  EXPECT_CALL(adapter_, GetSinks(kTimeout))
      .WillOnce(::testing::Return(
          std::expected<std::vector<pas::core::adapter::Sink>, pas::core::utility::Error>{sinks}));

  EXPECT_CALL(adapter_, SetSink(sinks.front().id, kTimeout))
      .WillOnce(::testing::Return(std::expected<void, pas::core::utility::Error>{
          std::unexpected<pas::core::utility::Error>{error}}));

  std::stringstream error_stream;
  pas::cli::commands::CycleSinkCommand::Execute(adapter_, error_stream, kTimeout);

  EXPECT_FALSE(error_stream.view().empty());
  EXPECT_TRUE(error_stream.view().contains(error.message));
}
