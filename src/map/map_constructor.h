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
        bool verboseLogging);

    std::optional<MapObject> CreateMap(int currentIndex);

private:

    const bool m_verboseLogging;
	const LevelValues m_levelValues;

	BiomeInfoVariant m_biomeFoliageInfo;
    std::unique_ptr<FoliageDefinitions> m_foliageDefinitions;

    BiomeInfoVariant CreateBiomeInfo(LevelBiome biome, FoliageDefinitions& foliageDefinitions);


    std::pair<MapObject, bool> ConstructRandomMap(int levelSeed=-1);

    void SetMapBorders(
            Matrix<int> &foliageMap,
            int borderCount);

    Matrix<int> RandomFillSection(
            int sectionCountX,
            int sectionCountY,
            int randomFillPercent);

    std::pair<Matrix<FoliageType>, bool> CreateRandomFoliageMap();

    void CarveMainRoom(
        Matrix<int> &foliageMap,
        const Vector2Int worldPos,
        const float nodeDiameter,
        const int sectionCountX,
        const int sectionCountY);
};
