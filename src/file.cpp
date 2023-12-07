#include "file.h"

#include <unordered_map>

static std::unordered_map<IO::OpenMode, const char*> MODE_MAP{
    {IO::OpenMode::Read, "r"},
    {IO::OpenMode::Write, "w"}
};

namespace IO{
    File::File(const std::filesystem::path& path, OpenMode mode)
        : _path(path){
            _ptr = fopen(path.string().c_str(), MODE_MAP[mode]);
        }

        std::string File::read(){
            fseek(_ptr, 0, SEEK_END);
            auto sz = ftell(_ptr);
            fseek(_ptr, 0, SEEK_SET);
            std::string buffer;
            buffer.resize(sz);
            auto read = fread(buffer.data(), sz, 1, _ptr);
            buffer.resize(read);
            return buffer;
        }
}