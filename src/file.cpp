#include "file.h"

#include <text_processing/common.hpp>
#include <text_processing/split.hpp>
#include <unordered_map>

static std::unordered_map<IO::OpenMode, const char*> MODE_MAP{
    {IO::OpenMode::Read, "r"},
    {IO::OpenMode::Write, "w"},
    {IO::OpenMode::ReadBinary, "rb"},
    {IO::OpenMode::WriteBinary, "wb"}};

namespace IO {
File::File(const std::filesystem::path& path, OpenMode mode) : _path(path) {
  _ptr = fopen(path.string().c_str(), MODE_MAP[mode]);
  if (_ptr == nullptr) return;
  // Handle failures
  if (fseek(_ptr, 0, SEEK_END) != 0) {
    return;
  }
  auto size = ftell(_ptr);
  if (fseek(_ptr, 0, SEEK_SET) != 0 || size < 0) {
    return;
  }
  _size = size;
  _pos = 0;
  _mode = mode;
}

File::~File() {
  if (_ptr) fclose(_ptr);
}

bool File::is_open() const { return _ptr != nullptr; }
OpenMode File::mode() const { return _mode; }

bool File::close() {
  if (!is_open()) return false;
  auto result = fclose(_ptr) == 0;
  _ptr = nullptr;
  _pos = std::nullopt;
  _size = std::nullopt;
  return result;
}

std::optional<uint64_t> File::read_bytes(uint64_t amount, char* buff,
                                         uint64_t buff_size) {
  auto available_bytes = available();
  if (!can_read() || !available_bytes) return std::nullopt;
  if (buff == nullptr)
    buff = new char[std::min(amount, available_bytes.value())];
  auto bytes_read =
      fread(buff, 1, std::min(buff_size, available_bytes.value()), _ptr);
  _pos.value() += bytes_read;
  return bytes_read;
}

std::optional<uint64_t> File::pos() const { return _pos; }

std::optional<uint64_t> File::size() const { return _size; }

std::optional<uint64_t> File::seek(uint64_t offset, SeekMode mode) {
  if (!_pos || !_size) return std::nullopt;
  if (mode == SeekMode::Jump && offset > _size.value()) return std::nullopt;
  if (mode == SeekMode::Skip &&
      (offset > _size.value() || _size.value() - offset < _pos.value()))
    return std::nullopt;
  if (mode == SeekMode::End && _size.value() < offset) return std::nullopt;

  auto new_pos = mode == SeekMode::Jump
                     ? offset
                     : (mode == SeekMode::Skip ? _pos.value() + offset
                                               : _size.value() - offset);
  if (static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) > new_pos)
    return std::nullopt;

  if (fseek(_ptr, static_cast<int64_t>(new_pos), SEEK_SET) == 0) {
    _pos = new_pos;
    return _pos;
  }
  return std::nullopt;
}

bool File::is_good() const { return _ptr != nullptr; }

bool File::is_sequential() const { return false; }

std::optional<std::vector<std::string>> File::read_lines() {
  auto bytes = read();
  if (!bytes) return std::nullopt;
  return Text::to_string_vector(Text::split(bytes.value(), '\n'));
}
}  // namespace IO