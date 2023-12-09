#pragma once

#include "dev.h"

#include <filesystem>
#include <string>
#include <vector>

namespace IO{
    class File: public Device{
        public:
            File(const std::filesystem::path& path, OpenMode mode);
            ~File() override;
            bool is_open() const override;
            OpenMode mode() const override;
            bool close() override;

            int64_t read_bytes(int64_t amount = ALL, char* buff = nullptr, int64_t buff_size = 0) override;
            int64_t pos() const override;
            int64_t seek(int64_t offset, SeekMode mode = SeekMode::Jump) override;
            bool is_sequential() const override;
            
            std::vector<std::string> read_lines();
        private:
            FILE* _ptr = nullptr;
            std::filesystem::path _path;
            int64_t _pos = 0;
            int64_t _size = 0;
            OpenMode _mode = OpenMode::Unknown;
    };
}