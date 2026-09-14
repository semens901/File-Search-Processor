#ifndef FAKE_FILE_CREATOR_H
#define FAKE_FILE_CREATOR_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FakeFileCreator 
{
public:
    
    FakeFileCreator(const std::string& content);

    ~FakeFileCreator();

    std::string get_path() const;

private:
    std::filesystem::path path;
};


#endif // FAKE_FILE_CREATOR_H