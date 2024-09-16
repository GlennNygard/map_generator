#pragma once

#include <unordered_map>
#include <unordered_set>
#include <filesystem>

#include "foliage.h"
#include "disk_manager.h"
#include "foliage_definitions.h"
#include "logger.h"
#include "squashed_list.h"


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

using DefinitionsArray = std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT>;
using DefaultFoliageSquashedList = FoliageSquashedList<FoliageHelpers::MAX_FOLIAGE_COUNT, 10>;

// CRTP class.
// Forward declarations.
class GrassBiomeInfo;
class TestingBiomeInfo;

template <typename Derived>
class BiomeInfo {
public:

    DefaultFoliageSquashedList neighbourBonusList;

    BiomeInfo() {}
    BiomeInfo(FoliageDefinitions& foliageDefinitions) {}

    BiomeInfo(const BiomeInfo&) = default;
    // BiomeInfo& operator=(const BiomeInfo&) = delete;

    virtual void VFunc() = 0;

    // Move constructor.
    BiomeInfo(BiomeInfo&& other) noexcept
        : m_upRelations(std::move(other.m_upRelations)),
          m_downRelations(std::move(other.m_downRelations)),
          m_leftRelations(std::move(other.m_leftRelations)),
          m_rightRelations(std::move(other.m_rightRelations)),
          m_upLeftRelations(std::move(other.m_upLeftRelations)),
          m_upRightRelations(std::move(other.m_upRightRelations)),
          m_downLeftRelations(std::move(other.m_downLeftRelations)),
          m_downRightRelations(std::move(other.m_downRightRelations)),
          neighbourBonusList(std::move(other.neighbourBonusList)),
          m_foliagePriority(std::move(other.m_foliagePriority)),
          m_walkableFoliagePriority(std::move(other.m_walkableFoliagePriority)),
          m_startFoliagePriority(std::move(other.m_startFoliagePriority)) {}

    // Move assignment operator.
    BiomeInfo& operator=(BiomeInfo&& other) noexcept {
        if(this != &other) { // Self-assignment check
            m_upRelations = std::move(other.m_upRelations);
            m_downRelations = std::move(other.m_downRelations);
            m_leftRelations = std::move(other.m_leftRelations);
            m_rightRelations = std::move(other.m_rightRelations);
            m_upLeftRelations = std::move(other.m_upLeftRelations);
            m_upRightRelations = std::move(other.m_upRightRelations);
            m_downLeftRelations = std::move(other.m_downLeftRelations);
            m_downRightRelations = std::move(other.m_downRightRelations);
            neighbourBonusList = std::move(other.neighbourBonusList);
            m_foliagePriority = std::move(other.m_foliagePriority);
            m_walkableFoliagePriority = std::move(other.m_walkableFoliagePriority);
            m_startFoliagePriority = std::move(other.m_startFoliagePriority);
        }
        return *this;
    }


    template<typename T, typename V>
    const std::vector<T> KeysFromMap(const std::unordered_map<T, V>& map) const {
        std::vector<T> keys;
        keys.reserve(map.size());

        for(auto kv : map) {
            keys.push_back(kv.first);
        }
        return keys;
    }

    /**
     * All types available with the current biome. Use this when resetting
     * a tracker to the default values.
     */
    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& GetFoliagePriority() const {
        return m_foliagePriority;}

    /**
     * All types walkable available with the current biome.
     * Use this when resetting a tracker to the default values.
     */
    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& GetWalkableFoliagePriority() const {
        return m_walkableFoliagePriority;}

    const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& GetStartFoliagePriority() const {
        return m_startFoliagePriority;}

