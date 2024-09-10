#pragma once

#include <unordered_map>
#include <unordered_set>

#include "foliage.h"
#include "disk_manager.h"
#include "foliage_definitions.h"
#include "logger.h"


class BiomeFoliageInfo {
public:

    std::vector<std::vector<int>> upRelations;
    std::vector<std::vector<int>> downRelations;
    std::vector<std::vector<int>> leftRelations;
    std::vector<std::vector<int>> rightRelations;

    std::vector<std::vector<int>> upLeftRelations;
    std::vector<std::vector<int>> upRightRelations;
    std::vector<std::vector<int>> downLeftRelations;
    std::vector<std::vector<int>> downRightRelations;

    template<typename T, typename V>
    std::vector<T> keys_from_map(std::unordered_map<T, V> map);

    /**
     * All types available with the current biome. Use this when resetting
     * a tracker to the default values.
     */
    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& get_possible_types() {
        return possibleTypes;}

    /**
     * All types walkable available with the current biome.
     * Use this when resetting a tracker to the default values.
     */
    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& get_walkable_possible_types() {
        return walkablePossibleTypes;}

    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& get_start_possible_types() {
        return startPossibleTypes;}

    BiomeFoliageInfo();

    std::unordered_map<int, std::unordered_set<int>> impossibleTypesDict;

    std::unordered_set<int> defaultHigherSet;

    std::unordered_map<int, std::unordered_map<int, int>> relationsDict;
	std::vector<std::vector<std::pair<int, int>>> neighbourBonus;

    std::pair<std::vector<std::vector<int>>*, Direction> get_relations_from_nodes(
        Vector2Int lastNodePos, Vector2Int currentNodePos);

protected:
    std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> possibleTypes;
    std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> walkablePossibleTypes;
	std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> startPossibleTypes;

    const void setup(
			std::unordered_map<int, int> allowedTypes,
			std::unordered_map<int, int> walkableAllowedTypes,
			std::string relationsFile);

    const int toFI(std::string foliageName) {
        auto nameToFoliageIndex = foliagedef::get_foliage_definitions().get_name_to_foliage_index_map();
        auto itr = nameToFoliageIndex.find(foliageName);
        if(itr == nameToFoliageIndex.end()) {
            logger::log_error("Could not find entry for foliage: "+foliageName);
        }
        return itr->second;
    }
};
