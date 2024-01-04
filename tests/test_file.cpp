#include <gtest/gtest.h>
#include <io_dev/file.h>

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

TEST(File, BinaryFileSizeInBinaryMode) {
  auto file = IO::File("file.bin", IO::OpenMode::ReadBinary);
  EXPECT_TRUE(file.is_open());
  EXPECT_EQ(file.size(), 512);
}

TEST(File, BinaryFileSizeInTextMode) {
  auto file = IO::File("file.bin", IO::OpenMode::Read);
  EXPECT_TRUE(file.is_open());
  EXPECT_EQ(file.size(), 512);
}

TEST(File, TextFileSizeInBinaryMode) {
  auto file = IO::File("file.txt", IO::OpenMode::ReadBinary);
  EXPECT_TRUE(file.is_open());
  EXPECT_EQ(file.size(), 512);
}

TEST(File, TextFileSizeInTextMode) {
  auto file = IO::File("file.txt", IO::OpenMode::Read);
  EXPECT_TRUE(file.is_open());
  EXPECT_EQ(file.size(), 512);
}
