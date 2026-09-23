#include "file_reader.h"

fsp::fs::FileReader::FileReader(std::string file_name)
: in(file_name), file_name(file_name)
{
    status = in.is_open();
    if(!status)
        std::cerr << "Failed to open file: " << file_name << std::endl;
}

fsp::fs::FileReader::~FileReader()
{
    if(is_valid())
        in.close();
}

bool fsp::fs::FileReader::open(std::string file_name)
{
    if(in.is_open())
        in.close();

    this->file_name = file_name;
    in.open(file_name);
    status = in.is_open();

    if(status)
        return true;

    std::cerr << "Failed to open file: " << file_name << std::endl;
    return false;
}

bool fsp::fs::FileReader::is_valid() const
{
    return status;
}

std::ifstream& fsp::fs::FileReader::get_stream() const
{
    return in;
}

std::string fsp::fs::FileReader::get_file_name() const
{
    return file_name;
}
