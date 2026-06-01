#include "utility/process_executor.hh"

#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <expected>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

namespace {
using namespace std::string_literals;

class ProcessExecutorTest : public ::testing::Test {
 protected:
  utility::ProcessExecutor executor_;
  static constexpr std::size_t kBufferSize{1024};
  static constexpr std::chrono::milliseconds kExecuteTimeout{200};
};

TEST_F(ProcessExecutorTest, GetSuccessCode) {
  auto response = executor_.Execute("true"s, {}, kBufferSize, kExecuteTimeout);

  ASSERT_TRUE(response.has_value());
  ASSERT_TRUE(response->empty());
}

TEST_F(ProcessExecutorTest, GetErrorCode) {
  auto response = executor_.Execute("false"s, {}, kBufferSize, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  EXPECT_EQ(response.error().code, std::errc::operation_not_permitted);
  ASSERT_TRUE(response.error().message.empty());
}

TEST_F(ProcessExecutorTest, GetSuccessMessage) {
  constexpr static std::string_view kExpectedResponse = "Expected echo message";

  auto response =
      executor_.Execute("echo"s, {std::string(kExpectedResponse)}, kBufferSize, kExecuteTimeout);

  ASSERT_TRUE(response.has_value());
  ASSERT_EQ(*response, kExpectedResponse);
}

TEST_F(ProcessExecutorTest, GetErrorMessage) {
  constexpr static std::string_view kExpectedErrorResponse = "Expected echo error message";

  auto response = executor_.Execute(
      "echo", {std::string(kExpectedErrorResponse), ">&2"s}, kBufferSize, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::operation_not_permitted);
  ASSERT_EQ(response.error().message, kExpectedErrorResponse);
}

TEST_F(ProcessExecutorTest, OutputBufferOverflow) {
  std::string expected_output_message = "Message"s;
  std::size_t message_buffer_size = expected_output_message.size();

  {
    auto response =
        executor_.Execute("echo"s, {expected_output_message}, message_buffer_size, kExecuteTimeout);
    ASSERT_TRUE(response.has_value());
  }

  {
    auto response = executor_.Execute(
        "echo"s, {expected_output_message}, --message_buffer_size, kExecuteTimeout);
    ASSERT_FALSE(response.has_value());
    ASSERT_EQ(response.error().code, std::errc::message_size);
  }
}

TEST_F(ProcessExecutorTest, ProcessNotFound) {
  auto response = executor_.Execute("wrong_application_name"s, {}, kBufferSize, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  EXPECT_EQ(response.error().code, std::errc::no_such_file_or_directory);
}

TEST_F(ProcessExecutorTest, ProcessTimeoutFailure) {
  constexpr std::chrono::milliseconds kProcessRunDuration{500};

  auto response = executor_.Execute(
      "sleep"s, {std::to_string(kProcessRunDuration.count())}, kBufferSize, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::timed_out);
}

TEST_F(ProcessExecutorTest, PrematureExitDueToTimeout) {
  constexpr std::chrono::milliseconds kProcessRunDuration{500};
  const auto deadline = std::chrono::steady_clock::now() + kProcessRunDuration;

  auto response = executor_.Execute(
      "sleep"s, {std::to_string(kProcessRunDuration.count())}, kBufferSize, kExecuteTimeout);

  // Проверяем выход из функции по достижении timeout'а.
  EXPECT_GT(deadline, std::chrono::steady_clock::now());

  // Проверяем код ошибки.
  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::timed_out);
}

TEST_F(ProcessExecutorTest, ProcessKillAfterTimeout) {
  /**
   * @brief Получает список PID для процесса.
   * @param process_name имя процесса
   * @return список PID
   * @throw std::runtime_error В случае ошибки при вызове процесса pgrep
   */
  auto get_pids = [this](const std::string& process_name) -> std::set<std::string> {
    auto pgrep_result = executor_.Execute("pgrep"s, {process_name}, kBufferSize, kExecuteTimeout);

    if (not pgrep_result.has_value()) {
      throw std::runtime_error("pgrep not returned result");
    }

    return pgrep_result.value() | std::views::split('\n') |
           std::views::transform([](const auto& value) -> std::string {
             return std::string(value.cbegin(), value.cend());
           }) |
           std::ranges::to<std::set<std::string>>();
  };

  const std::string kProcessName = "sleep"s;

  // Получаем список ID процессов sleep до вызова тестового метода:
  auto pgrep_before_run_result = get_pids(kProcessName);

  constexpr std::chrono::milliseconds kProcessRunDuration{100};
  constexpr std::chrono::milliseconds kProcessRunTimeout{1};
  auto response = executor_.Execute(
      kProcessName, {std::to_string(kProcessRunDuration.count())}, kBufferSize, kProcessRunTimeout);

  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::timed_out);

  // Получаем список ID процессов sleep после вызова тестового метода и сравниваем оба списка:
  auto pgrep_after_run_result = get_pids(kProcessName);
  ASSERT_EQ(pgrep_before_run_result, pgrep_after_run_result);
}
}  // namespace
