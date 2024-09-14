#pragma once

#include <unordered_map>
#include <unordered_set>
#include <filesystem>

#include "foliage.h"
#include "disk_manager.h"
#include "foliage_definitions.h"
#include "logger.h"
#include "squashed_list.h"


struct FoliageNeighbourBonus {

    FoliageNeighbourBonus() = default;

    FoliageNeighbourBonus(std::vector<std::pair<int, int>> neighbourBonus) {
        m_neighbourBonus = neighbourBonus;
    }

    FoliageNeighbourBonus(std::vector<std::pair<int, int>>&& neighbourBonus) 
        : m_neighbourBonus(std::move(neighbourBonus)) {}

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

    FoliageRelation(std::vector<FoliageType> relations) 
        : m_relations(std::move(relations)) {}

    // Getter.
    FoliageType operator [](FoliageType i) const {
        return m_relations[i];}
    // Setter.
    FoliageType & operator [](FoliageType i) {
        return m_relations[i];}

    size_t size() const {
        return m_relations.size();
    }

private:
    std::vector<FoliageType> m_relations;
};

class BiomeFoliageInfo {
public:
    using FoliageSquashedList = FoliageSquashedList<FoliageHelpers::MAX_FOLIAGE_COUNT, 10>;

    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> leftRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> rightRelations;

    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upLeftRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upRightRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downLeftRelations;
    std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downRightRelations;

    FoliageSquashedList neighbourBonusList;

    BiomeFoliageInfo(FoliageDefinitions& foliageDefinitions) {}

    // Move constructor.
    BiomeFoliageInfo(BiomeFoliageInfo&& other) noexcept
        : upRelations(std::move(other.upRelations)),
          downRelations(std::move(other.downRelations)),
          leftRelations(std::move(other.leftRelations)),
          rightRelations(std::move(other.rightRelations)),
          upLeftRelations(std::move(other.upLeftRelations)),
          upRightRelations(std::move(other.upRightRelations)),
          downLeftRelations(std::move(other.downLeftRelations)),
          downRightRelations(std::move(other.downRightRelations)),
          neighbourBonusList(std::move(other.neighbourBonusList)),
          foliagePriority(std::move(other.foliagePriority)),
          walkableFoliagePriority(std::move(other.walkableFoliagePriority)),
          startFoliagePriority(std::move(other.startFoliagePriority)) {}

    // Move assignment operator.
    BiomeFoliageInfo& operator=(BiomeFoliageInfo&& other) noexcept {
        if(this != &other) { // Self-assignment check
            upRelations = std::move(other.upRelations);
            downRelations = std::move(other.downRelations);
            leftRelations = std::move(other.leftRelations);
            rightRelations = std::move(other.rightRelations);
            upLeftRelations = std::move(other.upLeftRelations);
            upRightRelations = std::move(other.upRightRelations);
            downLeftRelations = std::move(other.downLeftRelations);
            downRightRelations = std::move(other.downRightRelations);
            neighbourBonusList = std::move(other.neighbourBonusList);
            foliagePriority = std::move(other.foliagePriority);
            walkableFoliagePriority = std::move(other.walkableFoliagePriority);
            startFoliagePriority = std::move(other.startFoliagePriority);
        }
        return *this;
    }

    BiomeFoliageInfo(const BiomeFoliageInfo&) = default;
    BiomeFoliageInfo& operator=(const BiomeFoliageInfo&) = delete;


    template<typename T, typename V>
    std::vector<T> keys_from_map(const std::unordered_map<T, V>& map);

    /**
     * All types available with the current biome. Use this when resetting
     * a tracker to the default values.
     */
    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& GetFoliagePriority() {
        return foliagePriority;}

    /**
     * All types walkable available with the current biome.
     * Use this when resetting a tracker to the default values.
     */
    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& GetWalkableFoliagePriority() {
        return walkableFoliagePriority;}

    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& get_start_foliage_priority() {
        return startFoliagePriority;}

    virtual std::pair<std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT>*, Direction> get_relations_from_nodes(
        Vector2Int lastNodePos, Vector2Int currentNodePos);

protected:
    std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> foliagePriority;
    std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> walkableFoliagePriority;
	std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> startFoliagePriority;

    const void setup(
        std::unordered_map<FoliageType, int>& allowedTypes,
        std::unordered_map<FoliageType, int>& walkableAllowedTypes,
        FoliageDefinitions& foliageDefinitions,
        std::filesystem::path relationsPath);
};
