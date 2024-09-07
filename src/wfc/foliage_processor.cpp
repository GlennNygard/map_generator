
#include <iostream>
#include <random>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <chrono>
#include <cmath>
#include <limits>

#include "logger.h"
#include "foliage_processor.h"
#include "gmath.h"


FoliageProcessor::FoliageProcessor(BiomeFoliageInfo biomeFoliageInfo, bool verboseLogging) {
    m_issuesFound = false;
    _biomeFoliageInfo = biomeFoliageInfo;
    m_verboseLogging = verboseLogging;

    std::random_device dev;
    std::mt19937 rng(dev());
    // Distribution in range [1, 6].
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6);
}

std::chrono::milliseconds FoliageProcessor::getTime() {
    std::chrono::milliseconds startTime = duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return startTime;
}

std::pair<Matrix<FoliageType>, bool> FoliageProcessor::mark_foliage_nodes(
        Matrix<int> fullNodeMap,
        int gridStartX, int gridStartY,
        int gridEndX, int gridEndY,
        int sideCountX, int sideCountY) {

    Matrix<FoliageType> fullFoliageMap(
        MapDefinitions::FULL_GRID_COUNT_X, MapDefinitions::FULL_GRID_COUNT_Y);
    Matrix<FoliageData> fullFoliageDataMap(
        MapDefinitions::FULL_GRID_COUNT_X, MapDefinitions::FULL_GRID_COUNT_Y);

    int subsectionCountX = sideCountX/MapDefinitions::SUBSECTION_SIZE;
    int subsectionCountY = sideCountY/MapDefinitions::SUBSECTION_SIZE;

    int maxAttempts = subsectionCountX*subsectionCountY * 3;

    int overallAttempts = 0;
    int currentSubsectionAttempts = 0;
    int currentRetryAttempts = 0;

    int furthestIndex = 0;
    Vector2Int furthestSectionPos = {};

    // Local foliage maps.
    Matrix<FoliageType> foliageMap (
        MapDefinitions::SUBSECTION_FULL_SIZE,
        MapDefinitions::SUBSECTION_FULL_SIZE);
    Matrix<FoliageData> foliageDataMap (
        MapDefinitions::SUBSECTION_FULL_SIZE,
        MapDefinitions::SUBSECTION_FULL_SIZE);

    Matrix<int> requiresPropagationMap (
        MapDefinitions::SUBSECTION_FULL_SIZE,
        MapDefinitions::SUBSECTION_FULL_SIZE);

    const int foliageMapLengthX = MapDefinitions::SUBSECTION_FULL_SIZE;
    const int foliageMapLengthY = MapDefinitions::SUBSECTION_FULL_SIZE;

    std::vector<std::array<int,2>> forcedWalkableSubsections = {};

    int halfX = (subsectionCountX/2);
    int halfY = (subsectionCountY/2);

    Vector2Int rangeX = {1,1};
    Vector2Int rangeY = {1,1};
    if(subsectionCountX % 2 == 0) {
        rangeX.x = halfX-1;
        rangeX.y = halfX+1;
    }
    else {
        rangeX.x = halfX-1;
        rangeX.y = halfX+1;
    }

    if(subsectionCountY % 2 == 0) {
        rangeY.x = halfY-1;
        rangeY.y = halfY+1;
    }
    else {
        rangeY.x = halfY-1;
        rangeY.y = halfY+1;
    }

    for(int secX = rangeX.x; secX <= rangeX.y; secX++) {
        for(int secY = rangeY.x; secY <= rangeY.y; secY++) {
            forcedWalkableSubsections.push_back(std::array<int,2>{secX, secY});
        }
    }

    std::cout << std::format("Subsections: {},{}",subsectionCountX,subsectionCountY) << std::endl;

    int mapForcedWalkableSectionBorder = 2;
    for(int secX = 0; secX < subsectionCountX; secX++) {
        for(int secY = 0; secY < subsectionCountY; secY++) {
            if(secX > mapForcedWalkableSectionBorder && secX < subsectionCountX-mapForcedWalkableSectionBorder &&
                    secY > mapForcedWalkableSectionBorder && secY < subsectionCountY-mapForcedWalkableSectionBorder) {
                continue;
            }
            forcedWalkableSubsections.push_back(std::array<int,2>{secX, secY});
        }
    }

    for(auto secPos : forcedWalkableSubsections) {
        int secX = secPos[0];
        int secY = secPos[1];
        for(int x = 0; x < foliageMapLengthX; x++) {
            for(int y = 0; y < foliageMapLengthY; y++) {
                Vector2Int fullCoord = convert_to_global(secX, secY, x, y);
                int fullX = fullCoord.x;
                int fullY = fullCoord.y;
                if(fullX < 0 || fullY < 0 || fullX >= sideCountX || fullY >= sideCountY) {
                    continue;
                }
                fullFoliageDataMap[fullX][fullY].isWalkableOnly = true;
            }
        }
    }

    std::stack<Vector2Int> sectionStack = {};
    for(int secX = subsectionCountX-1; secX >= 0; secX--) {
        for(int secY = subsectionCountY-1; secY >= 0; secY--) {
            sectionStack.push(Vector2Int{secX, secY});
        }
    }

    std::vector<Vector2Int> retryList = {};
    int retryIndex = 0;
    int currentIndex = 0;


    double requiredTime = 0;
    long retryTime = 0;
    std::chrono::milliseconds startTime = duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());

    bool failed = false;
    while(sectionStack.size() > 0) {
        std::chrono::milliseconds time = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        Vector2Int currentSectionPos;
        if(retryList.size() > 0) {
            if(retryIndex >= retryList.size()) {
                if(RUN_PRINTS && m_verboseLogging) {
                    logger::log(
                        std::format(
                            "Current: {0}. Overall {1}. Retries {2}. Count {3}. Furthest {4}. Last current ind: {5}",
                            currentSubsectionAttempts, overallAttempts,
                            retryIndex, retryList.size(),
                            furthestIndex, currentIndex));
                }
            }
            currentSectionPos = retryList[retryIndex];
            retryIndex++;
        }
        else {
            currentSectionPos = sectionStack.top();
            sectionStack.pop();
        }
        int secX = currentSectionPos.x;
        int secY = currentSectionPos.y;
        currentIndex = subsectionCountX * secX + secY;

        if(RUN_PRINTS && m_verboseLogging) {
            logger::log(std::format(
                "STARTING ATTEMPT"
                "Section: {}."
                "Furthest: {}."
                "Remaining in stack: {}.",
                currentSectionPos.to_string(),
                furthestSectionPos.to_string(),
                sectionStack.size()));
        }

        m_recursionCount = 0;
        m_emptyRecursionCount = 0;
        m_emptyRecursionEarlyCount = 0;

        // GLOBAL MAP BORDERS.
        for(int x = 0; x < MapDefinitions::SUBSECTION_FULL_SIZE; x++) {
            for(int y = 0; y < MapDefinitions::SUBSECTION_FULL_SIZE; y++) {
                Vector2Int fullCoord = convert_to_global(secX, secY, x, y);
                int fullX = fullCoord.x;
                int fullY = fullCoord.y;

                // Reset main array.
                foliageMap[x][y] = FoliageType::Foliage_NoSelection;

                FoliageData foliageData = foliageDataMap[x][y];

                // Outside main map.
                if(fullX < 0 || fullY < 0 || fullX >= sideCountX || fullY >= sideCountY) {
                    foliageData.set_remaining_from_array(_biomeFoliageInfo.WalkablePossibleTypes);
                    foliageDataMap[x][y] = foliageData;
                    requiresPropagationMap[x][y] = true;
                    continue;
                }

                // We must have checked if we are outside the full map
                // before getting the fullNode.
                FoliageData fullNodeFoliageData = fullFoliageDataMap[fullX][fullY];
                foliageData.isWalkableOnly = fullNodeFoliageData.isWalkableOnly;

                bool forcedWalkable = false;
                if(fullNodeFoliageData.isWalkableOnly) {
                    forcedWalkable = true;
                }

                // Node has previous data.
                if(fullNodeFoliageData.hasData && fullNodeFoliageData.get_remaining_possible_types()) {
                    if(x < 1 || y < 1 || x >= MapDefinitions::SUBSECTION_FULL_SIZE-1 || y >= MapDefinitions::SUBSECTION_FULL_SIZE-1) {
                        foliageData.set_remaining_from_array((*fullNodeFoliageData.get_remaining_possible_types()));
                        foliageDataMap[x][y] = foliageData;
                        requiresPropagationMap[x][y] = true;
                        continue;
                    }
                }

                // Node does not have previous data.
                else {
                    // Constrain border edges.
                    if(x < MapDefinitions::SUBSECTION_BORDER || y < MapDefinitions::SUBSECTION_BORDER ||
                            x >= foliageMapLengthX-MapDefinitions::SUBSECTION_BORDER ||
                            y >= foliageMapLengthY-MapDefinitions::SUBSECTION_BORDER) {
                        foliageData.set_remaining_from_array(_biomeFoliageInfo.WalkablePossibleTypes);
                        requiresPropagationMap[x][y] = true;

                        foliageDataMap[x][y] = foliageData;
                        continue;
                    }
                }

                requiresPropagationMap[x][y] = forcedWalkable;

                if(forcedWalkable) {
                    foliageData.set_remaining_from_array(_biomeFoliageInfo.WalkablePossibleTypes);
                }
                else {
                    foliageData.set_remaining_from_array(_biomeFoliageInfo.PossibleTypes);
                }
                foliageDataMap[x][y] = foliageData;
            }
        }

        logger::log_grid(foliageDataMap, _biomeFoliageInfo.WalkablePossibleTypes);

        bool overallFailed = false;
        for(int i = 0; i < foliageMapLengthX * foliageMapLengthY; i++) {
            int x = i % foliageMapLengthX;
            int y = (int)std::floor((float)i / foliageMapLengthX);

            if(!requiresPropagationMap[x][y]) {
                continue;
            }
            Vector2Int node = {x,y};
            FoliageData nodeData = foliageDataMap[node.x][node.y];
            bool success = update_possible_types(
                node,
                (*nodeData.get_remaining_possible_types()),
                _biomeFoliageInfo,
                foliageMap,
                foliageDataMap);
            overallFailed = success ? overallFailed : true;
            if(overallFailed) {
                break;
            }
        }
        if(overallFailed) {
            logger::log_error("Initial propagation failed. Should not happen.");
        }

        auto resultsFoliageMap = Matrix<FoliageType>(
            MapDefinitions::SUBSECTION_FULL_SIZE,
            MapDefinitions::SUBSECTION_FULL_SIZE);
        auto resultsFoliageDataMap = Matrix<FoliageData>(
            MapDefinitions::SUBSECTION_FULL_SIZE,
            MapDefinitions::SUBSECTION_FULL_SIZE);

        logger::log_grid(foliageDataMap, _biomeFoliageInfo.WalkablePossibleTypes);
        if(!overallFailed) {

            auto processResult = process_subsection(
                foliageMap, foliageDataMap);
            if(processResult) {
                resultsFoliageMap = (*processResult).first;
                resultsFoliageDataMap = (*processResult).second;
            }
            else {
                overallFailed = true;
            }
        }


        // Generation failed.
        if(overallFailed) {
            overallAttempts++;
            currentSubsectionAttempts++;
            currentRetryAttempts++;
            if(RUN_PRINTS && m_verboseLogging) {
                std::cout <<
                    std::format(
                    "Processing subsection ({},{}) failed. Attempts: {}. Overall attempts: {}.",
                    secX, secY, currentSubsectionAttempts, overallAttempts) << std::endl;
            }
            if(overallAttempts >= maxAttempts) {
                sectionStack.push({secX, secY});
                failed = true;
                break;
            }
            // If we are on the furthest attempted section,
            // i.e the same as the one we failed.
            if(currentIndex > furthestIndex) {
                currentSubsectionAttempts = 0;
                currentRetryAttempts = 0;
                retryIndex = 0;
                retryList.clear();
                if(secX == 0) {
                    if(secY > 0) {
                        retryList.push_back({secX, secY-1});
                    }
                    retryList.push_back({secX, secY});
                    continue;
                }
                // The order the subsections are retried matters.
                // They must match the original execution,
                // otherwise the section seams won't be treated correctly.
                if(secY > 0) {
                    retryList.push_back({secX-1, secY-1});
                }
                retryList.push_back({secX-1, secY});
                if(secY < subsectionCountY-2) {
                    retryList.push_back({secX-1, secY+1});
                }
                if(secY > 0) {
                    retryList.push_back({secX, secY-1});
                }
                retryList.push_back({secX, secY});

                // Clear retried data.
                for(Vector2Int retryPos : retryList) {
                    clear_subsection(
                        fullFoliageMap, fullFoliageDataMap,
                        retryPos, gridStartX, gridStartY);
                }
            }
            else if(currentSubsectionAttempts >= 2) {
                currentSubsectionAttempts = 0;
                retryIndex = 0;

                // Clear retried data.
                for(Vector2Int retryPos : retryList) {
                    clear_subsection(
                        fullFoliageMap, fullFoliageDataMap,
                        retryPos, gridStartX, gridStartY);
                }
            }
            else {
                retryIndex = std::max(retryIndex-2, 0);
                clear_subsection(
                    fullFoliageMap, fullFoliageDataMap,
                    currentSectionPos, gridStartX, gridStartY);
                clear_subsection(
                    fullFoliageMap, fullFoliageDataMap,
                    retryList[retryIndex], gridStartX, gridStartY);
            }
            retryTime += (getTime() - time).count();
            continue;
        }

        // Generation succeeded.
        bool isFurthestIndex = currentIndex > furthestIndex;
        if(isFurthestIndex) {
            if(retryList.size() > 0) {
                retryIndex = 0;
                retryList = std::vector<Vector2Int>();
            }
            currentSubsectionAttempts = 0;
            currentRetryAttempts = 0;
            furthestIndex = currentIndex;
            furthestSectionPos = currentSectionPos;
        }

        if(RUN_GRID_PRINTS && m_verboseLogging) {
            std::cout << "Writing results: " << std::endl;
            logger::log_grid(resultsFoliageDataMap, _biomeFoliageInfo.WalkablePossibleTypes);
            logger::log_grid(resultsFoliageMap);
        }
        if(RUN_PRINTS && m_verboseLogging) {
            std::cout << std::format(
                "Recursion Count: {} / {} / {}",
                m_recursionCount,
                m_emptyRecursionCount,
                m_emptyRecursionEarlyCount) << std::endl;
        }

        // Write back to full map without borders.
        // We only write back data to the top left border nodes.
        for(int x = 0; x < foliageMapLengthX; x++) {
            for(int y = 0; y < foliageMapLengthY; y++) {
                Vector2Int fullCoord = convert_to_global(secX, secY, x, y);
                int fullX = fullCoord.x;
                int fullY = fullCoord.y;
                if(fullX < 0 || fullY < 0 || fullX >= fullFoliageMap.dim_a() || fullY >= fullFoliageMap.dim_b()) {
                    continue;
                }
                fullFoliageMap[fullX][fullY] = resultsFoliageMap[x][y];
                if(x >= 1 && y >= 1 &&
                        x < resultsFoliageDataMap.dim_a()-1 &&
                        y < resultsFoliageDataMap.dim_b()-1) {
                    fullFoliageDataMap[fullX][fullY].set_remaining_from_array(
                        (*resultsFoliageDataMap[x][y].get_remaining_possible_types()));
                }
            }
        }

        if(isFurthestIndex) {
            requiredTime += (getTime() - time).count();
        }
        else {
            retryTime += (getTime() - time).count();
        }
    }

    // If generation failed, we mark the remaining nodes as NoFoliage.
    // This is to help with debugging.
    if(failed) {
        while(sectionStack.size() > 0) {

            Vector2Int currentSectionPos = sectionStack.top();
            sectionStack.pop();

            int secX = currentSectionPos.x;
            int secY = currentSectionPos.y;

            for(int x = 0; x < foliageMapLengthX; x++) {
                for(int y = 0; y < foliageMapLengthY; y++) {
                    // (int fullX, int fullY) = convert_to_global(secX, secY, x, y);
                    Vector2Int fullCoord = convert_to_global(secX, secY, x, y);
                    int fullX = fullCoord.x;
                    int fullY = fullCoord.y;

                    if(fullX < 0 || fullY < 0 || fullX >= sideCountX || fullY >= sideCountY) {
                        continue;
                    }

                    if(fullFoliageMap[fullX][fullY] != FoliageType::Foliage_NoSelection) {
                        continue;
                    }

                    fullFoliageMap[fullX][fullY] = FoliageType::Foliage_CompletelyThrottenGround;

                }
            }
        }
        std::cout << "GENERATION FAILED." << std::endl;
    }

    long executionTime = requiredTime + retryTime;
    double timePerSubsection = (executionTime/(subsectionCountX*subsectionCountY));
    double effectiveExecutionTime = requiredTime;
    double effectiveTimePerSubsection = (effectiveExecutionTime/((subsectionCountX*subsectionCountY)-overallAttempts));

    std::cout <<
        std::format("Total sections: {}. ", subsectionCountX*subsectionCountY) <<
        std::format("Failed attempts: {}. ", overallAttempts)  <<
        std::format("Time: {} - {}. ", (getTime() - startTime).count(), startTime.count()) <<
        std::format("Time to execute. Effective: {}. All: {}. ",effectiveExecutionTime, executionTime) <<
        std::format("Avg time per subsection. Effective: {}. ",effectiveTimePerSubsection) <<
        std::format(" (All: {})",timePerSubsection) << std::endl;

    return std::pair(fullFoliageMap, !failed);
};

