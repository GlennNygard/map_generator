
#include <iostream>
#include <random>
#include <stdlib.h>
#include <stack>
#include <chrono>
#include <limits>

#include "foliage_processor.h"
#include "logger.h"
#include "relations.h"


FoliageProcessor::FoliageProcessor(BiomeFoliageInfo biomeFoliageInfo, bool verboseLogging) {
    m_issuesFound = false;
    m_biomeFoliageInfo = biomeFoliageInfo;
    m_verboseLogging = verboseLogging;
}

std::chrono::milliseconds FoliageProcessor::get_time() {
    std::chrono::milliseconds startTime = duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return startTime;
}

float FoliageProcessor::format_time(long count) {
    return static_cast<float>(count) / 1000;
}

std::pair<Matrix<FoliageType>, bool> FoliageProcessor::mark_foliage_nodes(
        const LevelValues levelValues) {

    const int fullMapSideCountX = levelValues.gridCountX;
    const int fullMapSideCountY = levelValues.gridCountY;

    const int subsectionCountX = levelValues.subsectionCountX;
    const int subsectionCountY = levelValues.subsectionCountY;

    const int subsectionBorderedSideCountX = levelValues.subsectionBorderedSideCountX;
    const int subsectionBorderedSideCountY = levelValues.subsectionBorderedSideCountY;

    const int maxAttempts = subsectionCountX*subsectionCountY * 3;

    Matrix<FoliageType> fullFoliageMap(
        fullMapSideCountX, fullMapSideCountY);
    Matrix<FoliageData> fullFoliageDataMap(
        fullMapSideCountX, fullMapSideCountY);

    int overallAttempts = 0;
    int currentSubsectionAttempts = 0;
    int currentRetryAttempts = 0;

    int furthestIndex = 0;
    Vector2Int furthestSectionPos = {};

    // Local foliage maps.
    Matrix<FoliageType> foliageMap (
        subsectionBorderedSideCountX,
        subsectionBorderedSideCountY);
    Matrix<FoliageData> foliageDataMap (
        subsectionBorderedSideCountX,
        subsectionBorderedSideCountY);

    Matrix<int> requiresPropagationMap (
        subsectionBorderedSideCountX,
        subsectionBorderedSideCountY);

    prepare_full_data(fullFoliageDataMap, levelValues);

    std::stack<Vector2Int> sectionStack = {};
    for(int secX = subsectionCountX-1; secX >= 0; secX--) {
        for(int secY = subsectionCountY-1; secY >= 0; secY--) {
            sectionStack.push(Vector2Int{secX, secY});
        }
    }

    std::vector<Vector2Int> retryList = {};
    int retryIndex = 0;
    int currentIndex = 0;

    long requiredTime = 0;
    long retryTime = 0;
    std::chrono::milliseconds startTime = get_time();

    bool failed = false;
    while(sectionStack.size() > 0) {
        std::chrono::milliseconds time = get_time();

        Vector2Int currentSectionPos;
        if(retryList.size() > 0) {
            if(retryIndex >= retryList.size()) {
                if(m_verboseLogging) {
                    logger::log(
                        std::format(
                            "Current: {0}."
                            "Overall {1}."
                            "Retries {2}."
                            "Count {3}."
                            "Furthest {4}."
                            "Last current ind: {5}",
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
        currentIndex = subsectionCountX * currentSectionPos.x + currentSectionPos.y;

        if(m_verboseLogging) {
            logger::log(std::format(
                "STARTING ATTEMPT \n"
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

        prepare_section_data(
            foliageMap, foliageDataMap,
            requiresPropagationMap, fullFoliageDataMap,
            currentSectionPos);

        logger::log_grid(
            foliageDataMap, m_biomeFoliageInfo.get_walkable_possible_types());

        bool overallFailed = false;
        for(int i = 0; i < subsectionBorderedSideCountX * subsectionBorderedSideCountY; i++) {
            int x = i % subsectionBorderedSideCountX;
            int y = static_cast<int>(
                std::floor(static_cast<float>(i) / subsectionBorderedSideCountX));

            if(!requiresPropagationMap[x][y]) {
                continue;
            }
            Vector2Int node (x,y);
            FoliageData nodeData = foliageDataMap[node.x][node.y];
            bool success = update_possible_types(
                node,
                nodeData.get_remaining_possible_types(),
                m_biomeFoliageInfo,
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

        logger::log_grid(
            foliageDataMap, m_biomeFoliageInfo.get_walkable_possible_types());

        // Process subsection!
        if(!overallFailed) {

            auto processResult = process_subsection(
                foliageMap, foliageDataMap);
            if(!processResult) {
                overallFailed = true;
            }
        }

        // Generation failed.
        if(overallFailed) {
            int secX = currentSectionPos.x;
            int secY = currentSectionPos.y;
            overallAttempts++;
            currentSubsectionAttempts++;
            currentRetryAttempts++;
            if(m_verboseLogging) {
                logger::log(
                    std::format(
                    "Processing subsection ({},{}) failed. "
                    "Attempts: {}. Overall attempts: {}.",
                    secX, secY, currentSubsectionAttempts, overallAttempts));
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
                // otherwise the section's seams won't be treated correctly.
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
                    clear_subsection_from_full_grid(
                        fullFoliageMap, fullFoliageDataMap,
                        retryPos);
                }
            }
            else if(currentSubsectionAttempts >= 2) {
                currentSubsectionAttempts = 0;
                retryIndex = 0;

                // Clear retried data.
                for(Vector2Int retryPos : retryList) {
                    clear_subsection_from_full_grid(
                        fullFoliageMap, fullFoliageDataMap,
                        retryPos);
                }
            }
            else {
                retryIndex = std::max(retryIndex-2, 0);
                clear_subsection_from_full_grid(
                    fullFoliageMap, fullFoliageDataMap,
                    currentSectionPos);
                clear_subsection_from_full_grid(
                    fullFoliageMap, fullFoliageDataMap,
                    retryList[retryIndex]);
            }
            retryTime += (get_time() - time).count();
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

        if(m_verboseLogging) {
            std::cout << "Writing results: " << std::endl;
            logger::log_grid(foliageDataMap,
                m_biomeFoliageInfo.get_walkable_possible_types());
            logger::log_grid(foliageMap);

            logger::log(std::format(
                "Recursion Count: {} / {} / {}",
                m_recursionCount,
                m_emptyRecursionCount,
                m_emptyRecursionEarlyCount));
        }

        write_to_full_map(
            foliageMap, foliageDataMap,
            fullFoliageMap, fullFoliageDataMap,
            currentSectionPos);

        if(isFurthestIndex) {
            requiredTime += (get_time() - time).count();
        }
        else {
            retryTime += (get_time() - time).count();
        }
    }

    // If generation failed, we mark the remaining nodes as NoFoliage.
    // This is to help with debugging.
    if(failed) {
        while(sectionStack.size() > 0) {

            Vector2Int currentSectionPos = sectionStack.top();
            sectionStack.pop();

            clear_subsection_from_full_grid(
                fullFoliageMap, currentSectionPos, levelValues);
        }
        logger::log("GENERATION FAILED.");
    }

    double executionTime = format_time(requiredTime + retryTime);
    double timePerSubsection = (executionTime/(subsectionCountX*subsectionCountY));
    double effectiveExecutionTime = format_time(requiredTime);
    double effectiveTimePerSubsection = (effectiveExecutionTime/((subsectionCountX*subsectionCountY)-overallAttempts));

    logger::log(
        std::format("Total sections: {}. ", subsectionCountX*subsectionCountY) +
        std::format("Failed attempts: {}. ", overallAttempts)  +
        std::format("Time: {}. ", format_time((get_time() - startTime).count())) +
        std::format("Time to execute. Effective: {}. All: {}. ",
            effectiveExecutionTime, executionTime) +
        std::format("Avg time per subsection. Effective: {}. ",
            effectiveTimePerSubsection) +
        std::format(" (All: {})",timePerSubsection));

    return std::pair(fullFoliageMap, !failed);
};

void FoliageProcessor::prepare_full_data(
        Matrix<FoliageData> &fullFoliageDataMap,
        const LevelValues levelValues) {
    std::vector<Vector2Int> forcedWalkableSubsections = {};

    int halfX = (levelValues.subsectionCountX/2);
    int halfY = (levelValues.subsectionCountY/2);

    Vector2Int rangeX = {1,1};
    Vector2Int rangeY = {1,1};
    if(levelValues.subsectionCountX % 2 == 0) {
        rangeX.x = halfX-1;
        rangeX.y = halfX+1;
    }
    else {
        rangeX.x = halfX-1;
        rangeX.y = halfX+1;
    }

    if(levelValues.subsectionCountY % 2 == 0) {
        rangeY.x = halfY-1;
        rangeY.y = halfY+1;
    }
    else {
        rangeY.x = halfY-1;
        rangeY.y = halfY+1;
    }

    for(int secX = rangeX.x; secX <= rangeX.y; secX++) {
        for(int secY = rangeY.x; secY <= rangeY.y; secY++) {
            forcedWalkableSubsections.push_back(Vector2Int(secX, secY));
        }
    }

    logger::log(std::format(
        "Subsections: {},{}",
        levelValues.subsectionCountX,
        levelValues.subsectionCountY));

    int mapForcedWalkableSectionBorder = 2;
    for(int secX = 0; secX < levelValues.subsectionCountX; secX++) {
        for(int secY = 0; secY < levelValues.subsectionCountY; secY++) {
            if(secX > mapForcedWalkableSectionBorder &&
                    secX <levelValues.subsectionCountX-mapForcedWalkableSectionBorder &&
                    secY > mapForcedWalkableSectionBorder &&
                    secY < levelValues.subsectionCountY-mapForcedWalkableSectionBorder) {
                continue;
            }
            forcedWalkableSubsections.push_back(Vector2Int(secX, secY));
        }
    }

    for(auto secPos : forcedWalkableSubsections) {
        for(int x = 0; x < levelValues.subsectionBorderedSideCountX; x++) {
            for(int y = 0; y < levelValues.subsectionBorderedSideCountY; y++) {
                Vector2Int fullCoord = relations::convert_to_global(
                    secPos.x, secPos.y, x, y);
                int fullX = fullCoord.x;
                int fullY = fullCoord.y;
                if(fullX < 0 || fullY < 0 ||
                        fullX >= fullFoliageDataMap.dim_a() ||
                        fullY >= fullFoliageDataMap.dim_b()) {
                    continue;
                }
                fullFoliageDataMap[fullX][fullY].isWalkableOnly = true;
            }
        }
    }
}

void FoliageProcessor::prepare_section_data(
        Matrix<FoliageType> &foliageMap,
        Matrix<FoliageData> &foliageDataMap,
        Matrix<int> &requiresPropagationMap,
        const Matrix<FoliageData> &fullFoliageDataMap,
        const Vector2Int currentSectionPos) {

    for(int x = 0; x < foliageMap.dim_a(); x++) {
        for(int y = 0; y < foliageMap.dim_b(); y++) {
            Vector2Int fullCoord = relations::convert_to_global(
                currentSectionPos.x, currentSectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;

            // Reset main array.
            foliageMap[x][y] = FoliageType::Foliage_NoSelection;

            FoliageData foliageData = foliageDataMap[x][y];

            // Outside main map.
            if(fullX < 0 || fullY < 0 ||
                    fullX >= fullFoliageDataMap.dim_a() ||
                    fullY >= fullFoliageDataMap.dim_b()) {
                foliageData.set_remaining_from_array(
                    m_biomeFoliageInfo.get_walkable_possible_types());
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
            if(fullNodeFoliageData.hasData && fullNodeFoliageData.hasData) {
                if(x < 1 || y < 1 || x >= foliageMap.dim_a()-1 || y >= foliageMap.dim_b()-1) {
                    foliageData.set_remaining_from_array(
                        fullNodeFoliageData.get_remaining_possible_types());
                    foliageDataMap[x][y] = foliageData;
                    requiresPropagationMap[x][y] = true;
                    continue;
                }
            }

            // Node does not have previous data.
            else {
                // Constrain border edges.
                if(x < MapDefinitions::SUBSECTION_BORDER ||
                        y < MapDefinitions::SUBSECTION_BORDER ||
                        x >= foliageMap.dim_a()-MapDefinitions::SUBSECTION_BORDER ||
                        y >= foliageMap.dim_b()-MapDefinitions::SUBSECTION_BORDER) {
                    foliageData.set_remaining_from_array(
                        m_biomeFoliageInfo.get_walkable_possible_types());
                    requiresPropagationMap[x][y] = true;

                    foliageDataMap[x][y] = foliageData;
                    continue;
                }
            }

            requiresPropagationMap[x][y] = forcedWalkable;

            if(forcedWalkable) {
                foliageData.set_remaining_from_array(
                    m_biomeFoliageInfo.get_walkable_possible_types());
            }
            else {
                foliageData.set_remaining_from_array(
                    m_biomeFoliageInfo.get_possible_types());
            }
            foliageDataMap[x][y] = foliageData;
        }
    }
}

void FoliageProcessor::write_to_full_map(
        const Matrix<FoliageType> &resultsFoliageMap,
        Matrix<FoliageData> &resultsFoliageDataMap,
        Matrix<FoliageType> &fullFoliageMap,
        Matrix<FoliageData> &fullFoliageDataMap,
        Vector2Int sectionPos) {
    // Write back to full map without borders.
    // We only write back data to the top left border nodes.
    for(int x = 0; x < resultsFoliageMap.dim_a(); x++) {
        for(int y = 0; y < resultsFoliageMap.dim_b(); y++) {
            Vector2Int fullCoord = relations::convert_to_global(
                sectionPos.x, sectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;
            if(fullX < 0 || fullY < 0 ||
                    fullX >= fullFoliageMap.dim_a() ||
                    fullY >= fullFoliageMap.dim_b()) {
                continue;
            }
            fullFoliageMap[fullX][fullY] = resultsFoliageMap[x][y];
            if(x >= 1 && y >= 1 &&
                    x < resultsFoliageMap.dim_a()-1 &&
                    y < resultsFoliageMap.dim_b()-1) {
                fullFoliageDataMap[fullX][fullY].set_remaining_from_array(
                    resultsFoliageDataMap[x][y].get_remaining_possible_types());
            }
        }
    }
}

void FoliageProcessor::clear_subsection_from_full_grid(
        Matrix<FoliageType>& fullFoliageMap,
        Vector2Int sectionPos,
        LevelValues levelValues) {

    int secX = sectionPos.x;
    int secY = sectionPos.y;

    for(int x = 0; x < levelValues.subsectionBorderedSideCountX; x++) {
        for(int y = 0; y < levelValues.subsectionBorderedSideCountY; y++) {
            Vector2Int fullCoord = relations::convert_to_global(secX, secY, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;

            if(fullX < 0 || fullY < 0 ||
                    fullX >= fullFoliageMap.dim_a() ||
                    fullY >= fullFoliageMap.dim_b()) {
                continue;
            }

            if(fullFoliageMap[fullX][fullY] != FoliageType::Foliage_NoSelection) {
                continue;
            }

            fullFoliageMap[fullX][fullY] = FoliageType::Foliage_CompletelyThrottenGround;
        }
    }
}

void FoliageProcessor::clear_subsection_from_full_grid(
        Matrix<FoliageType>& fullFoliageMap,
        Matrix<FoliageData>& fullFoliageDataMap,
        const Vector2Int sectionPos) {

    for(int x = 0; x < MapDefinitions::SUBSECTION_FULL_SIZE; x++) {
        for(int y = 0; y < MapDefinitions::SUBSECTION_FULL_SIZE; y++) {
            Vector2Int fullCoord = relations::convert_to_global(sectionPos.x, sectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;

            if(fullX < 0 || fullY < 0 || fullX >= fullFoliageMap.dim_a() || fullY >= fullFoliageMap.dim_b()) {
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
                    // (right side border excluded).
                    if(fullFoliageDataMap[fullX][fullY].isWalkableOnly) {
                        fullFoliageDataMap[fullX][fullY].set_remaining_from_array(
                            m_biomeFoliageInfo.get_walkable_possible_types());
                    }
                    else {
                        fullFoliageDataMap[fullX][fullY].set_remaining_from_array(
                            m_biomeFoliageInfo.get_possible_types());
                    }
                }
                else {
                    fullFoliageDataMap[fullX][fullY].hasData = false;
                }
            }
            else if(x >= MapDefinitions::SUBSECTION_FULL_SIZE-1 &&
                    y >= 1 && y < MapDefinitions::SUBSECTION_FULL_SIZE-1) {
                fullFoliageDataMap[fullX][fullY].hasData = false;
            }
        }
    }
}

std::optional<std::pair<Matrix<FoliageType>, Matrix<FoliageData>>> FoliageProcessor::process_subsection(
        Matrix<FoliageType>& foliageMap,
        Matrix<FoliageData>& foliageDataMap) {

    m_issuesFound = false;

    int sideCountX = foliageMap.dim_a();
    int sideCountY = foliageMap.dim_b();

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
            m_biomeFoliageInfo,
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
            nodeData.get_remaining_possible_types(),
            foliageInfo.get_possible_types());
        if(currentFoliageType == FoliageType::Foliage_NoSelection) {
            // Something went wrong selecting the next type.
            // Most likely there are no avilable foliage types left.
            // Generation will have to be re-done.
            std::cout << std::format(
                "Assigning foliage failed. Possible types: {}",
                nodeData.get_remaining_possible_types().size()) << std::endl;
            return false;
        }
    }

    // Update neighbour possible types with neighbour bonus values.
    auto bonusTypes = m_biomeFoliageInfo.neighbourBonus[static_cast<int>(currentFoliageType)];
    if(bonusTypes.size() != 0) {
        auto neighbours = relations::get_neighbours(
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

                auto remaining = neighbourData.get_remaining_possible_types();

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
        unfilteredNeighbours = relations::get_neighbours(
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
            remaining = nodeData.get_remaining_possible_types();
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

    auto res = m_biomeFoliageInfo.get_relations_from_nodes(lastNodePos, currentNodePos);
    auto lastNodeDirectionalRelations = res.first;
    Direction direction = res.second;

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
    auto remainingPossibleTypes = nodeData.get_remaining_possible_types();

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
        if(m_verboseLogging) {
            std::vector<std::string> lastNodePossibleTypesStrings = {};
            for(int value : lastNodePossibleTypes) {
                lastNodePossibleTypesStrings.push_back(
                    std::to_string(value));}
            std::string lastNodePossibleTypesString = DiskManager::join(
                lastNodePossibleTypesStrings, ", ");
            logger::log(std::format(
                "Node no longer has any remaining possible types. {0}\n"
                "Direction: {1}\n"
                "Removed types: {2}\n"
                "Last node possible types {3}: {4}",
                    currentNodePos.to_string(),
                    static_cast<int>(direction),
                    newlyRemovedCount,
                    lastNodePos.to_string(),
                    lastNodePossibleTypesString));

            logger::log_grid(
                foliageDataMap,
                m_biomeFoliageInfo.get_walkable_possible_types());
            logger::log_grid(foliageMap);

            auto neighbours = relations::get_neighbours(
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
            logger::log("No more possible types available. Last node: "+neighboursString);
        }
        return false;
    }

    bool use4WayNeighbours = false;

    // Find out what types are not impossible for neighbours.
    std::vector<Vector2Int> unfilteredNeighbours;
    if(use4WayNeighbours) {
        unfilteredNeighbours = relations::get_4_way_neighbours(
            currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
    }
    else {
        unfilteredNeighbours = relations::get_neighbours(
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