    const std::pair<std::reference_wrapper<DefinitionsArray>, Direction> GetRelationsFromNodes(
            Vector2Int lastNodePos, Vector2Int currentNodePos) {

        // Check for up relations.
        if(currentNodePos.x == lastNodePos.x && currentNodePos.y > lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_upRelations),
                Direction::DirectionUp);
        }
        // Check for down relations.
        else if(currentNodePos.x == lastNodePos.x && currentNodePos.y < lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_downRelations),
                Direction::DirectionDown);
        }
        // We have moved right-wise.
        // Check for left relations.
        else if(currentNodePos.x < lastNodePos.x && currentNodePos.y == lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_leftRelations),
                Direction::DirectionLeft);
        }
        // Check for right relations.
        else if(currentNodePos.x > lastNodePos.x && currentNodePos.y == lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_rightRelations),
                Direction::DirectionRight);
        }

        // Check for down left relations.
        else if(currentNodePos.x < lastNodePos.x && currentNodePos.y < lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_downLeftRelations),
                Direction::DirectionDownLeft);
        }
        // Check for down right relations.
        else if(currentNodePos.x > lastNodePos.x && currentNodePos.y < lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_downRightRelations),
                Direction::DirectionDownRight);
        }
        // Check for up left relations.
        else if(currentNodePos.x < lastNodePos.x && currentNodePos.y > lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_upLeftRelations),
                Direction::DirectionUpLeft);
        }
        // Check for up right relations.
        else if(currentNodePos.x > lastNodePos.x && currentNodePos.y > lastNodePos.y) {
            return std::make_pair(
                std::reference_wrapper<DefinitionsArray>(m_upRightRelations),
                Direction::DirectionUpRight);
        }

        return std::make_pair(
            std::reference_wrapper<DefinitionsArray>(m_upRelations),
            Direction::DirectionNone);
    }

