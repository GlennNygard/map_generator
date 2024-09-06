#pragma once

#include <unordered_map>
#include <unordered_set>

#include "foliage.h"
#include "disk_manager.h"


class BiomeFoliageInfo {
    public:

    std::vector<FoliageType> defaultSet;
    std::vector<FoliageType> walkableDefaultSet;

    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> PossibleTypes = {};
    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> WalkablePossibleTypes = {};
	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> StartPossibleTypes = {};

    std::unordered_map<FoliageType, std::unordered_set<FoliageType>> impossibleTypesDict;

    std::unordered_set<FoliageType> defaultHigherSet;

    std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> relationsDict;

	std::array<std::vector<std::pair<FoliageType, int>>, static_cast<size_t>(
        FoliageType::Foliage_MAX)> NeighbourBonus;

    // FoliageType UpRelations [Foliage_MAX][0];
    // FoliageType DownRelations [Foliage_MAX][0];
    // FoliageType LeftRelations [Foliage_MAX][0];
    // FoliageType RightRelations [Foliage_MAX][0];

    // FoliageType UpLeftRelations [Foliage_MAX][0];
    // FoliageType UpRightRelations [Foliage_MAX][0];
    // FoliageType DownLeftRelations [Foliage_MAX][0];
    // FoliageType DownRightRelations [Foliage_MAX][0];


    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> UpRelations;
    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> DownRelations;
    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> LeftRelations;
    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> RightRelations;

    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> UpLeftRelations;
    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> UpRightRelations;
    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> DownLeftRelations;
    std::array<std::vector<FoliageType>, static_cast<size_t>(FoliageType::Foliage_MAX)> DownRightRelations;


    BiomeFoliageInfo();

    void Setup(
			std::unordered_map<FoliageType, int> allowedTypes,
			std::unordered_map<FoliageType, int> walkableAllowedTypes,
			std::string relationsFile);

    template<typename T, typename V>
    std::vector<T> keysFromMap(std::unordered_map<T, V> map);

	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> GetNewDefaultData();

	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> GetNewWalkableData();

	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> GetNewEmptyData();
};
