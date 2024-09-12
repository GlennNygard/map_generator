#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "foliage_reader.h"
#include "foliage_info.h"


int get_foliage_w_set_count(std::vector<FoliageInfo> dataList) {
    int foliageCount = 0;
    for(auto info : dataList) {
        if(info.directionSet != nullptr) {
            foliageCount++;
        }
    }
    return foliageCount;
}

int get_set_count(FoliageInfo info) {
    return info.directionSet->size();
}

TEST_CASE( "Foliage yaml is read correctly", "[yaml_read]" ) {

    auto path = std::filesystem::path (RESOURCE_PATH_TEST) / "foliage_tests.yaml";
    std::vector<FoliageInfo> dataList = {};
    foliagereader::read(dataList, path, 0, 0);

    SECTION("Default data read correctly.") {
        REQUIRE( dataList.size() == 4 );
        REQUIRE( dataList.capacity() >= 4 );
    }

    SECTION("Check order.") {
        CHECK( dataList[0].foliageName == "Foliage01" );
        CHECK( dataList[3].foliageName == "Foliage04" );
    }

    SECTION("Foliage set data read correctly.") {
        CHECK( get_foliage_w_set_count(dataList) == 2 );
        CHECK( get_set_count(dataList[3]) == 4 );
        CHECK( dataList[3].directionSet->at(0) == 1 );
        CHECK( dataList[3].directionSet->at(2) == 3 );
    }
}
