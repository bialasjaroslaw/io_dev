#include "file.h"
#include <text_processing/common.hpp>
#include <text_processing/split.hpp>

#include <unordered_map>

static std::unordered_map<IO::OpenMode, const char*> MODE_MAP{
    {IO::OpenMode::Read, "r"},
    {IO::OpenMode::Write, "w"}
};

namespace IO{
    File::File(const std::filesystem::path& path, OpenMode mode)
        : _path(path){
            _ptr = fopen(path.c_str(), MODE_MAP[mode]);
        }

        std::string File::read(){
            fseek(_ptr, 0, SEEK_END);
            auto sz = pos();
            seek(0);
            std::string buffer;
            buffer.resize(sz);
            auto read = fread(buffer.data(), sz, 1, _ptr);
            buffer.resize(read);
            return buffer;
        }

        std::vector<std::string> File::read_lines(){
            return Text::to_string_vector(Text::split(read(), '\n'));
        }

        int64_t File::pos() const{
            return ftell(_ptr);
        }
        int64_t File::seek(int64_t new_position){
            if (fseek(_ptr, new_position, SEEK_SET) == 0)
                return pos();
            return -1;
        }

        bool File::is_sequential() const{
            return true;
        }
}