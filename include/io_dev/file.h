#pragma once

#include "dev.h"

#include <filesystem>
#include <string>
#include <vector>

namespace IO{
    enum class OpenMode{
        Read,
        Write
    };

    class File: public Device{
        public:
        File(const std::filesystem::path& path, OpenMode mode);
        ~File() override;
        std::string read();
        private:
        FILE* _ptr = nullptr;
        std::filesystem::path _path;
    };
}