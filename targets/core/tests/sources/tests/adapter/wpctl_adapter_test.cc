#include "pas-core/adapter/wpctl_adapter.hh"

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <memory>
#include <optional>
#include <system_error>

#include "pas-core/adapter/data/sink.hh"
#include "pas-core/process/process_executor.hh"

namespace {
class WpctlAdapterTest : public ::testing::Test {
 public:
  static void SetUpTestSuite() {
    pas::core::process::ProcessExecutor executor;
    auto result = executor.Execute("/bin/wpctl", {"help"}, kTimeout);

    if (not result.has_value() and result.error().code == std::errc::no_such_file_or_directory) {
      GTEST_SKIP_("/bin/wpctl not found, skip all tests");
    }
  }

 protected:
  pas::core::adapter::WpctlApapter adapter_{
      std::make_shared<pas::core::process::ProcessExecutor>()};
  constexpr static std::chrono::milliseconds kTimeout{1'000};
};
}  // namespace

TEST_F(WpctlAdapterTest, GetActiveSink) {
  auto result = adapter_.GetActiveSink(kTimeout);

  ASSERT_TRUE(result.has_value());

  EXPECT_TRUE(result->id);
  EXPECT_FALSE(result->name.empty());
  EXPECT_TRUE(result->is_active);
}

TEST_F(WpctlAdapterTest, GetSinks) {
  auto result = adapter_.GetSinks(kTimeout);

  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(result->empty());
}

/**
 * @brief Устанавливаем новый активный sink, после чего вертаем все взад.
 * @warning В этом тесте меняются системные настройки, что может повлечь изменение текущего
 * устройства вывода звука.
 */
TEST_F(WpctlAdapterTest, SetSink) {
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
