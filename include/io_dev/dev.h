#pragma once

#include <string>

namespace IO{
    class Device{
        public:
            virtual ~Device() = 0;
            virtual std::string read() = 0;
            virtual int64_t pos() const = 0;
            virtual int64_t seek(int64_t new_position) = 0;
            virtual bool is_sequential() const = 0;
    };
}