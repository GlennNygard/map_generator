#pragma once

#include <queue>

#include "map_definitions.h"
#include "foliage_data.h"
#include "foliage.h"
#include "biome_foliage_info.h"


class FoliageProcessor {

public:

    FoliageProcessor(
		BiomeFoliageInfo&& biomeFoliageInfo,
		const LevelValues& levelValues,
		bool verboseLogging);

	std::pair<Matrix<FoliageType>, bool> ProcessNewMap();

private:

	bool m_verboseLogging;
	bool m_issuesFound;

	BiomeFoliageInfo m_biomeFoliageInfo;
	const LevelValues m_levelValues;
};
