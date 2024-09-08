
#include <unordered_map>

#include "map_definitions.h"
#include "foliage.h"
#include "gmath.h"

const std::unordered_map<std::string, MapSize> MapDefinitions::stringSizeMappings = {
    {"small", MapSize::MapSize_Small},
    {"medium", MapSize::MapSize_Medium},
    {"large", MapSize::MapSize_Large},
};

const std::unordered_map<MapSize, Vector2Int> MapDefinitions::mapSizeMappings = {
    {MapSize::MapSize_Smaller, Vector2Int(4,4)},
    {MapSize::MapSize_Small, Vector2Int(5,5)},
    {MapSize::MapSize_Medium, Vector2Int(7,7)},
    {MapSize::MapSize_Large, Vector2Int(9,9)},
    {MapSize::MapSize_VeryLarge, Vector2Int(11,11)},
    {MapSize::MapSize_IncrediblyLarge, Vector2Int(13,13)},
    {MapSize::MapSize_Massive, Vector2Int(15,15)},
};

const LevelValues MapDefinitions::create_level_values(MapSize mapSize) {
    LevelBiome mapBiome = LevelBiome::Grass;
    Vector2Int sectionCount = MapDefinitions::mapSizeMappings.at(mapSize);
    LevelValues levelValues = LevelValues::create_values(sectionCount, mapBiome);
    return levelValues;
}

const LevelValues LevelValues::create_values(Vector2Int sectionCount, LevelBiome biome) {
    auto lv = LevelValues();
    lv.biome = biome;

    lv.gridCountX = MapDefinitions::SECTION_NODE_COUNT * sectionCount.x;
    lv.gridCountY = MapDefinitions::SECTION_NODE_COUNT * sectionCount.y;

    lv.subsectionCountX = lv.gridCountX / MapDefinitions::SUBSECTION_SIZE;
    lv.subsectionCountY = lv.gridCountY / MapDefinitions::SUBSECTION_SIZE;

    lv.subsectionBorderedSideCountX = MapDefinitions::SUBSECTION_FULL_SIZE;
    lv.subsectionBorderedSideCountY = MapDefinitions::SUBSECTION_FULL_SIZE;

    lv.gridBoundsX = Vector2Int(0,lv.gridCountX);
    lv.gridBoundsY = Vector2Int(0,lv.gridCountY);

    return lv;
}
