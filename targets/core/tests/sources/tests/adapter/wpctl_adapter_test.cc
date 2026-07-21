#include "pas-core/adapter/wpctl_adapter.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/process/interfaces/iprocess_executor.hh"
#include "pas-core/process/process_executor.hh"
#include "pas-core/utility/error.hh"

namespace {
class MockProcessExecutor : public pas::core::process::IProcessExecutor {
 public:
  // NOLINTNEXTLINE(modernize-use-trailing-return-type)
  MOCK_METHOD((std::expected<std::string, pas::core::utility::Error>),
              Execute,
              (const std::filesystem::path& command,
               const std::vector<std::string>& args,
               std::chrono::milliseconds timeout),
              (const, override));

  static const std::string_view kMockWpctlStatusResponse;
};

class WpctlAdapterTest : public ::testing::Test {
 public:
  static void SetUpTestSuite() {
    pas::core::process::ProcessExecutor executor;
    auto result = executor.Execute("/bin/wpctl", {"help"}, kTimeout);

    if (not result.has_value() and result.error().code == std::errc::no_such_file_or_directory) {
      wpctl_is_installed = false;
    }
  }

 protected:
  static bool wpctl_is_installed;
  pas::core::adapter::WpctlApapter adapter_;
  constexpr static std::chrono::milliseconds kTimeout{1'000};
};

bool WpctlAdapterTest::wpctl_is_installed = true;
}  // namespace

TEST_F(WpctlAdapterTest, MockGetSinks) {
  auto mock_process_executor = std::make_shared<MockProcessExecutor>();
  pas::core::adapter::WpctlApapter adapter(mock_process_executor);

  // Параметры вызова
  const std::filesystem::path command = "/bin/wpctl";
  const std::vector<std::string> args = {"status"};

  EXPECT_CALL(*mock_process_executor, Execute(command, args, kTimeout))
      .WillOnce(::testing::Return(std::expected<std::string, pas::core::utility::Error>{
          MockProcessExecutor::kMockWpctlStatusResponse}));

  const auto get_sinks_result = adapter.GetSinks(kTimeout);

  ASSERT_TRUE(get_sinks_result.has_value());

  const auto& sinks = get_sinks_result.value();

  EXPECT_EQ(34, sinks[0].id);
  EXPECT_EQ("GSP 370 Analog Stereo", sinks[0].name);
  EXPECT_EQ(0.92, sinks[0].volume);
  EXPECT_TRUE(sinks[0].is_active);

  EXPECT_EQ(62, sinks[1].id);
  EXPECT_EQ("USB Audio Speakers", sinks[1].name);
  EXPECT_EQ(0.54, sinks[1].volume);
  EXPECT_FALSE(sinks[1].is_active);
}

TEST_F(WpctlAdapterTest, GetSinks) {
  if (not wpctl_is_installed) {
    GTEST_SKIP_("/bin/wpctl not found, test skipped.");
  }

  auto result = adapter_.GetSinks(kTimeout);

  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(result->empty());

  // В выводе должен быть один активный sink.
  constexpr std::size_t kActiveSinksCount = 1;
  ASSERT_EQ(kActiveSinksCount,
            std::ranges::count_if(result.value(), [](const pas::core::adapter::Sink& sink) -> bool {
              return sink.is_active;
            }));
}

/**
 * @brief Устанавливаем новый активный sink, после чего вертаем все взад.
 * @warning В этом тесте меняются системные настройки, что может повлечь изменение текущего
 * устройства вывода звука.
 */
TEST_F(WpctlAdapterTest, SetSink) {
  if (not wpctl_is_installed) {
    GTEST_SKIP_("/bin/wpctl not found, test skipped.");
  }

  auto get_sink_id = [](bool is_active,
                        const pas::core::adapter::WpctlApapter& adapter) -> std::optional<int> {
    const auto get_sinks_result = adapter.GetSinks(kTimeout);
    if (not get_sinks_result.has_value() or get_sinks_result->empty()) {
      return std::nullopt;
    }

    // Получаем активный в данный момент sink.
    auto active_sink = std::ranges::find_if(
        get_sinks_result.value(),
        [is_active](const auto& sink) -> auto { return sink.is_active == is_active; });

    if (get_sinks_result.value().end() == active_sink) {
      return std::nullopt;
    }

    return active_sink->id;
  };

  const auto active_sink_id = get_sink_id(true, adapter_);
  ASSERT_TRUE(active_sink_id.has_value());

  const auto expected_next_active_sink_id = get_sink_id(false, adapter_);

  // Проверяем что есть неактивный sink - иначе сбрасываем тест, так как не на что переключаться.
  if (std::nullopt == expected_next_active_sink_id) {
    GTEST_SKIP_("Only one sink already exist");
  }

  // Устанавливаем другой sink.
  auto set_sink_result = adapter_.SetSink(expected_next_active_sink_id.value(), kTimeout);
  ASSERT_TRUE(set_sink_result.has_value());

  // Проверяем, что активный sink изменился.
  const auto new_active_sink_id = get_sink_id(true, adapter_);

  ASSERT_TRUE(new_active_sink_id.has_value());
  EXPECT_EQ(expected_next_active_sink_id.value(), new_active_sink_id.value());

  if (new_active_sink_id.value() != active_sink_id.value()) {
    // Меняем обратно активный sink.
    set_sink_result = adapter_.SetSink(active_sink_id.value(), kTimeout);
    ASSERT_TRUE(set_sink_result.has_value());

    const auto active_sink_id_after_switch = get_sink_id(true, adapter_);

    // Проверяем что sink корректно изменился на тот что был вначале.
    ASSERT_TRUE(active_sink_id_after_switch.has_value());
    ASSERT_EQ(active_sink_id.value(), active_sink_id_after_switch.value());
  }
}

const std::string_view MockProcessExecutor::kMockWpctlStatusResponse =
    R"(PipeWire 'pipewire-0' [1.6.7, user@linux, cookie:907739398]
 └─ Clients:
        32. WirePlumber                         [1.6.7, user@linux, pid:878]
        33. pipewire                            [1.6.7, user@linux, pid:880]
        47. WirePlumber [export]                [1.6.7, user@linux, pid:878]
        48. Telegram                            [1.6.7, user@linux, pid:1002]
        49. xdg-desktop-portal                  [1.6.7, user@linux, pid:1097]
        50. xdg-desktop-portal-hyprland         [1.6.7, user@linux, pid:1209]
        51. wpctl                               [1.6.7, user@linux, pid:17453]
        75. Steam Voice Settings                [1.6.7, user@linux, pid:1255]
        76. Steam                               [1.6.7, user@linux, pid:1255]
        78. Chromium input                      [1.6.7, user@linux, pid:288]

Audio
 ├─ Devices:
 │      52. Navi 31 HDMI/DP Audio               [alsa]
 │      53. USB Audio                           [alsa]
 │      54. Radeon High Definition Audio Controller [alsa]
 │      55. GSP 370                             [alsa]
 │  
 ├─ Sinks:
 │  *   34. GSP 370 Analog Stereo               [vol: 0.92]
 │      62. USB Audio Speakers                  [vol: 0.54]
 │  
 ├─ Sources:
 │  *   46. GSP 370 Mono                        [vol: 0.76]
 │      63. USB Audio Line Input                [vol: 1.00]
 │      64. USB Audio Microphone                [vol: 1.00]
 │  
 ├─ Filters:
 │  
 └─ Streams:

Video
 ├─ Devices:
 │  
 ├─ Sinks:
 │  
 ├─ Sources:
 │  
 ├─ Filters:
 │  
 └─ Streams:

Settings
 └─ Default Configured Devices:
         0. Audio/Sink    alsa_output.usb-Sennheiser_GSP_370-00.analog-stereo)";
