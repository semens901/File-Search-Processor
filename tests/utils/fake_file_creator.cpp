#include "fake_file_creator.h"

FakeFileCreator::FakeFileCreator(const std::string &content)
{
    path = fs::temp_directory_path() / ("test_" + std::to_string(rand()) + ".txt");
    
    std::ofstream out(path);
    out << content;
}

FakeFileCreator::~FakeFileCreator()
{
    if (fs::exists(path)) {
        fs::remove(path);
    }
}

std::string FakeFileCreator::get_path() const
{
    return path.string();
}
