#ifndef FILESYSTEM_FILE_READER_H
#define FILESYSTEM_FILE_READER_H

#include <fstream>
#include <algorithm>
#include <iostream>

#include "log_entry.h"

namespace fsp::fs
{
    class FileReader
    {
    public:
        FileReader(std::string file_name);

        ~FileReader();

        bool open(std::string file_name);
        
        LogEntry search_text(std::string text);

        bool is_valid();


    private:
        std::ifstream in;
        std::string file_name;
        bool status{false};
    };
}

#endif // FILESYSTEM_FILE_READER_H