protected:
    FoliageArray m_foliagePriority;
    FoliageArray m_walkableFoliagePriority;
    FoliageArray m_startFoliagePriority;

    DefinitionsArray m_upRelations;
    DefinitionsArray m_downRelations;
    DefinitionsArray m_leftRelations;
    DefinitionsArray m_rightRelations;

    DefinitionsArray m_upLeftRelations;
    DefinitionsArray m_upRightRelations;
    DefinitionsArray m_downLeftRelations;
    DefinitionsArray m_downRightRelations;

    const void Setup(
            const std::unordered_map<FoliageType, int>& allowedTypes,
            const std::unordered_map<FoliageType, int>& walkableAllowedTypes,
            FoliageDefinitions& foliageDefinitions,
            const std::filesystem::path& relationsPath) {

        
        struct TempRelation {
            bool hasData = false;
            std::unordered_map<FoliageType, int> map;
        };

        m_foliagePriority = {};
        m_walkableFoliagePriority = {};
        m_startFoliagePriority = {};

        DiskManager mdm = DiskManager(foliageDefinitions);
        auto mapObjectOptional = mdm.LoadMapObject(relationsPath);
        if(!mapObjectOptional) {
            logger::LogError("Could not load relations file: "+relationsPath.generic_string());
            return;
        }
        auto mapObject = *mapObjectOptional;

        const size_t foliageCount = foliageDefinitions.GetFoliageCount();

        std::vector<int> defaultSet (foliageCount);
        std::vector<int> walkableDefaultSet (foliageCount);

        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upRelationsDict = {};
        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downRelationsDict = {};
        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> leftRelationsDict = {};
        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> rightRelationsDict = {};

        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upLeftRelationsDict = {};
        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> upRightRelationsDict = {};
        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downLeftRelationsDict = {};
        std::array<TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> downRightRelationsDict = {};

        int defaultIndex = 0;
        int walkableDefaultIndex = 0;

        for(int x = 0; x < mapObject.lengthX; x++) {
            for(int y = 0; y < mapObject.lengthY; y++) {
                
                auto map = mapObject.map;
                auto mapNode = map[x][y];
                int currentFoliageType = mapNode.foliageType;

                if(currentFoliageType == FoliageHelpers::NO_SELECTION_INDEX) {
                    continue;
                }

                for(int nx = -1; nx <= 1; nx++) {
                    for(int ny = -1; ny <= 1; ny++) {

                        if(nx == 0 && ny == 0) {
                            continue;
                        }

                        int realNX = x + nx;
                        int realNY = y + ny;

                        if(realNX < 0 || realNX >= mapObject.lengthX ||
                                realNY < 0 || realNY >= mapObject.lengthY) {
                            
                            continue;
                        }

                        int neighbourFoliageType =
                            mapObject.map[realNX][realNY].foliageType;

                        if(neighbourFoliageType == FoliageHelpers::NO_SELECTION_INDEX) {
                            continue;
                        }

                        // We point to stack-allocated memory, so pointer does not have to be freed.
                        std::array<
                            TempRelation, FoliageHelpers::MAX_FOLIAGE_COUNT>* relationsDict;
                        Direction direction = Direction::DirectionNone;
                        if(nx == 0 && ny == 1) {
                            relationsDict = &upRelationsDict;
                            direction = Direction::DirectionUp;
                        }

                        if(nx == 0 && ny == -1) {
                            relationsDict = &downRelationsDict;
                            direction = Direction::DirectionDown;
                        }

                        if(nx == -1 && ny == 0) {
                            relationsDict = &leftRelationsDict;
                            direction = Direction::DirectionLeft;
                        }

                        if(nx == 1 && ny == 0) {
                            relationsDict = &rightRelationsDict;
                            direction = Direction::DirectionRight;
                        }

                        if(nx == -1 && ny == 1) {
                            relationsDict = &upLeftRelationsDict;
                            direction = Direction::DirectionUpLeft;
                        }

                        if(nx == 1 && ny == 1) {
                            relationsDict = &upRightRelationsDict;
                            direction = Direction::DirectionUpRight;
                        }

                        if(nx == -1 && ny == -1) {
                            relationsDict = &downLeftRelationsDict;
                            direction = Direction::DirectionDownLeft;
                        }

                        if(nx == 1 && ny == -1) {
                            relationsDict = &downRightRelationsDict;
                            direction = Direction::DirectionDownRight;
                        }

                        std::unordered_map<FoliageType, int> valueDict = {};
                        if((*relationsDict)[currentFoliageType].hasData) {
                            valueDict = (*relationsDict)[currentFoliageType].map;
                        }
                        bool alreadyContains = valueDict.contains(neighbourFoliageType);
                        if(alreadyContains) {
                            continue;
                        }
                        valueDict[neighbourFoliageType] = 1;
                        (*relationsDict)[currentFoliageType].map = valueDict;
                        (*relationsDict)[currentFoliageType].hasData = true;

                        auto allowedTypesIter = allowedTypes.find(currentFoliageType);
                        if(allowedTypesIter != allowedTypes.end()) {
                            int priority = allowedTypesIter->second;
                            if(m_foliagePriority[(currentFoliageType)] <= 0) {
                                m_foliagePriority[(currentFoliageType)] = priority;
                                defaultSet[defaultIndex] = currentFoliageType;
                                defaultIndex++;
                            }
                        }
                        auto walkableAllowedTypesIter = walkableAllowedTypes.find(currentFoliageType);
                        if(walkableAllowedTypesIter != walkableAllowedTypes.end()) {
                            int priority = walkableAllowedTypesIter->second;
                            if(m_walkableFoliagePriority[(currentFoliageType)] <= 0) {
                                m_walkableFoliagePriority[(currentFoliageType)] = priority;
                                walkableDefaultSet[walkableDefaultIndex] = currentFoliageType;
                                walkableDefaultIndex++;
                            }
                        }
                    }
                }
            }
        }

        defaultSet.resize(defaultIndex);
        walkableDefaultSet.resize(walkableDefaultIndex);

        for(size_t i = 0; i < FoliageHelpers::MAX_FOLIAGE_COUNT; i++) {
            if(upRelationsDict[i].hasData) {
                m_upRelations[i] = FoliageRelation(
                    KeysFromMap(upRelationsDict[i].map));
            }
            if(downRelationsDict[i].hasData) {
                m_downRelations[i] = FoliageRelation(
                    KeysFromMap(downRelationsDict[i].map));
            }
            if(leftRelationsDict[i].hasData) {
                m_leftRelations[i] = FoliageRelation(
                    KeysFromMap(leftRelationsDict[i].map));
            }
            if(rightRelationsDict[i].hasData) {
                m_rightRelations[i] = FoliageRelation(
                    KeysFromMap(rightRelationsDict[i].map));
            }
            if(upLeftRelationsDict[i].hasData) {
                m_upLeftRelations[i] = FoliageRelation(
                    KeysFromMap(upLeftRelationsDict[i].map));
            }
            if(upRightRelationsDict[i].hasData) {
                m_upRightRelations[i] = FoliageRelation(
                    KeysFromMap(upRightRelationsDict[i].map));
            }
            if(downLeftRelationsDict[i].hasData) {
                m_downLeftRelations[i] = FoliageRelation(
                    KeysFromMap(downLeftRelationsDict[i].map));
            }
            if(downRightRelationsDict[i].hasData) {
                m_downRightRelations[i] = FoliageRelation(
                    KeysFromMap(downRightRelationsDict[i].map));
            }
        }
    }
};

using BiomeInfoVariant = std::variant<GrassBiomeInfo, TestingBiomeInfo>;
