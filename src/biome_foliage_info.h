#pragma once

#include <unordered_map>
#include <unordered_set>

#include "foliage.h"
#include "disk_manager.h"


class BiomeFoliageInfo {
    public:

    std::vector<FoliageType> defaultSet;
    std::vector<FoliageType> walkableDefaultSet;

    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> possibleTypes = {};
    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> walkablePossibleTypes = {};
	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> startPossibleTypes = {};

    std::unordered_map<FoliageType, std::unordered_set<FoliageType>> impossibleTypesDict;

    std::unordered_set<FoliageType> defaultHigherSet;

    std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> relationsDict;

	std::array<std::vector<std::pair<FoliageType, int>>, static_cast<size_t>(
        FoliageType::Foliage_MAX)> neighbourBonus;

    std::vector<std::vector<FoliageType>> upRelations;
    std::vector<std::vector<FoliageType>> downRelations;
    std::vector<std::vector<FoliageType>> leftRelations;
    std::vector<std::vector<FoliageType>> rightRelations;

    std::vector<std::vector<FoliageType>> upLeftRelations;
    std::vector<std::vector<FoliageType>> upRightRelations;
    std::vector<std::vector<FoliageType>> downLeftRelations;
    std::vector<std::vector<FoliageType>> downRightRelations;


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
