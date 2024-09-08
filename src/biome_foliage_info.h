#pragma once

#include <unordered_map>
#include <unordered_set>

#include "foliage.h"
#include "disk_manager.h"


class BiomeFoliageInfo {
public:

    std::vector<std::vector<FoliageType>> upRelations;
    std::vector<std::vector<FoliageType>> downRelations;
    std::vector<std::vector<FoliageType>> leftRelations;
    std::vector<std::vector<FoliageType>> rightRelations;

    std::vector<std::vector<FoliageType>> upLeftRelations;
    std::vector<std::vector<FoliageType>> upRightRelations;
    std::vector<std::vector<FoliageType>> downLeftRelations;
    std::vector<std::vector<FoliageType>> downRightRelations;

    template<typename T, typename V>
    std::vector<T> keys_from_map(std::unordered_map<T, V> map);

    const std::vector<FoliageType> get_default_set()& {return defaultSet;}
    const std::vector<FoliageType> get_default_walkable_set()& {return walkableDefaultSet;}

    const std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>& get_possible_types() {
        return possibleTypes;}

    const std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>& get_walkable_possible_types() {
        return walkablePossibleTypes;}

    const std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>& get_start_possible_types() {
        return startPossibleTypes;}

    BiomeFoliageInfo();

    std::unordered_map<FoliageType, std::unordered_set<FoliageType>> impossibleTypesDict;

    std::unordered_set<FoliageType> defaultHigherSet;

    std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> relationsDict;
	std::array<std::vector<std::pair<FoliageType, int>>, static_cast<size_t>(
        FoliageType::Foliage_MAX)> neighbourBonus;

    std::pair<std::vector<std::vector<FoliageType>>*, Direction> get_relations_from_nodes(
        Vector2Int lastNodePos, Vector2Int currentNodePos);

protected:
    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> possibleTypes = {};
    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> walkablePossibleTypes = {};
	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> startPossibleTypes = {};

    std::vector<FoliageType> defaultSet;
    std::vector<FoliageType> walkableDefaultSet;

    const void setup(
			std::unordered_map<FoliageType, int> allowedTypes,
			std::unordered_map<FoliageType, int> walkableAllowedTypes,
			std::string relationsFile);
};
