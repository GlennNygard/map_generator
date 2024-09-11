#pragma once

#include <unordered_map>
#include <unordered_set>

#include "foliage.h"
#include "disk_manager.h"
#include "foliage_definitions.h"
#include "logger.h"


struct FoliageNeighbourBonus {

    FoliageNeighbourBonus() = default;

    FoliageNeighbourBonus(std::vector<std::pair<int, int>> neighbourBonus) {
        m_neighbourBonus = neighbourBonus;
    }

    // Getter.
    std::pair<int, int> operator [](int i) const {
        return m_neighbourBonus[i];}
    // Setter.
    std::pair<int, int> & operator [](int i) {
        return m_neighbourBonus[i];}

    size_t size() const {
        return m_neighbourBonus.size();
    }

private:
    std::vector<std::pair<int, int>> m_neighbourBonus;
};

struct FoliageRelation {

    FoliageRelation() = default;

    FoliageRelation(std::vector<int> relations) {
        m_relations = relations;
    }

    // Getter.
    int operator [](int i) const {
        return m_relations[i];}
    // Setter.
    int & operator [](int i) {
        return m_relations[i];}

    size_t size() const {
        return m_relations.size();
    }

private:
    std::vector<int> m_relations;
};

class BiomeFoliageInfo {
public:

    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> leftRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> rightRelations;

    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upLeftRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upRightRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downLeftRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downRightRelations;

    std::unordered_map<int, std::unordered_set<int>> impossibleTypesDict;

    std::unordered_set<int> defaultHigherSet;

    std::unordered_map<int, std::unordered_map<int, int>> relationsDict;
	std::vector<FoliageNeighbourBonus> neighbourBonusList;

    BiomeFoliageInfo();

    template<typename T, typename V>
    std::vector<T> keys_from_map(const std::unordered_map<T, V>& map);

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

    std::pair<std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT>*, Direction> get_relations_from_nodes(
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