void FoliageProcessor::clear_subsection(
        Matrix<FoliageType>& fullFoliageMap,
        Matrix<FoliageData>& fullFoliageDataMap,
        const Vector2Int sectionPos,
        const int gridStartX,
        const int gridStartY) {

    for(int x = 0; x < MapDefinitions::SUBSECTION_FULL_SIZE; x++) {
        for(int y = 0; y < MapDefinitions::SUBSECTION_FULL_SIZE; y++) {
            Vector2Int fullCoord = convert_to_global(sectionPos.x, sectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;

            if(fullX < 0 || fullY < 0 || fullX >= MapDefinitions::FULL_GRID_COUNT_X || fullY >= MapDefinitions::FULL_GRID_COUNT_Y) {
                continue;
            }

            fullFoliageMap[fullX][fullY] = FoliageType::Foliage_NoSelection;

            if(x >= 1 && y >= 1 &&
                    x < MapDefinitions::SUBSECTION_FULL_SIZE-1 &&
                    y < MapDefinitions::SUBSECTION_FULL_SIZE-1) {
                // If we are within 1 tile border from the edge.
                if((x < MapDefinitions::SUBSECTION_BORDER ||
                        y < MapDefinitions::SUBSECTION_BORDER ||
                        y >= MapDefinitions::SUBSECTION_FULL_SIZE-MapDefinitions::SUBSECTION_BORDER) &&
                        // Exclude the right side border.
                        x < MapDefinitions::SUBSECTION_FULL_SIZE-MapDefinitions::SUBSECTION_BORDER) {
                    // If we are within a 1 tile border, but not within the proper border
                    // (right side border excluded)
                    if(fullFoliageDataMap[fullX][fullY].isWalkableOnly) {
                        fullFoliageDataMap[fullX][fullY].set_remaining_from_array(_biomeFoliageInfo.WalkablePossibleTypes);
                    }
                    else {
                        fullFoliageDataMap[fullX][fullY].set_remaining_from_array(_biomeFoliageInfo.PossibleTypes);
                    }
                }
                else {
                    fullFoliageDataMap[fullX][fullY].hasData = false;
                }
            }
            else if(x >= MapDefinitions::SUBSECTION_FULL_SIZE-1 && y >= 1 && y < MapDefinitions::SUBSECTION_FULL_SIZE-1) {
                fullFoliageDataMap[fullX][fullY].hasData = false;
            }
        }
    }
}

Vector2Int FoliageProcessor::convert_to_global(int secX, int secY, int x, int y) {
    int fullX = ((MapDefinitions::SUBSECTION_SIZE * secX) - MapDefinitions::SUBSECTION_BORDER) + x;
    int fullY = ((MapDefinitions::SUBSECTION_SIZE * secY) - MapDefinitions::SUBSECTION_BORDER) + y;
    return {fullX,fullY};
}

std::optional<std::pair<Matrix<FoliageType>, Matrix<FoliageData>>> FoliageProcessor::process_subsection(
        Matrix<FoliageType>& foliageMap,
        Matrix<FoliageData>& foliageDataMap
        ) {

    m_issuesFound = false;

    int sideCountX = MapDefinitions::SUBSECTION_FULL_SIZE;
    int sideCountY = MapDefinitions::SUBSECTION_FULL_SIZE;

    std::unordered_set<Vector2Int> remainingNodes = {};

    for(int x = 0; x < sideCountX; x++) {
        for(int y = 0; y < sideCountY; y++) {
            Vector2Int nodePos = {x, y};
            remainingNodes.insert(nodePos);
        }
    }

    while(remainingNodes.size() > 0) {
        Vector2Int nextNode = get_next_best_node(
            remainingNodes, foliageDataMap);
        remainingNodes.erase(nextNode);
        bool success = assign_foliage(
            nextNode,
            foliageMap[nextNode.x][nextNode.y],
            _biomeFoliageInfo,
            foliageMap,
            foliageDataMap);
        
        if(!success) {
            return std::nullopt;
        }
    }

    return std::pair(foliageMap, foliageDataMap);
}

Vector2Int FoliageProcessor::get_next_best_node(
        std::unordered_set<Vector2Int> remainingNodes,
        Matrix<FoliageData>& foliageDataGrid) {
    float bestEntropy = std::numeric_limits<float>::max();
    Vector2Int bestNode = (*remainingNodes.begin());
    std::vector<Vector2Int> possibleBestNodes = {};
    for(Vector2Int node : remainingNodes) {

        // Naive entropy.
        float entropy = foliageDataGrid[node.x][node.y].get_remaining_length();
        if(entropy <= bestEntropy) {
            if(entropy < bestEntropy) {
                possibleBestNodes.clear();
            }
            bestEntropy = entropy;
            possibleBestNodes.push_back(node);
        }
    }
    if(possibleBestNodes.size() > 0) {
        bestNode = possibleBestNodes[std::rand() % possibleBestNodes.size()];
    }
    return bestNode;
}

template<size_t size>
int FoliageProcessor::getSum(std::array<int, size> arr) {
    int sum = 0;
    for(int val : arr) {
        sum += val;
    }
    return sum;
}

bool FoliageProcessor::assign_foliage(
        const Vector2Int currentNodePos,
        const FoliageType newType,
        BiomeFoliageInfo& foliageInfo,
        Matrix<FoliageType>& foliageMap,
        Matrix<FoliageData>& foliageDataMap) {

    FoliageData nodeData = foliageDataMap[currentNodePos.x][currentNodePos.y];

    FoliageType currentFoliageType = newType;
    if(currentFoliageType == FoliageType::Foliage_NoSelection) {

        currentFoliageType = make_selection(
            *nodeData.get_remaining_possible_types(),
            foliageInfo.PossibleTypes);
        if(currentFoliageType == FoliageType::Foliage_NoSelection) {
            // Something went wrong selecting the next type.
            // Most likely there are no avilable foliage types left.
            // Generation will have to be re-done.
            // issuesFound = true;
            if(RUN_PRINTS) {
                std::cout << std::format(
                    "Assigning foliage failed. Possible types: {}",
                    (*nodeData.get_remaining_possible_types()).size()) << std::endl;
            }
            return false;
            // throw new AlgorithmDeadlockedException();
            // return FoliageType.NoSelection;
        }
    }

    // Update neighbour possible types with neighbour bonus values.
    auto bonusTypes = _biomeFoliageInfo.NeighbourBonus[static_cast<int>(currentFoliageType)];
    if(bonusTypes.size() != 0) {
        auto neighbours = get_neighbours(
            currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
        for(auto neighbourPos : neighbours) {
            auto selectedFoliageType = foliageMap[neighbourPos.x][neighbourPos.y];
            if(selectedFoliageType != FoliageType::Foliage_NoSelection) {
                continue;
            }
            auto neighbourData = foliageDataMap[neighbourPos.x][neighbourPos.y];
            for(int i = 0; i < bonusTypes.size(); i++) {

                auto bonus = bonusTypes[i];
                if(bonus.second <= 0) {
                    continue;
                }

                auto remaining = *neighbourData.get_remaining_possible_types();

                if(remaining[static_cast<int>(bonus.first)] <= 0) {
                    continue;
                }
                remaining[static_cast<int>(bonus.first)] += bonus.second;
            }
        }
    }

    foliageMap[currentNodePos.x][currentNodePos.y] = currentFoliageType;

    std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> newRemainingPossibleTypes = {};
    newRemainingPossibleTypes[static_cast<int>(currentFoliageType)] = 1;

    bool success = update_possible_types(
        currentNodePos,
        newRemainingPossibleTypes,
        foliageInfo, foliageMap, foliageDataMap);

    return success;
}

template<size_t size>
bool FoliageProcessor::update_possible_types(
        const Vector2Int initialCurrentNodePos,
        const std::array<int, size>& remainingPossibleTypes,
        BiomeFoliageInfo& foliageInfo,
        Matrix<FoliageType>& foliageMap,
        Matrix<FoliageData>& foliageDataMap){

    Matrix<int> propagatedNodes (
        MapDefinitions::SUBSECTION_FULL_SIZE,
        MapDefinitions::SUBSECTION_FULL_SIZE);
    auto _actionQueue = std::queue<std::pair<Vector2Int,Vector2Int>>();

    {
        for(int x = 0; x < propagatedNodes.dim_a(); x++) {
            for(int y = 0; y < propagatedNodes.dim_b(); y++) {
                propagatedNodes[x][y] = false;
            }
        }

        std::vector<Vector2Int> unfilteredNeighbours;
        unfilteredNeighbours = get_neighbours(
            initialCurrentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);

        for(Vector2Int nnp : unfilteredNeighbours) {

            if(foliageMap[nnp.x][nnp.y] != FoliageType::Foliage_NoSelection) {
                continue;
            }

            Vector2Int neighbourNodePos = nnp;

            propagatedNodes[nnp.x][nnp.y] = true;
            _actionQueue.push({initialCurrentNodePos, neighbourNodePos});
        }
    }

    while(_actionQueue.size() > 0) {
        auto pair = _actionQueue.front();
        _actionQueue.pop();
        Vector2Int currentNodePos = pair.first;
        Vector2Int neighbourNodePos = pair.second;

        auto remaining = remainingPossibleTypes;

        if(currentNodePos != initialCurrentNodePos) {
            FoliageData nodeData = foliageDataMap[currentNodePos.x][currentNodePos.y];
            remaining = *nodeData.get_remaining_possible_types();
        }

        bool success = update_possible_types_recursively(
            currentNodePos,
            neighbourNodePos,
            foliageInfo,
            remaining,
            foliageMap,
            foliageDataMap,
            propagatedNodes,
            _actionQueue);
        if(!success) {
            return false;
        }
    }
    return true;
    // if(overallFailed) {
    // 	Logger.LogWarning("Could not propagate new node. Started from: "+currentNodePos);
    // }
}

template<size_t size>
bool FoliageProcessor::update_possible_types_recursively(
        const Vector2Int lastNodePos,
        const Vector2Int currentNodePos,
        BiomeFoliageInfo& foliageInfo,
        const std::array<int, size>& lastNodePossibleTypes,
        Matrix<FoliageType>& foliageMap,
        Matrix<FoliageData>& foliageDataMap,
        Matrix<int>& propagatedNodes,
        std::queue<std::pair<Vector2Int,Vector2Int>>& actionQueue) {

    m_recursionCount++;

    std::array<std::vector<FoliageType>, size>* lastNodeDirectionalRelations;
    Direction direction = Direction::DirectionNone;
    // Check for up relations.
    if(currentNodePos.x == lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.UpRelations;
        direction = Direction::DirectionUp;
    }
    // Check for down relations.
    else if(currentNodePos.x == lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.DownRelations;
        direction = Direction::DirectionDown;
    }
    // We have moved right-wise.
    // Check for left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y == lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.LeftRelations;
        direction = Direction::DirectionLeft;
    }
    // Check for right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y == lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.RightRelations;
        direction = Direction::DirectionRight;
    }

    // Check for down left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.DownLeftRelations;
        direction = Direction::DirectionDownLeft;
    }
    // Check for down right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y < lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.DownRightRelations;
        direction = Direction::DirectionDownRight;
    }
    // Check for up left relations.
    else if(currentNodePos.x < lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.UpLeftRelations;
        direction = Direction::DirectionUpLeft;
    }
    // Check for up right relations.
    else if(currentNodePos.x > lastNodePos.x && currentNodePos.y > lastNodePos.y) {
        lastNodeDirectionalRelations = &foliageInfo.UpRightRelations;
        direction = Direction::DirectionUpRight;
    }

    // We find the types in the remove dict that any
    // remaining possible types all have in common.
    // These are no longer viable as possible neighbours.
    if(!lastNodeDirectionalRelations) {
        std::cout << "Directional WFC array does not exist, or could not be found." << std::endl;
    }

    std::array<int, size> stillPossibleItems = {};

    for(int foliageIndex = 0; foliageIndex < lastNodePossibleTypes.size(); foliageIndex++) {
        // This foliage was not possible from the last node.
        if(lastNodePossibleTypes[foliageIndex] <= 0) {
            continue;
        }
        auto relationFoliageTypes = (*lastNodeDirectionalRelations)[foliageIndex];
        if(relationFoliageTypes.size() == 0) {
            continue;
        }
        for(auto foliageType : relationFoliageTypes) {

            stillPossibleItems[static_cast<int>(foliageType)] = 1;
        }
    }

    FoliageData nodeData = foliageDataMap[currentNodePos.x][currentNodePos.y];
    auto remainingPossibleTypes = *nodeData.get_remaining_possible_types();

    int remainingCount = 0;
    int newlyRemovedCount = 0;

    for(int foliageIndex = 0; foliageIndex < remainingPossibleTypes.size(); foliageIndex++) {

        if(remainingPossibleTypes[foliageIndex] <= 0) {
            continue;
        }

        if(stillPossibleItems[foliageIndex] <= 0) {
            remainingPossibleTypes[foliageIndex] = 0;
            newlyRemovedCount++;
            continue;
        }

        remainingCount++;
    }

    if(newlyRemovedCount == 0) {
        m_emptyRecursionCount++;
        return true;
    }

    nodeData.set_remaining_possible_types(remainingPossibleTypes);
    foliageDataMap[currentNodePos.x][currentNodePos.y] = nodeData;

    if(remainingCount == 0) {
        if(RUN_PRINTS && m_verboseLogging) {
            std::vector<std::string> lastNodePossibleTypesStrings = {};
            for(int value : lastNodePossibleTypes) {
                lastNodePossibleTypesStrings.push_back(
                    std::to_string(value));}
            std::string lastNodePossibleTypesString = DiskManager::join(
                lastNodePossibleTypesStrings, ", ");
            std::cout << std::format(
                "Node no longer has any remaining possible types. {0}\n"
                "Direction: {1}\n"
                "Removed types: {2}\n"
                "Last node possible types {3}: {4}",
                    currentNodePos.to_string(),
                    static_cast<int>(direction),
                    newlyRemovedCount,
                    lastNodePos.to_string(),
                    lastNodePossibleTypesString) << std::endl;

            logger::log_grid(foliageDataMap, _biomeFoliageInfo.WalkablePossibleTypes);
            logger::log_grid(foliageMap);

            auto neighbours = get_neighbours(
                currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
            std::string neighboursString = "";

            std::vector<std::string> neighbourFoliageStrings = {};
            for(auto v2i : neighbours) {
                neighbourFoliageStrings.push_back(
                    std::to_string(static_cast<int>(foliageMap[v2i.x][v2i.y])));}
            if(neighbourFoliageStrings.size() == 8) {
                auto nf = neighbourFoliageStrings;
                neighboursString = std::format(
                "{0}, {1}, {2}"
                "{3}, SELECTING, {4}"
                "{5}, {6}, {7}",
                    nf[0],nf[1],nf[2],nf[3],nf[4],nf[5],nf[6],nf[7]);
            }
            else {
                std::string neighboursString = DiskManager::join(neighbourFoliageStrings, ", ");
            }
            std::cout << "No more possible types available. Last node: "+neighboursString << std::endl;
        }
        return false;
    }

    bool use4WayNeighbours = false;

    // Find out what types are not impossible for neighbours.
    std::vector<Vector2Int> unfilteredNeighbours;
    if(use4WayNeighbours) {
        unfilteredNeighbours = get_4_way_neighbours(
            currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
    }
    else {
        unfilteredNeighbours = get_neighbours(
            currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
    }


    for(Vector2Int nnp : unfilteredNeighbours) {
        if(nnp == lastNodePos) {
            continue;
        }
        if(propagatedNodes[nnp.x][nnp.y]) {
            continue;
        }
        if(foliageMap[nnp.x][nnp.y] != FoliageType::Foliage_NoSelection) {
            continue;
        }

        propagatedNodes[nnp.x][nnp.y] = true;

        Vector2Int neighbourNodePos = nnp;
        actionQueue.push({currentNodePos,neighbourNodePos});
    }

    return true;
}

std::vector<Vector2Int> FoliageProcessor::get_neighbours(Vector2Int gridPos, int mapSize) {
    int lengthX = mapSize;
    int lengthY = mapSize;
    std::vector<Vector2Int> vecList = {};
    for(int y = 1; y >= -1; y--) {
        for(int x = -1; x <= 1; x++) {

            if(x == 0 && y == 0) {
                continue;
            }

            int gridX = gridPos.x + x;
            int gridY = gridPos.y + y;

            if(gridX >= lengthX || gridX < 0 || gridY >= lengthY || gridY < 0) {
                continue;
            }

            vecList.push_back({gridX, gridY});
        }
    }
    return vecList;
}

std::vector<Vector2Int> FoliageProcessor::get_4_way_neighbours(Vector2Int gridPos, int mapSize) {
    int lengthX = mapSize;
    int lengthY = mapSize;
    std::vector<Vector2Int> vecList = {};
    if(gridPos.x-1 >= 0) {
        vecList.push_back({gridPos.x-1, gridPos.y});
    }
    if(gridPos.x+1 < lengthX) {
        vecList.push_back({gridPos.x+1, gridPos.y});
    }
    if(gridPos.y-1 >= 0) {
        vecList.push_back({gridPos.x, gridPos.y-1});
    }
    if(gridPos.y+1 < lengthY) {
        vecList.push_back({gridPos.x, gridPos.y+1});
    }
    return vecList;
}

template<size_t size>
FoliageType FoliageProcessor::make_selection(
        const std::array<int, size>& possibleTypes,
        const std::array<int, size>& mainWeights) {

    // Tally total score.
    int total = 0;
    for(int foliageIndex = 0; foliageIndex < possibleTypes.size(); foliageIndex++) {
        int typeWeight = possibleTypes[foliageIndex];
        // We ignore types that have been fully removed.
        if(typeWeight <= 0) {
            continue;
        }
        total += mainWeights[foliageIndex] + typeWeight;
    }

    if(total == 0) {
        return FoliageType::Foliage_NoSelection;
    }

    // We do this to have the value scale together with the node size.
    int randomVal = std::rand() % total;
    int currentVal = 0;
    for(int foliageIndex = 0; foliageIndex < possibleTypes.size(); foliageIndex++) {
        int typeWeight = possibleTypes[foliageIndex];
        if(typeWeight <= 0) {
            continue;
        }
        currentVal += mainWeights[foliageIndex] + typeWeight;
        if(randomVal < currentVal) {
            return static_cast<FoliageType>(foliageIndex);
        }
    }
    return FoliageType::Foliage_NoSelection;
}
