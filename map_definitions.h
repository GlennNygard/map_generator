#pragma once

#include <unordered_map>

#include "foliage/foliage.h"
#include "gmath.h"


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


	static const std::unordered_map<MapSize, Vector2Int> mapSizeMappings;
};
