#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <set>

#include "fsp/concurrency/thread_safe_queue.h"
#include "fsp/scanners/directory_scanner.h"
#include "../utils/fake_directory_creator.h"

namespace fs = std::filesystem;

static void write_file(const fs::path& path, const std::string& content)
{
    std::ofstream out(path, std::ios::binary);
    out << content;
}

TEST_CASE("DirectoryScanner scans recursively and returns only regular files", "[directory][unit]")
{
    const auto root = fs::temp_directory_path() / ("fsp_directory_scanner_" + std::to_string(std::rand()));
    const auto nested = root / "nested";
    const auto file1 = root / "root.txt";
    const auto file2 = nested / "child.txt";
    const auto directory_to_ignore = root / "skip_me";

    fs::create_directories(nested);
    fs::create_directories(directory_to_ignore);

    write_file(file1, "root file\n");
    write_file(file2, "child file\n");

    fsp::cy::ThreadSafeQueue<fs::path> queue;
    fsp::ss::DirectoryScanner scanner;

    scanner.scan(root, queue);

    std::set<fs::path> found_files;
    while (!queue.empty())
    {
        fs::path item;
        REQUIRE(queue.try_pop(item));
        found_files.insert(item);
    }

    REQUIRE(found_files.size() == 2);
    REQUIRE(found_files.count(file1) == 1);
    REQUIRE(found_files.count(file2) == 1);
    REQUIRE(found_files.count(directory_to_ignore) == 0);

    fs::remove_all(root);
}

TEST_CASE("DirectoryScanner ignores missing root path", "[directory][unit]")
{
    const auto missing_root = fs::temp_directory_path() / ("fsp_directory_scanner_missing_" + std::to_string(std::rand()));

    fsp::cy::ThreadSafeQueue<fs::path> queue;
    fsp::ss::DirectoryScanner scanner;

    scanner.scan(missing_root, queue);

    REQUIRE(queue.empty());
}

TEST_CASE("DirectoryScanner finds files in deeply nested folders", "[directory][unit]")
{
    FakeDirectoryCreator dir("nested_scan");
    const auto root = dir.root_path();

    dir.create_file("a.txt", "first\n");
    dir.create_file("level1/level2/b.txt", "second\n");
    dir.create_file("level1/level2/level3/c.txt", "third\n");
    dir.create_directory("empty_dir");

    fsp::cy::ThreadSafeQueue<fs::path> queue;
    fsp::ss::DirectoryScanner scanner;

    scanner.scan(root, queue);

    std::set<fs::path> found_files;
    while (!queue.empty())
    {
        fs::path item;
        REQUIRE(queue.try_pop(item));
        found_files.insert(item);
    }

    REQUIRE(found_files.size() == 3);
    REQUIRE(found_files.count(root / "a.txt") == 1);
    REQUIRE(found_files.count(root / "level1/level2/b.txt") == 1);
    REQUIRE(found_files.count(root / "level1/level2/level3/c.txt") == 1);
}

TEST_CASE("DirectoryScanner ignores directories while keeping regular files", "[directory][unit]")
{
    FakeDirectoryCreator dir("regular_only");
    const auto root = dir.root_path();

    dir.create_file("root.txt", "x\n");
    dir.create_file("nested/inner.txt", "y\n");
    dir.create_directory("nested/empty_folder");

    fsp::cy::ThreadSafeQueue<fs::path> queue;
    fsp::ss::DirectoryScanner scanner;

    scanner.scan(root, queue);

    std::set<fs::path> found_files;
    while (!queue.empty())
    {
        fs::path item;
        REQUIRE(queue.try_pop(item));
        found_files.insert(item);
    }

    REQUIRE(found_files.size() == 2);
    REQUIRE(found_files.count(root / "root.txt") == 1);
    REQUIRE(found_files.count(root / "nested/inner.txt") == 1);
    REQUIRE(found_files.count(root / "nested/empty_folder") == 0);
}
