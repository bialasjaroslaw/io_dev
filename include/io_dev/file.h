#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "dev.h"

namespace IO {
class File : public Device {
 public:
  File(const std::filesystem::path& path, OpenMode mode);
  ~File() override;
  bool is_open() const override;
  OpenMode mode() const override;
  bool close() override;

  std::optional<uint64_t> read_bytes(uint64_t amount = ALL,
                                     char* buff = nullptr,
                                     uint64_t buff_size = 0) override;
  std::optional<uint64_t> pos() const override;
  std::optional<uint64_t> size() const override;
  std::optional<uint64_t> seek(uint64_t offset,
                               SeekMode mode = SeekMode::Jump) override;
  bool is_good() const override;
  bool is_sequential() const override;

  std::optional<std::vector<std::string>> read_lines();

 private:
  FILE* _ptr = nullptr;
  std::filesystem::path _path;
  std::optional<uint64_t> _pos = std::nullopt;
  std::optional<uint64_t> _size = std::nullopt;
  OpenMode _mode = OpenMode::Unknown;
};
}  // namespace IO