#include <catch2/catch_test_macros.hpp>
#include "../utils/fake_file_creator.h"

#include "fsp/search/search_line_engine.h"
#include "fsp/filesystem/log_entry.h"
#include "fsp/scanners/file_scanner.h"

TEST_CASE("FileScanner scanning file", "[scanner][unit]")
{
    FakeFileCreator fake_creator(std::string("jpaesojfpoesfjwpof\n")
                + std::string("adawdjioher;gseoghrse'oih\n") 
                + std::string("adaoiuwfhiuFLWAFLWIUEAGFWA;FAJF\n"));
    
    fsp::ss::FileScanner scanner;
    std::string path = fake_creator.get_path();

    fsp::fs::FileReader file_reader(path);

    std::string text_search = "adawdjioher;gseoghrse'oih";

    auto result = scanner.search(text_search, file_reader);


    REQUIRE(result.file_name == path);
    REQUIRE(result.line_number == 2);
    REQUIRE(result.text == text_search);
}