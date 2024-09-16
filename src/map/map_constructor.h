#pragma once

#include "matrix.h"
#include "foliage.h"
#include "gmath.h"
#include "map_definitions.h"
#include "foliage_definitions.h"
#include "biome_info_import.h"


class MapConstructor {

public:

    MapConstructor(
        const LevelValues& levelValues,
        FoliageDefinitions& foliageDefinitions,
        const int seed);

    std::optional<MapObject> CreateMap(int currentIndex);

private:

	const LevelValues m_levelValues;
    const int m_overrideSeed;

	BiomeInfoVariant m_biomeFoliageInfo;
    std::unique_ptr<FoliageDefinitions> m_foliageDefinitions;

    BiomeInfoVariant CreateBiomeInfo(LevelBiome biome, FoliageDefinitions& foliageDefinitions);

    std::pair<MapObject, bool> ConstructRandomMap(int levelSeed);
    std::pair<Matrix<FoliageType>, bool> CreateRandomFoliageMap();
};
