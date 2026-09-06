#ifndef FILESYSTEM_FILE_READER_H
#define FILESYSTEM_FILE_READER_H

#include <fstream>
#include <algorithm>
#include <iostream>
#include <memory>

namespace fsp::fs
{
    class FileReader
    {
    public:
        FileReader() = default;
        FileReader(std::string file_name);

        ~FileReader();

        bool open(std::string file_name);

        bool is_valid() const;

        std::ifstream& get_stream() const;

        std::string get_file_name() const;


    private:
        mutable std::ifstream in;
        std::string file_name;
        mutable bool status{false};
    };
}

#endif // FILESYSTEM_FILE_READER_H