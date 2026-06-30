#include "pas-core/filesystem/file_reader.hh"

#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

#include "utility/temp_file.hh"

TEST(FileReaderTest, ReadSuccess) {
  constexpr std::string_view kExpectedContent = "This is success test\nNew line";
  auto file = pas::tests::utility::TempFile("ReadSuccess",
                                            {kExpectedContent.data(), kExpectedContent.size()});

  auto readed_content = pas::filesystem::FileReader::Read(file.GetFullPath());

  EXPECT_EQ(kExpectedContent, readed_content);
}

TEST(FileReaderTest, ReadNonExistFileFailure) {
  std::string non_exist_file_name = "NonExistFileName";

  // Пытаемся несколько раз создать уникальное имя.
  for (std::size_t wrong_attrmpt_count = 0; std::filesystem::exists(non_exist_file_name);
       ++wrong_attrmpt_count) {
    non_exist_file_name += "%";

    ASSERT_LE(wrong_attrmpt_count, 20);
  }
  EXPECT_THROW(pas::filesystem::FileReader::Read(non_exist_file_name), std::runtime_error);
};

TEST(FileReaderTest, ReadPartialFile) {
  constexpr std::string_view kContent = "This is success test";
  constexpr std::string_view kExpectedPartContent{kContent.data(), kContent.size() / 3};

  auto file =
      pas::tests::utility::TempFile("ReadPartialSuccess", {kContent.data(), kContent.size()});

  auto readed_content =
      pas::filesystem::FileReader::Read(file.GetFullPath(), kExpectedPartContent.size());

  EXPECT_EQ(kExpectedPartContent, readed_content);
}

TEST(FileReaderTest, ReadEmptyFile) {
  auto file = pas::tests::utility::TempFile("EmptyFile", {});
  auto content = pas::filesystem::FileReader::Read(file.GetFullPath());

  EXPECT_TRUE(content.empty());
}

TEST(FileReaderTest, ReadMoreThanFileSize) {
  constexpr std::string_view kContent = "Short";
  constexpr std::size_t kReadedBytesCount = kContent.size() * 2;

  auto file = pas::tests::utility::TempFile("SmallFile", {kContent.data(), kContent.size()});
  auto result = pas::filesystem::FileReader::Read(file.GetFullPath(), kReadedBytesCount);

  EXPECT_EQ(result, kContent);
}

TEST(FileReaderTest, ReadZeroBytes) {
  constexpr std::string_view kContent = "Some content";

  auto file = pas::tests::utility::TempFile("ZeroRead", {kContent.data(), kContent.size()});

  auto result = pas::filesystem::FileReader::Read(file.GetFullPath(), 0);

  EXPECT_TRUE(result.empty());
}
