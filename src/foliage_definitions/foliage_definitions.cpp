#include "foliage_definitions.h"

const std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> FoliageDefinitions::get_foliage_definitions() {
    if(FoliageDefinitions::_foliageDict) {
        return *FoliageDefinitions::_foliageDict;
    }

    std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> foliageDict = {};

    foliagecommon::setup(foliageDict, 0);
    chasm::setup(foliageDict, 1400);
    wallhigher::setup(foliageDict, 1500);
    wall::setup(foliageDict, 1600);
    bordertree::setup(foliageDict, 1700);
    cliffsidehigh::setup(foliageDict, 1800);

    FoliageDefinitions::_foliageDict = foliageDict;
    return foliageDict;
}

std::optional<std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)>> FoliageDefinitions::_foliageDict;
