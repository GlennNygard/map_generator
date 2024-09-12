#pragma once

#include <queue>

#include "map_definitions.h"
#include "foliage_data.h"
#include "foliage.h"
#include "biome_foliage_info.h"


class FoliageProcessor {

public:

    FoliageProcessor(
		BiomeFoliageInfo biomeFoliageInfo,
		LevelValues levelValues,
		bool verboseLogging);

	std::pair<Matrix<int>, bool> mark_foliage_nodes();

private:

	bool m_verboseLogging;
	bool m_issuesFound;

	BiomeFoliageInfo m_biomeFoliageInfo;
	LevelValues m_levelValues;

	int m_recursionCount;
	int m_emptyRecursionCount;
	int m_emptyRecursionEarlyCount;

	std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> m_stillPossibleBuffer;

	std::chrono::milliseconds get_time();
	float format_time(long count);

	void prepare_full_data(
		Matrix<FoliageData>& fullFoliageDataMap,
        const LevelValues& levelValues);

	void prepare_section_data(
        Matrix<int> &foliageMap,
        Matrix<FoliageData> &foliageDataMap,
        Matrix<char> &requiresPropagationMap,
        const Matrix<FoliageData> &fullFoliageDataMap,
        const Vector2Int currentSectionPos);

	void write_to_full_map(
        const Matrix<int> &resultsFoliageMap,
		Matrix<FoliageData> &resultsFoliageDataMap,
        Matrix<int> &fullFoliageMap,
		Matrix<FoliageData> &fullFoliageDataMap,
        Vector2Int sectionPos);

	void clear_subsection_from_full_grid(
		Matrix<int>& fullFoliageMap,
		Matrix<FoliageData>& fullFoliageDataMap,
		const LevelValues& levelValues,
		const Vector2Int sectionPos);

    std::optional<std::pair<Matrix<int>, Matrix<FoliageData>>> process_subsection(
		Matrix<int>& foliageMap,
		Matrix<FoliageData>& foliageDataMap);

	Vector2Int get_next_best_node(
		const std::unordered_set<Vector2Int>& remainingNodes,
		Matrix<FoliageData>& foliageDataGrid);

	bool assign_foliage(
		const Vector2Int currentNodePos,
		const int newType,
		BiomeFoliageInfo& foliageInfo,
		Matrix<int>& foliageMap,
		Matrix<FoliageData>& foliageDataMap);

	template<size_t foliageCount>
	bool update_possible_types(
		const Vector2Int initialCurrentNodePos,
		const std::array<int, foliageCount>& remainingPossibleTypes,
		BiomeFoliageInfo& foliageInfo,
		Matrix<int>& foliageMap,
		Matrix<FoliageData>& foliageDataMap);

	template<size_t foliageCount>
	bool update_possible_types_recursively(
		const Vector2Int lastNodePos,
		const Vector2Int currentNodePos,
		BiomeFoliageInfo& foliageInfo,
		const std::array<int, foliageCount>& lastNodePossibleTypes,
		Matrix<int>& foliageMap,
		Matrix<FoliageData>& foliageDataMap,
		Matrix<int>& propagatedNodes,
		std::queue<std::pair<Vector2Int,Vector2Int>>& actionQueue);

	template<size_t foliageCount>
	int make_selection(
		const std::array<int, foliageCount>& possibleTypes,
		const std::array<int, foliageCount>& mainWeights);
};
