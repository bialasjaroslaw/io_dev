#include "file.h"
#include <text_processing/common.hpp>
#include <text_processing/split.hpp>

#include <unordered_map>

static std::unordered_map<IO::OpenMode, const char*> MODE_MAP{
    {IO::OpenMode::Read, "r"},
    {IO::OpenMode::Write, "w"},
    {IO::OpenMode::ReadBinary, "rb"},
    {IO::OpenMode::WriteBinary, "wb"}
};

namespace IO{
    File::File(const std::filesystem::path& path, OpenMode mode)
        : _path(path){
            _ptr = fopen(path.string().c_str(), MODE_MAP[mode]);
            if(_ptr == nullptr)
                return;
            // Handle failures
            fseek(_ptr, 0, SEEK_END);
            _size = ftell(_ptr);
            fseek(_ptr, 0, SEEK_SET);
            _mode = mode;
        }

    File::~File(){
        if(_ptr)
            fclose(_ptr);
    }

    bool File::is_open() const{
        return _ptr != nullptr;
    }
    OpenMode File::mode() const{
        return _mode;
    }

    bool File::close(){
        if(!is_open())
            return false;
        auto result = fclose(_ptr) == 0;
        _ptr = nullptr;
        return result;
    }

    int64_t File::read_bytes(int64_t amount, char* buff, int64_t buff_size){
        if(!can_read())
            return INVALID;
        auto sz = available();
        if(buff == nullptr)
            buff = new char[std::min(amount, sz)];
        auto bytes_read = fread(buff, 1, sz, _ptr);
        _pos += bytes_read;
        return bytes_read;
    }

    int64_t File::pos() const{
        return _pos;
    }

    int64_t File::size() const{
        return _size;
    }

    int64_t File::seek(int64_t offset, SeekMode mode){
        auto new_pos = mode == SeekMode::Jump ? offset : (mode == SeekMode::Skip ? _pos + offset : _size - offset);
        if (fseek(_ptr, offset, SEEK_SET) == 0){
            _pos = new_pos;
            return _pos;
        }
        return UNKNOWN;
    }

    bool File::is_good() const
    {
        return _ptr != nullptr;
    }

    bool File::is_sequential() const{
        return false;
    }

    std::vector<std::string> File::read_lines(){
        return Text::to_string_vector(Text::split(read(), '\n'));
    }
}