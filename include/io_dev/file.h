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

            std::string read() override;
            int64_t pos() const override;
            int64_t seek(int64_t new_position) override;
            bool is_sequential() const override;
            
            std::vector<std::string> read_lines();
        private:
            FILE* _ptr = nullptr;
            std::filesystem::path _path;
    };
}