#pragma once

#include "matrix.h"
#include "foliage.h"
#include "gmath.h"
#include "map_definitions.h"
#include "foliage_processor.h"

class MapConstructor {

public:

    MapConstructor(
        const LevelValues& levelValues,
        FoliageDefinitions& foliageDefinitions,
        bool verboseLogging);

    std::optional<MapObject> create_map(int currentIndex);


private:

    std::unique_ptr<FoliageProcessor> m_foliageProcessor;
    std::unique_ptr<FoliageDefinitions> m_foliageDefinitions;
    LevelValues m_levelValues;

    std::pair<MapObject, bool> construct_random_map(int levelSeed=-1);

    void set_map_borders(
            Matrix<int> &foliageMap,
            int borderCount);

    Matrix<int> random_fill_section(
            int sectionCountX,
            int sectionCountY,
            int randomFillPercent);

    std::pair<Matrix<FoliageType>, bool> create_random_foliage_map();

    void carve_main_room(
        Matrix<int> &foliageMap,
        const Vector2Int worldPos,
        const float nodeDiameter,
        const int sectionCountX,
        const int sectionCountY);
};
