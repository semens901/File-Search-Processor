#include <catch2/catch_test_macros.hpp>
#include "../utils/fake_file_creator.h"

#include "fsp/search/search_line_engine.h"
#include "fsp/filesystem/log_entry.h"

TEST_CASE("SearchLineEngine finds exact line in a file", "[search][unit]")
{
    fsp::sh::SearchLineEngine search_engine;

    FakeFileCreator fake_creator(std::string("jpaesojfpoesfjwpof\n")
                + std::string("adawdjioher;gseoghrse'oih\n") 
                + std::string("adaoiuwfhiuFLWAFLWIUEAGFWA;FAJF\n"));
    
    std::string path = fake_creator.get_path();

    fsp::fs::FileReader file_reader(path);

    std::string text_search = "adawdjioher;gseoghrse'oih";

    auto search = search_engine.process_search(text_search, file_reader);

    REQUIRE(search.file_name == path);
    REQUIRE(search.line_number == 2);
    REQUIRE(search.text == text_search);
}