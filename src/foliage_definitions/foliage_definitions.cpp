#include <filesystem>

#include "foliage_definitions.h"
#include "foliage_reader.h"


std::optional<FoliageDefinitions> FoliageDefinitions::foliageDefinitions;

void FoliageDefinitions::create_foliage_definitions() {
    m_nameToFoliageIndex = std::unordered_map<std::string, FoliageType>();

    std::vector<FoliageInfo> foliageList = {};

    std::filesystem::path resourcePath (RESOURCE_PATH);

    int currentFoliageIndex = 0;
    currentFoliageIndex = foliagereader::read(
        foliageList,
        resourcePath / "foliage_common.yaml",
        0, currentFoliageIndex);

    currentFoliageIndex = foliagereader::read(
        foliageList,
        resourcePath / "foliage_chasm.yaml",
        1400, currentFoliageIndex);

    currentFoliageIndex = foliagereader::read(
        foliageList,
        resourcePath / "foliage_wall_higher.yaml",
        1500, currentFoliageIndex);

    currentFoliageIndex = foliagereader::read(
        foliageList,
        resourcePath / "foliage_wall.yaml",
        1600, currentFoliageIndex);

    currentFoliageIndex = foliagereader::read(
        foliageList,
        resourcePath / "foliage_bordertree.yaml",
        1700, currentFoliageIndex);

    currentFoliageIndex = foliagereader::read(
        foliageList,
        resourcePath / "foliage_cliffside_high.yaml",
        1800, currentFoliageIndex);

    for(auto info : foliageList) {
        m_nameToFoliageIndex[info.foliageName] = info.foliageType;
    }

    logger::Log(std::format("Total foliage count: {}.",foliageList.size()));

    foliageInfoElements = foliageList;
}
