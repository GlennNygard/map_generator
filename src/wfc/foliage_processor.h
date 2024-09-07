#pragma once

#include <iostream>
#include <random>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <chrono>
#include <cmath>
#include <limits>

#include "map_definitions.h"
#include "foliage_data.h"
#include "foliage.h"
#include "biome_foliage_info.h"


class FoliageProcessor {

public:

    FoliageProcessor(BiomeFoliageInfo biomeFoliageInfo, bool verboseLogging);

	std::pair<Matrix<FoliageType>, bool> mark_foliage_nodes(
		Matrix<int> fullNodeMap,
		int gridStartX, int gridStartY,
		int gridEndX, int gridEndY,
		int sideCountX, int sideCountY);


private:

	const bool RUN_PRINTS = true;
	const bool RUN_GRID_PRINTS = false;

	bool m_verboseLogging;
	bool m_issuesFound;

	BiomeFoliageInfo _biomeFoliageInfo;

	int m_recursionCount = 0;
	int m_emptyRecursionCount = 0;
	int m_emptyRecursionEarlyCount = 0;

	std::chrono::milliseconds getTime();

	void clear_subsection(
			Matrix<FoliageType>& fullFoliageMap,
			Matrix<FoliageData>& fullFoliageDataMap,
			const Vector2Int sectionPos,
			const int gridStartX,
			const int gridStartY);

	Vector2Int convert_to_global(int secX, int secY, int x, int y);

	template<size_t size>
	int getSum(std::array<int, size> arr);

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

	std::vector<Vector2Int> get_neighbours(Vector2Int gridPos, int mapSize);

	std::vector<Vector2Int> get_4_way_neighbours(Vector2Int gridPos, int mapSize);

	template<size_t size>
	FoliageType make_selection(
		const std::array<int, size>& possibleTypes,
		const std::array<int, size>& mainWeights);
};
