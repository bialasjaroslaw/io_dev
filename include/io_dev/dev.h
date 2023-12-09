#pragma once

#include <optional>
#include <string>
#include <span>

namespace IO{
    enum class OpenMode{
        Unknown,
        Read,
        Write,
        ReadBinary,
        WriteBinary
    };

    enum class SeekMode{
        Jump,
        Skip,
        End
    };

    constexpr int64_t ALL = -1;
    constexpr int64_t UNKNOWN = -2;
    constexpr int64_t INVALID = -3;
    class Device{
        public:
            virtual ~Device() = default;
            virtual bool is_open() const = 0;
            virtual OpenMode mode() const = 0;
            virtual bool close() = 0;
            virtual int64_t read_bytes(int64_t amount = ALL, char* buff = nullptr, int64_t buff_size = 0) = 0;
            bool can_read() const{
                return mode() == OpenMode::Read || mode() == OpenMode::ReadBinary;
            }
            bool can_write() const{
                return mode() == OpenMode::Write || mode() == OpenMode::WriteBinary;
            }
            std::string read(int64_t amount = ALL){
                std::string data;
                if(!can_read())
                    return data;
                auto sz = size();
                // Could be an issue if device does not return correct size or returns 0
                if(sz > 0)
                    data.resize(sz);
                auto bytes_read = read_bytes(amount, data.data(), data.size());
                data.resize(bytes_read);
                return data;
            }
            virtual int64_t pos() const = 0;
            virtual int64_t size() const = 0;
            int64_t available() const{
                auto p = pos();
                auto s = size();
                if(p == UNKNOWN || s  == UNKNOWN || p > s)
                    return UNKNOWN;
                return std::max(int64_t{0}, s - p);
            }
            virtual int64_t seek(int64_t offset, SeekMode mode = SeekMode::Jump) = 0;
            int64_t skip(int64_t offset){
                return seek(offset, SeekMode::Skip);
            }
            virtual bool is_good() const = 0;
            virtual bool is_sequential() const = 0;

            operator bool() const{
                return is_good();
            }

            template <typename T>
            std::optional<T> read_as(){
                // Maybe check if mode is correct for data type?
                char buff[sizeof(T)];
                if(read_bytes(sizeof(T), buff, sizeof(T)) != sizeof(T))
                    return std::nullopt;
                T value{};
                memcpy(&value, buff);
                return value;
            }
    };
}