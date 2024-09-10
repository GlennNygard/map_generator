#include <unordered_map>
#include <unordered_set>
#include <expected>

#include "biome_foliage_info.h"
#include "foliage.h"
#include "disk_manager.h"



BiomeFoliageInfo::BiomeFoliageInfo() {
    impossibleTypesDict = std::unordered_map<int, std::unordered_set<int>>();
    defaultHigherSet = std::unordered_set<int>();
    relationsDict = std::unordered_map<int, std::unordered_map<int, int>>();

    walkablePossibleTypes = {};
    startPossibleTypes = {};
    possibleTypes = {};
}

const void BiomeFoliageInfo::setup(
        std::unordered_map<int, int> allowedTypes,
        std::unordered_map<int, int> walkableAllowedTypes,
        std::string relationsFile) {

    DiskManager mdm = DiskManager();
    auto relationsPath = mdm.get_relational_map_path(relationsFile);
    auto mapObjectOptional = mdm.load_map_object(relationsPath);
    if(!mapObjectOptional) {
        std::cerr << "Could not load relations file: "+relationsPath << std::endl;
        return;
    }
    auto mapObject = *mapObjectOptional;

    const size_t foliageCount = foliagedef::get_foliage_definitions().get_foliage_count();

    neighbourBonus = std::vector<std::vector<std::pair<int, int>>>(foliageCount);
    std::vector<int> defaultSet (foliageCount);
    std::vector<int> walkableDefaultSet (foliageCount);

    std::unordered_map<int, std::unordered_map<int, int>> upRelationsDict = {};
    std::unordered_map<int, std::unordered_map<int, int>> downRelationsDict = {};
    std::unordered_map<int, std::unordered_map<int, int>> leftRelationsDict = {};
    std::unordered_map<int, std::unordered_map<int, int>> rightRelationsDict = {};

    std::unordered_map<int, std::unordered_map<int, int>> upLeftRelationsDict = {};
    std::unordered_map<int, std::unordered_map<int, int>> upRightRelationsDict = {};
    std::unordered_map<int, std::unordered_map<int, int>> downLeftRelationsDict = {};
    std::unordered_map<int, std::unordered_map<int, int>> downRightRelationsDict = {};


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


                    // Dictionary<int, Dictionary<int, int>> relationsDict = null;
                    std::unordered_map<
                        int, std::unordered_map<int, int>>* relationsDict;
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
                    auto valueDictIter = relationsDict->find(currentFoliageType);
                    if(valueDictIter != relationsDict->end()) {
                        valueDict = valueDictIter->second;
                    }
                    bool alreadyContains = valueDict.contains(neighbourFoliageType);
                    if(alreadyContains) {
                        continue;
                    }
                    valueDict[neighbourFoliageType] = 1;
                    (*relationsDict)[currentFoliageType] = valueDict;

                    auto allowedTypesIter = allowedTypes.find(currentFoliageType);
                    if(allowedTypesIter != allowedTypes.end()) {
                        int priority = allowedTypesIter->second;
                        if(possibleTypes[static_cast<int>(currentFoliageType)] <= 0) {
                            possibleTypes[static_cast<int>(currentFoliageType)] = priority;
                            defaultSet[defaultIndex] = currentFoliageType;
                            defaultIndex++;
                        }
                    }

                    // if(walkableAllowedTypes.TryGetValue(currentFoliageType, out int wPriority)) {
                    auto walkableAllowedTypesIter = walkableAllowedTypes.find(currentFoliageType);
                    if(walkableAllowedTypesIter != walkableAllowedTypes.end()) {
                        int priority = walkableAllowedTypesIter->second;
                        if(walkablePossibleTypes[static_cast<int>(currentFoliageType)] <= 0) {
                            walkablePossibleTypes[static_cast<int>(currentFoliageType)] = priority;
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

    // const int foliageLength = static_cast<int>(FoliageHelpers::MAX_FOLIAGE_COUNT);

    upRelations = std::vector<std::vector<int>> (foliageCount);
    downRelations = std::vector<std::vector<int>> (foliageCount);
    leftRelations = std::vector<std::vector<int>> (foliageCount);
    rightRelations = std::vector<std::vector<int>> (foliageCount);

    upLeftRelations = std::vector<std::vector<int>> (foliageCount);
    upRightRelations = std::vector<std::vector<int>> (foliageCount);
    downLeftRelations = std::vector<std::vector<int>> (foliageCount);
    downRightRelations = std::vector<std::vector<int>> (foliageCount);


    for(auto kvp : upRelationsDict) {
        upRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
    for(auto kvp : downRelationsDict) {
        downRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
    for(auto kvp : leftRelationsDict) {
        leftRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
    for(auto kvp : rightRelationsDict) {
        rightRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }

    for(auto kvp : upLeftRelationsDict) {
        upLeftRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
    for(auto kvp : upRightRelationsDict) {
        upRightRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
    for(auto kvp : downLeftRelationsDict) {
        downLeftRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
    for(auto kvp : downRightRelationsDict) {
        downRightRelations[static_cast<int>(kvp.first)] = keys_from_map(kvp.second);
    }
}

std::pair<std::vector<std::vector<int>>*, Direction> BiomeFoliageInfo::get_relations_from_nodes(
        Vector2Int lastNodePos, Vector2Int currentNodePos) {
    // Check for up relations.
    if(currentNodePos.x == lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        return std::make_pair(&upRelations, Direction::DirectionUp);
        // lastNodeDirectionalRelations = &foliageInfo.upRelations;
        // direction = Direction::DirectionUp;
    }
    // Check for down relations.
    else if(currentNodePos.x == lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        return std::make_pair(&downRelations, Direction::DirectionDown);
        // lastNodeDirectionalRelations = &foliageInfo.downRelations;
        // direction = Direction::DirectionDown;
    }
    // We have moved right-wise.
    // Check for left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y == lastNodePos.y) {
        return std::make_pair(&leftRelations, Direction::DirectionLeft);
        // lastNodeDirectionalRelations = &foliageInfo.leftRelations;
        // direction = Direction::DirectionLeft;
    }
    // Check for right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y == lastNodePos.y) {
        return std::make_pair(&rightRelations, Direction::DirectionRight);
        // lastNodeDirectionalRelations = &foliageInfo.rightRelations;
        // direction = Direction::DirectionRight;
    }

    // Check for down left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        return std::make_pair(&downLeftRelations, Direction::DirectionDownLeft);
        // lastNodeDirectionalRelations = &foliageInfo.downLeftRelations;
        // direction = Direction::DirectionDownLeft;
    }
    // Check for down right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        return std::make_pair(&downRightRelations, Direction::DirectionDownRight);
        // lastNodeDirectionalRelations = &foliageInfo.downRightRelations;
        // direction = Direction::DirectionDownRight;
    }
    // Check for up left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        return std::make_pair(&upLeftRelations, Direction::DirectionUpLeft);
        // lastNodeDirectionalRelations = &foliageInfo.upLeftRelations;
        // direction = Direction::DirectionUpLeft;
    }
    // Check for up right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        return std::make_pair(&upRightRelations, Direction::DirectionUpRight);
        // lastNodeDirectionalRelations = &foliageInfo.upRightRelations;
        // direction = Direction::DirectionUpRight;
    }

    return std::make_pair(nullptr, Direction::DirectionNone);
}

template<typename T, typename V>
std::vector<T> BiomeFoliageInfo::keys_from_map(std::unordered_map<T, V> map) {
    std::vector<T> keys;
    keys.reserve(map.size());

    for(auto kv : map) {
        keys.push_back(kv.first);
    }
    return keys;
}
