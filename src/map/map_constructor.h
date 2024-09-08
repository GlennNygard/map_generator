#pragma once

#include "matrix.h"
#include "foliage.h"
#include "gmath.h"
#include "map_definitions.h"
#include "foliage_processor.h"

class MapConstructor {

public:

    MapConstructor(LevelValues levelValues, bool verboseLogging);

    std::optional<MapObject> create_map(int currentIndex);


private:

    std::unique_ptr<FoliageProcessor> m_foliageProcessor;
    LevelValues m_levelValues;

    std::pair<MapObject, bool> construct_random_map(int levelSeed=-1);

    static Matrix<int> carve_main_room(
            Matrix<int> nodeMap, Vector2Int worldPos,
            float nodeDiameter,
            int sectionCountX, int sectionCountY);

    static Matrix<FoliageType> carve_main_room(
            Matrix<FoliageType> foliageMap,
            Vector2Int worldPos,
            float nodeDiameter,
            int sectionCountX, int sectionCountY);

    static Matrix<int> generate_section_walkable_representation(
            Matrix<int> nodeMap, int smoothIterations);

    static Matrix<int> smooth_section(Matrix<int> nodeMap);

    static int get_surrounding_wall_count(Matrix<int> nodeMap, int gridX, int gridY);


    static Matrix<FoliageType> set_map_borders(
            Matrix<FoliageType> foliageMap,
            int borderCount);

    std::pair<Matrix<FoliageType>, bool> create_random_foliage_map(
            LevelValues levelValues);

    static Matrix<int> random_fill_section(
            int sectionCountX,
            int sectionCountY,
            int randomFillPercent);
};
