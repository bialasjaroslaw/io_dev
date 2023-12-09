# io_dev

## Motivation
I wanted to have simple and lightweight library for IO operations (real or in memory) regardles of their location. Devices should be replaceable easily without any need for extensive code adjustments or recompilation.

## Functionality

Simple library for IO devices operations. Devices should be at least:
* regular text or binary file
* remote file/network stream (http request-like access, additional library could be needed for HTTP GET, etc)
* memory file
* data streams (stdin)

Operations:
* read - read data, all by default, number of bytes as an optinal parameter
* pos - return current position (if available)
* size - return size of data held be device (if possible)
* seek - jump to specific portion of the file (just set position, might not perform any operation at all)
* is_sequential - if device allow for sequential or random access

## Further work
* [ ] Add tests
* [ ] Add memory device (circular buffer as an option?)
* [ ] Add network file device
* [ ] Add stdin stream device

## Tutorial

Open file in binary mode, read data from it and return as int32_t type 

```c++
auto value = File("file.bin", OpenMode::ReadBinary).read_as<int32_t>();
```

Open file in text mode, read content and return as vector of lines

```c++
auto lines = File("file.txt", OpenMode::Read).read_lines();
```

## Tests
TBD

## License
Licensed under BSD license.
