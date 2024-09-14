#pragma once

#include <unordered_map>

#include "foliage.h"
#include "gmath.h"


struct LevelValues {
    LevelBiome biome;

    int gridCountX;
    int gridCountY;

    /// @brief Count of full subsections in X direction.
    int subsectionCountX;
    /// @brief Count of full subsections in Y direction.
    int subsectionCountY;

    int subsectionBorderedSideCountX;
    int subsectionBorderedSideCountY;

    int borderSideCountX;
    int borderSideCountY;

    int subsectionBorderCountX;
    int subsectionBorderCountY;

    Vector2Int gridBoundsX;
    Vector2Int gridBoundsY;

    static const LevelValues CreateValues(
        Vector2Int sectionCount, LevelBiome biome);

    const int get_max_attempts() const {return subsectionCountX * subsectionCountY * 3;}
};

class MapDefinitions {

    public:

	static const int BORDER = 5;

    static const int SECTION_NODE_COUNT = 20;

	static const int SUBSECTION_BORDER = 3;
	static const int OVERLAP_BORDER = 3;
	static const int SUBSECTION_SIZE = 10;
	static const int SUBSECTION_FULL_SIZE = SUBSECTION_SIZE + SUBSECTION_BORDER*2;

	static const std::unordered_map<std::string, MapSize> stringSizeMappings;
	static const std::unordered_map<MapSize, Vector2Int> mapSizeMappings;

    static const LevelValues CreateLevelValues(MapSize mapSize);
};
