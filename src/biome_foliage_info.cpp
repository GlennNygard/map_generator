#include <unordered_map>
#include <unordered_set>
#include <expected>

#include "biome_foliage_info.h"
#include "foliage.h"
#include "disk_manager.h"
#include "logger.h"



BiomeFoliageInfo::BiomeFoliageInfo() {
    impossibleTypesDict = std::unordered_map<int, std::unordered_set<int>>();
    defaultHigherSet = std::unordered_set<int>();
    relationsDict = std::unordered_map<int, std::unordered_map<int, int>>();

    walkablePossibleTypes = {};
    possibleTypes = {};
    startPossibleTypes = {};
}

const void BiomeFoliageInfo::setup(
        std::unordered_map<int, int> allowedTypes,
        std::unordered_map<int, int> walkableAllowedTypes,
        std::string relationsFile) {

    struct TempRelation {
        bool hasData = false;
        std::unordered_map<int, int> map;
    };

    DiskManager mdm = DiskManager();
    auto relationsPath = mdm.get_relational_map_path(relationsFile);
    auto mapObjectOptional = mdm.load_map_object(relationsPath);
    if(!mapObjectOptional) {
        logger::log_error("Could not load relations file: "+relationsPath);
        return;
    }
    auto mapObject = *mapObjectOptional;

    const size_t foliageCount = foliagedef::get_foliage_definitions().get_foliage_count();

    neighbourBonusList = std::vector<FoliageNeighbourBonus>(foliageCount);
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

                    std::unordered_map<int, int> valueDict = {};
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
                        if(possibleTypes[(currentFoliageType)] <= 0) {
                            possibleTypes[(currentFoliageType)] = priority;
                            defaultSet[defaultIndex] = currentFoliageType;
                            defaultIndex++;
                        }
                    }
                    auto walkableAllowedTypesIter = walkableAllowedTypes.find(currentFoliageType);
                    if(walkableAllowedTypesIter != walkableAllowedTypes.end()) {
                        int priority = walkableAllowedTypesIter->second;
                        if(walkablePossibleTypes[(currentFoliageType)] <= 0) {
                            walkablePossibleTypes[(currentFoliageType)] = priority;
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
            upRelations[i] = FoliageRelation(
                keys_from_map(upRelationsDict[i].map));
        }
        if(downRelationsDict[i].hasData) {
            downRelations[i] = FoliageRelation(
                keys_from_map(downRelationsDict[i].map));
        }
        if(leftRelationsDict[i].hasData) {
            leftRelations[i] = FoliageRelation(
                keys_from_map(leftRelationsDict[i].map));
        }
        if(rightRelationsDict[i].hasData) {
            rightRelations[i] = FoliageRelation(
                keys_from_map(rightRelationsDict[i].map));
        }
        if(upLeftRelationsDict[i].hasData) {
            upLeftRelations[i] = FoliageRelation(
                keys_from_map(upLeftRelationsDict[i].map));
        }
        if(upRightRelationsDict[i].hasData) {
            upRightRelations[i] = FoliageRelation(
                keys_from_map(upRightRelationsDict[i].map));
        }
        if(downLeftRelationsDict[i].hasData) {
            downLeftRelations[i] = FoliageRelation(
                keys_from_map(downLeftRelationsDict[i].map));
        }
        if(downRightRelationsDict[i].hasData) {
            downRightRelations[i] = FoliageRelation(
                keys_from_map(downRightRelationsDict[i].map));
        }
    }
}

std::pair<std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT>*, Direction> BiomeFoliageInfo::get_relations_from_nodes(
        Vector2Int lastNodePos, Vector2Int currentNodePos) {
    // Check for up relations.
    if(currentNodePos.x == lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        return std::make_pair(&upRelations, Direction::DirectionUp);
    }
    // Check for down relations.
    else if(currentNodePos.x == lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        return std::make_pair(&downRelations, Direction::DirectionDown);
    }
    // We have moved right-wise.
    // Check for left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y == lastNodePos.y) {
        return std::make_pair(&leftRelations, Direction::DirectionLeft);
    }
    // Check for right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y == lastNodePos.y) {
        return std::make_pair(&rightRelations, Direction::DirectionRight);
    }

    // Check for down left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        return std::make_pair(&downLeftRelations, Direction::DirectionDownLeft);
    }
    // Check for down right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        return std::make_pair(&downRightRelations, Direction::DirectionDownRight);
    }
    // Check for up left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        return std::make_pair(&upLeftRelations, Direction::DirectionUpLeft);
    }
    // Check for up right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        return std::make_pair(&upRightRelations, Direction::DirectionUpRight);
    }

    return std::make_pair(nullptr, Direction::DirectionNone);
}

template<typename T, typename V>
std::vector<T> BiomeFoliageInfo::keys_from_map(const std::unordered_map<T, V>& map) {
    std::vector<T> keys;
    keys.reserve(map.size());

    for(auto kv : map) {
        keys.push_back(kv.first);
    }
    return keys;
}
