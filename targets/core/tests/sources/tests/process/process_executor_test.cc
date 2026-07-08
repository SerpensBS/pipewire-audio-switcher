#include "pas-core/process/process_executor.hh"

#include <gtest/gtest.h>

#include <chrono>
#include <format>
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
  pas::core::process::ProcessExecutor executor_;
  static constexpr std::chrono::milliseconds kExecuteTimeout{200};
};

TEST_F(ProcessExecutorTest, GetSuccessCode) {
  auto response = executor_.Execute("/bin/true"s, {}, kExecuteTimeout);

  ASSERT_TRUE(response.has_value());
  ASSERT_TRUE(response->empty());
}

TEST_F(ProcessExecutorTest, GetErrorCode) {
  auto response = executor_.Execute("/bin/false"s, {}, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  EXPECT_EQ(response.error().code, std::errc::operation_not_permitted);
  ASSERT_TRUE(response.error().message.empty());
}

TEST_F(ProcessExecutorTest, GetSuccessMessage) {
  constexpr static std::string_view kExpectedResponse = "Expected echo message\n";
  // Вычитаем символ переноса строки.
  constexpr static std::string_view kArgMessage = {kExpectedResponse.begin(),
                                                   kExpectedResponse.size() - 1};

  auto response = executor_.Execute("/bin/echo"s, {std::string(kArgMessage)}, kExecuteTimeout);

  ASSERT_TRUE(response.has_value());
  ASSERT_EQ(*response, kExpectedResponse);
}

TEST_F(ProcessExecutorTest, GetErrorMessage) {
  constexpr static std::string_view kExpectedErrorResponse = "Expected echo error message\n";
  // Вычитаем символ переноса строки.
  constexpr static std::string_view kArgMessage = {kExpectedErrorResponse.begin(),
                                                   kExpectedErrorResponse.size() - 1};

  auto response = executor_.Execute(
      "/bin/sh", {"-c", std::format("/bin/echo '{}' && exit 1", kArgMessage)}, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::operation_not_permitted);
  ASSERT_EQ(response.error().message, kExpectedErrorResponse);
}

TEST_F(ProcessExecutorTest, ProcessNotFound) {
  auto response = executor_.Execute("wrong_application_name"s, {}, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  EXPECT_EQ(response.error().code, std::errc::no_such_file_or_directory);
}

TEST_F(ProcessExecutorTest, ProcessTimeoutFailure) {
  constexpr std::chrono::seconds kProcessRunDuration{1};

  auto response = executor_.Execute(
      "/bin/sleep"s, {std::to_string(kProcessRunDuration.count())}, kExecuteTimeout);

  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::timed_out);
}

TEST_F(ProcessExecutorTest, PrematureExitDueToTimeout) {
  constexpr std::chrono::seconds kProcessRunDuration{2};
  const auto deadline = std::chrono::steady_clock::now() + kProcessRunDuration;

  auto response = executor_.Execute(
      "/bin/sleep"s, {std::to_string(kProcessRunDuration.count())}, kExecuteTimeout);

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
    auto pgrep_result = executor_.Execute(
        "/bin/sh"s, {"-c", std::format("/bin/pgrep {} || true", process_name)}, kExecuteTimeout);

    if (not pgrep_result.has_value()) {
      throw std::runtime_error("pgrep not returned result");
    }

    return pgrep_result.value() | std::views::split('\n') |
           std::views::transform([](const auto& value) -> std::string {
             return std::string(value.cbegin(), value.cend());
           }) |
           std::ranges::to<std::set<std::string>>();
  };

  const std::string kProcessName = "/bin/sleep"s;

  // Получаем список ID процессов sleep до вызова тестового метода:
  auto pgrep_before_run_result = get_pids(kProcessName);

  // Запускаем sleep.
  constexpr std::chrono::seconds kProcessRunDuration{2};
  constexpr std::chrono::milliseconds kProcessRunTimeout{100};
  auto response = executor_.Execute(
      kProcessName, {std::to_string(kProcessRunDuration.count())}, kProcessRunTimeout);

  // Проверяем что процесс упал по таймауту.
  ASSERT_FALSE(response.has_value());
  ASSERT_EQ(response.error().code, std::errc::timed_out);

  // Получаем список ID процессов sleep после вызова тестового метода и сравниваем оба списка.
  // Если они идентичны - значит зависший процесс был убит корректно.
  auto pgrep_after_run_result = get_pids(kProcessName);
  ASSERT_EQ(pgrep_before_run_result, pgrep_after_run_result);
}
}  // namespace
