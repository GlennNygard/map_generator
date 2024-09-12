#include <catch2/catch_test_macros.hpp>

#include "foliage_definitions.h"


class FoliageDefinitionsFixture {

    FoliageDefinitions foliageDefinitions = FoliageDefinitions::instance();

};


int toFI(std::string foliageName, FoliageDefinitions fd) {
    auto nameToFoliageIndex = fd.get_name_to_foliage_index_map();
    auto itr = nameToFoliageIndex.find(foliageName);
    if(itr == nameToFoliageIndex.end()) {
        logger::log_error("Could not find entry for foliage: "+foliageName);
    }
    return itr->second;
}

int toMI(std::string foliageName, FoliageDefinitions fd) {
    int foliageIndex = toFI(foliageName, fd);
    auto fiToFIIndex = fd.foliageInfoElements;
    return fiToFIIndex[foliageIndex].mapIndex;
}


TEST_CASE_METHOD(FoliageDefinitionsFixture, "Foliage definitions correct.", "[foliage_definitions]") {

    auto fd = FoliageDefinitions::instance();

    SECTION("Name to map index translation.") {
        CHECK(toMI("NoFoliage", fd) == 1);
        CHECK(toMI("CliffsideDesertMiddle", fd) == 1301);
        CHECK(toMI("WallThreeUp", fd) == 1610);
        CHECK(toMI("BorderTreeDownRight", fd) == 1708);
        CHECK(toMI("TreeCliffedge", fd) == 50);
    }
}
