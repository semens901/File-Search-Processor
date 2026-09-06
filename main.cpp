#include <iostream>

#include "fsp/search/search_line_engine.h"
#include "fsp/filesystem/log_entry.h"

int main()
{
    fsp::sh::SearchLineEngine search_engine;
    fsp::fs::FileReader file_reader("test.txt");
    auto search = search_engine.process_search("He checked the wooden shed, the mailbox, and even his old toy box,", file_reader);

    std::cout << "Line Number: " << search.line_number << std::endl;
    std::cout << "Text: " << search.text << std::endl;  
    std::cout << "File Name: " << search.file_name << std::endl;

    return 0;
}