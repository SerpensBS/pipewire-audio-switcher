#include "utility/process_executor.hh"

#include <gtest/gtest.h>

#include <cstddef>
#include <string>
#include <string_view>
#include <system_error>

namespace {
using namespace std::string_literals;

class ProcessExecutorTest : public ::testing::Test {
 protected:
  utility::ProcessExecutor executor_;
  static constexpr std::size_t kBufferSize = 1024;
};

TEST_F(ProcessExecutorTest, GetSuccessCode) {
  auto response = executor_.Execute("true"s, {}, kBufferSize);

  ASSERT_TRUE(response.has_value());
  ASSERT_TRUE(response->empty());
}

TEST_F(ProcessExecutorTest, GetErrorCode) {
  auto response = executor_.Execute("false", {}, kBufferSize);

  ASSERT_FALSE(response.has_value());
  EXPECT_EQ(response.error().code, std::errc::operation_not_permitted);
  ASSERT_TRUE(response.error().message.empty());
}

TEST_F(ProcessExecutorTest, GetSuccessMessage) {
  constexpr static std::string_view expected_response = "Expected echo message";

  auto response = executor_.Execute("echo", {std::string(expected_response)}, kBufferSize);

  ASSERT_TRUE(response.has_value());
  ASSERT_EQ(*response, expected_response);
}

TEST_F(ProcessExecutorTest, GetErrorMessage) {
  constexpr static std::string_view expected_error_response = "Expected echo error message";

  auto response =
      executor_.Execute("echo", {std::string(expected_error_response), ">&2"s}, kBufferSize);

  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::operation_not_permitted);
  ASSERT_EQ(response.error().message, expected_error_response);
}

TEST_F(ProcessExecutorTest, OutputBufferOverflow) {
  std::string expected_output_message = "Message";
  std::size_t message_buffer_size = expected_output_message.size();

  {
    auto response = executor_.Execute("echo", {expected_output_message}, message_buffer_size);
    ASSERT_TRUE(response.has_value());
  }

  {
    auto response = executor_.Execute("echo", {expected_output_message}, --message_buffer_size);
    ASSERT_FALSE(response.has_value());
    ASSERT_EQ(response.error().code, std::errc::message_size);
  }
}

TEST_F(ProcessExecutorTest, ProcessNotFound) {
  auto response = executor_.Execute("wrong_application_name", {}, kBufferSize);

  ASSERT_FALSE(response.has_value());
  EXPECT_EQ(response.error().code, std::errc::no_such_file_or_directory);
}
}  // namespace
