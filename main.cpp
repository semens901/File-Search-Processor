#include <iostream>

#include "fsp/filesystem/file_reader.h"

int main()
{
    fsp::fs::FileReader reader("test.txt");
    fsp::fs::LogEntry entry = reader.search_text("but the key did not fit anywhere.Feeling tired, Max sat down on the ");

    std::cout << "Line number: " << entry.line_number << std::endl;
    std::cout << "Text: " << entry.text << std::endl;
    std::cout << "File name: " << entry.file_name << std::endl;

    return 0;
}