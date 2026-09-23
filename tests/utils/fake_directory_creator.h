#ifndef FAKE_DIRECTORY_CREATOR_H
#define FAKE_DIRECTORY_CREATOR_H

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class FakeDirectoryCreator
{
public:
    explicit FakeDirectoryCreator(const std::string& root_name = "fake_dir");
    ~FakeDirectoryCreator();

    const fs::path& root_path() const;
    void create_file(const fs::path& relative_path, const std::string& content = "");
    void create_directory(const fs::path& relative_path);

private:
    fs::path path_;
};

#endif // FAKE_DIRECTORY_CREATOR_H
