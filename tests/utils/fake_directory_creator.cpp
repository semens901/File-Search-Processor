#include "fake_directory_creator.h"

#include <fstream>
#include <random>

FakeDirectoryCreator::FakeDirectoryCreator(const std::string& root_name)
{
    const auto seed = static_cast<unsigned int>(std::random_device{}());
    path_ = fs::temp_directory_path() / (root_name + "_" + std::to_string(seed));
    fs::create_directories(path_);
}

FakeDirectoryCreator::~FakeDirectoryCreator()
{
    if (!path_.empty() && fs::exists(path_))
    {
        fs::remove_all(path_);
    }
}

const fs::path& FakeDirectoryCreator::root_path() const
{
    return path_;
}

void FakeDirectoryCreator::create_file(const fs::path& relative_path, const std::string& content)
{
    const auto full_path = path_ / relative_path;
    if (full_path.has_parent_path())
    {
        fs::create_directories(full_path.parent_path());
    }

    std::ofstream out(full_path, std::ios::binary);
    out << content;
}

void FakeDirectoryCreator::create_directory(const fs::path& relative_path)
{
    fs::create_directories(path_ / relative_path);
}
