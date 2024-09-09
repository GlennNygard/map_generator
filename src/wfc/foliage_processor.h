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
		const LevelValues LevelValues);


private:

	bool m_verboseLogging;
	bool m_issuesFound;

	BiomeFoliageInfo m_biomeFoliageInfo;

	int m_recursionCount = 0;
	int m_emptyRecursionCount = 0;
	int m_emptyRecursionEarlyCount = 0;

	std::chrono::milliseconds get_time();
	float format_time(long count);

	void prepare_full_data(
		Matrix<FoliageData> &fullFoliageDataMap,
        const LevelValues levelValues);

	void prepare_section_data(
        Matrix<FoliageType> &foliageMap,
        Matrix<FoliageData> &foliageDataMap,
        Matrix<char> &requiresPropagationMap,
        const Matrix<FoliageData> &fullFoliageDataMap,
        const Vector2Int currentSectionPos);

	void write_to_full_map(
        const Matrix<FoliageType> &resultsFoliageMap,
		Matrix<FoliageData> &resultsFoliageDataMap,
        Matrix<FoliageType> &fullFoliageMap,
		Matrix<FoliageData> &fullFoliageDataMap,
        Vector2Int sectionPos);

	void clear_subsection_from_full_grid(
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
