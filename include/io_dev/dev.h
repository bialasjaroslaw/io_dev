#pragma once

#include <optional>
#include <string>
#include <span>
#include <limits>

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

    constexpr uint64_t ALL = std::numeric_limits<uint64_t>::max();
    class Device{
        public:
            virtual ~Device() = default;
            virtual bool is_open() const = 0;
            virtual OpenMode mode() const = 0;
            virtual bool close() = 0;
            virtual std::optional<uint64_t> read_bytes(uint64_t amount = ALL, char* buff = nullptr, uint64_t buff_size = 0) = 0;
            bool can_read() const{
                return mode() == OpenMode::Read || mode() == OpenMode::ReadBinary;
            }
            bool can_write() const{
                return mode() == OpenMode::Write || mode() == OpenMode::WriteBinary;
            }
            std::optional<std::string> read(uint64_t amount = ALL){
                std::string data;
                if(!can_read())
                    return std::nullopt;
                auto size_bytes = size();
                // Could be an issue if device does not return correct size or returns 0
                auto resize_to = size_bytes.value_or(amount);
                data.resize(std::min(resize_to, amount));
                auto bytes_read = read_bytes(amount, data.data(), data.size());
                if(!bytes_read)
                    return std::nullopt;
                data.resize(bytes_read.value());
                return data;
            }
            virtual std::optional<uint64_t> pos() const = 0;
            virtual std::optional<uint64_t> size() const = 0;
            std::optional<uint64_t> available() const{
                auto p = pos();
                auto s = size();
                if(!p || !s || p.value() > s.value())
                    return std::nullopt;
                return std::max(uint64_t{0}, s.value() - p.value());
            }
            virtual std::optional<uint64_t> seek(uint64_t offset, SeekMode mode = SeekMode::Jump) = 0;
            std::optional<uint64_t> skip(uint64_t offset){
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