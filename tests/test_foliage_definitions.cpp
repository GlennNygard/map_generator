#include <catch2/catch_test_macros.hpp>

#include "foliage_definitions.h"


int toFI(std::string foliageName, FoliageDefinitions& fd) {
    auto nameToFoliageIndex = fd.get_name_to_foliage_index_map();
    auto itr = nameToFoliageIndex.find(foliageName);
    if(itr == nameToFoliageIndex.end()) {
        logger::log_error("Could not find entry for foliage: "+foliageName);
    }
    return itr->second;
}

int toMI(std::string foliageName, FoliageDefinitions& fd) {
    int foliageIndex = toFI(foliageName, fd);
    auto fiToFIIndex = fd.foliageInfoElements;
    return fiToFIIndex[foliageIndex].mapIndex;
}


TEST_CASE("Foliage definitions correct.", "[foliage_definitions]") {

    FoliageDefinitions fd = FoliageDefinitions();

    SECTION("Name to map index translation.") {
        CHECK(toMI("NoSelection", fd) == FoliageHelpers::NO_SELECTION_INDEX);
        CHECK(toMI("NoFoliage", fd) == FoliageHelpers::NO_FOLIAGE_INDEX);
        CHECK(toMI("CliffsideDesertMiddle", fd) == 1301);
        CHECK(toMI("WallThreeUp", fd) == 1610);
        CHECK(toMI("BorderTreeDownRight", fd) == 1708);
        CHECK(toMI("TreeCliffedge", fd) == 50);
    }
}
