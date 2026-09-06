#include "search_line.h"
/*
fsp::fs::LogEntry fsp::sh::SearchLine::search_text(std::string text)
{
    if (!file_reader.is_valid())
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

    while (std::getline(file_reader.get_stream(), line)) 
    {
        pos = file_reader.get_stream().tellg();

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
    log_entry.file_name = file_reader.get_file_name();

    return log_entry;
}

fsp::fs::LogEntry fsp::sh::SearchLine::get_log_entry() const
{
    return log_entry;
}
*/

fsp::fs::LogEntry fsp::sh::SearchLine::search_text(std::string text, fsp::fs::FileReader& file_reader)
{
    if (!file_reader.is_valid())
    {
        std::cerr << "FileReader is not valid. Please open a file first." << std::endl;
        return fsp::fs::LogEntry();
    }

    std::string line;
    std::streamoff line_number = 1;

    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c); });

    bool found = false;
    while (std::getline(file_reader.get_stream(), line))
    {
        std::string original = line;

        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c){ return std::tolower(c); });

        auto it = line.find(text);
        if (it != std::string::npos)
        {
            log_entry.line_number = line_number;
            log_entry.text = original;
            log_entry.file_name = file_reader.get_file_name();
            found = true;
            break;
        }

        ++line_number;
    }

    if (!found)
    {
        log_entry.line_number = -1;
        log_entry.text.clear();
        log_entry.file_name = file_reader.get_file_name();
    }

    return log_entry;
}

fsp::fs::LogEntry fsp::sh::SearchLine::get_log_entry() const
{
    return log_entry;
}
