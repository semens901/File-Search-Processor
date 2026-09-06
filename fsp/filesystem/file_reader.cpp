#include "file_reader.h"

fsp::fs::FileReader::FileReader(std::string file_name)
: in(file_name)
{
    this->file_name = file_name;
    if(in.is_open())
        status = true;
}

fsp::fs::FileReader::~FileReader()
{
    if(is_valid())
        in.close();
}

bool fsp::fs::FileReader::open(std::string file_name)
{
    in.open(file_name);
    this->file_name = file_name;
    if(in.is_open())
    {
        status = true;
        return status;
    }
    return false;
}

fsp::fs::LogEntry fsp::fs::FileReader::search_text(std::string text)
{
    fsp::fs::LogEntry log_entry;
    if (!is_valid())
    {
        std::cerr << "FileReader is not valid. Please open a file first." << std::endl;
        return fsp::fs::LogEntry();
    }

    std::string line;
    
    std::streampos pos;

    std::transform(text.begin(), 
        text.end(), 
        text.begin(), 
        [](unsigned char c) {
            return std::tolower(c);
        });

    while (std::getline(in, line)) 
    {
        pos = in.tellg();

        std::transform(line.begin(), 
        line.end(), 
        line.begin(), 
        [](unsigned char c) {
            return std::tolower(c);
        });

        auto it = line.find(text);
        if(it != std::string::npos)
            break;
    }

    log_entry.line_number = pos;
    log_entry.text = line;
    log_entry.file_name = this->file_name;

    return log_entry;
}

bool fsp::fs::FileReader::is_valid()
{
    return status;
}
