#include "filesystem/file_reader.hh"

#include <gtest/gtest.h>

#include <span>
#include <stdexcept>
#include <string_view>

#include "utility/temp_file.hh"

TEST(FileReaderTest, ReadSuccess) {
  constexpr std::string_view expected_content = "This is success test\nNew line";
  auto file = pas::tests::utility::TempFile("ReadSuccess",
                                            {expected_content.data(), expected_content.size()});

  auto readed_content = pas::filesystem::FileReader::Read(file.GetFullPath());

  EXPECT_EQ(expected_content, readed_content);
}

TEST(FileReaderTest, ReadNonExistFileFailure) {
  EXPECT_THROW(pas::filesystem::FileReader::Read("sdafasd"), std::runtime_error);
};
