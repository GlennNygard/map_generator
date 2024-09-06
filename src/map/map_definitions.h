#pragma once

#include <unordered_map>

#include "foliage.h"
#include "gmath.h"


struct LevelValues {
    LevelBiome biome;

    int gridCountX;
    int gridCountY;

    Vector2Int gridBoundsX;
    Vector2Int gridBoundsY;

    LevelValues() {}

    static const LevelValues create_values(Vector2Int sectionCount, LevelBiome biome);
};

class MapDefinitions {

    public:

	const int BORDER = 5;

	static const int SUBSECTION_SIDE_COUNT_X = 20;
	static const int SUBSECTION_SIDE_COUNT_Y = 20;

	static const int SECTIONS_X = 8;
	static const int SECTIONS_Y = 8;

	static const int FULL_GRID_COUNT_X = SUBSECTION_SIDE_COUNT_X * SECTIONS_X;
	static const int FULL_GRID_COUNT_Y = SUBSECTION_SIDE_COUNT_Y * SECTIONS_Y;

	static const int SUBSECTION_BORDER = 3;
	static const int OVERLAP_BORDER = 3;
	static const int SUBSECTION_SIZE = 10;
	static const int SUBSECTION_FULL_SIZE = SUBSECTION_SIZE + SUBSECTION_BORDER*2;

	static const std::unordered_map<std::string, MapSize> stringSizeMappings;
	static const std::unordered_map<MapSize, Vector2Int> mapSizeMappings;

    static const LevelValues create_level_values(MapSize mapSize);
};
