#pragma once

#include <queue>

#include "map_definitions.h"
#include "foliage_data.h"
#include "foliage.h"
#include "biome_foliage_info.h"


class FoliageProcessor {

public:

    FoliageProcessor(BiomeFoliageInfo biomeFoliageInfo, bool verboseLogging);

	std::pair<Matrix<FoliageType>, bool> mark_foliage_nodes(
		LevelValues LevelValues
		// int gridStartX, int gridStartY,
		// int gridEndX, int gridEndY
		);


private:

	const bool RUN_PRINTS = true;
	const bool RUN_GRID_PRINTS = true;

	bool m_verboseLogging;
	bool m_issuesFound;

	BiomeFoliageInfo _biomeFoliageInfo;

	int m_recursionCount = 0;
	int m_emptyRecursionCount = 0;
	int m_emptyRecursionEarlyCount = 0;

	std::chrono::milliseconds get_time();
	float format_time(long count);

	void clear_subsection(
			Matrix<FoliageType>& fullFoliageMap,
			Matrix<FoliageData>& fullFoliageDataMap,
			const Vector2Int sectionPos);

    std::optional<std::pair<Matrix<FoliageType>, Matrix<FoliageData>>> process_subsection(
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap);

	Vector2Int get_next_best_node(
			std::unordered_set<Vector2Int> remainingNodes,
			Matrix<FoliageData>& foliageDataGrid);

	bool assign_foliage(
			const Vector2Int currentNodePos,
			const FoliageType newType,
			BiomeFoliageInfo& foliageInfo,
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap);

	template<size_t size>
	bool update_possible_types(
			const Vector2Int initialCurrentNodePos,
			const std::array<int, size>& remainingPossibleTypes,
			BiomeFoliageInfo& foliageInfo,
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap);

	template<size_t size>
	bool update_possible_types_recursively(
			const Vector2Int lastNodePos,
			const Vector2Int currentNodePos,
			BiomeFoliageInfo& foliageInfo,
			const std::array<int, size>& lastNodePossibleTypes,
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap,
			Matrix<int>& propagatedNodes,
			std::queue<std::pair<Vector2Int,Vector2Int>>& actionQueue);

	template<size_t size>
	FoliageType make_selection(
		const std::array<int, size>& possibleTypes,
		const std::array<int, size>& mainWeights);
};
