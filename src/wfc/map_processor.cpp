

#include "map_processor.h"
#include "logger.h"
#include "relations.h"


MapProcessor::MapProcessor(
        BiomeFoliageInfo& biomeFoliageInfo,
        const LevelValues& levelValues,
        const bool verboseLogging) :
            m_biomeFoliageInfo(biomeFoliageInfo),
            m_levelValues(levelValues),
            m_verboseLogging(verboseLogging) {

    m_issuesFound = false;

    m_recursionCount = 0;
	m_emptyRecursionCount = 0;
	m_emptyRecursionEarlyCount = 0;

	m_stillPossibleBuffer = {};
}

std::pair<Matrix<FoliageType>, bool> MapProcessor::RunProcessing() {

    m_fullFoliageMap = Matrix<FoliageType>(
         m_levelValues.gridCountX,
         m_levelValues.gridCountY);
    m_fullFoliageDataMap = Matrix<FoliageData>(
         m_levelValues.gridCountX,
         m_levelValues.gridCountY);

    m_sectionStack = {};

    m_overallAttempts = 0;
    m_currentSubsectionAttempts = 0;
    m_currentRetryAttempts = 0;

    m_furthestIndex = 0;
    m_retryIndex = 0;
    m_currentIndex = 0;

    m_furthestSectionPos = {};
    m_retryList = {};

    SectionState sectionState = InitialiseSectionState();

    PrepareFullData(m_fullFoliageDataMap);

    for(int secX = m_levelValues.subsectionCountX-1; secX >= 0; secX--) {
        for(int secY = m_levelValues.subsectionCountY-1; secY >= 0; secY--) {
            m_sectionStack.push(Vector2Int{secX, secY});
        }
    }

    auto timeTracker = TimeTracker();

    bool failed = false;
    while(m_sectionStack.size() > 0) {
        std::chrono::milliseconds time = TimeTracker::GetTime();

        const Vector2Int currentSectionPos = GetNextSectionPos();
        m_currentIndex = m_levelValues.subsectionCountX * currentSectionPos.x + currentSectionPos.y;

        m_recursionCount = 0;
        m_emptyRecursionCount = 0;
        m_emptyRecursionEarlyCount = 0;

        PrepareSectionData(sectionState, currentSectionPos);

        logger::LogGrid(
            sectionState.foliageDataMap,
            m_biomeFoliageInfo.GetWalkableFoliagePriority());

        bool overallFailed = RunInitialPropagation(sectionState);

        if(overallFailed && m_verboseLogging) {
            logger::log_error("Initial propagation failed.");
        }

        logger::LogGrid(
            sectionState.foliageDataMap, m_biomeFoliageInfo.GetWalkableFoliagePriority());

        // Process subsection!
        if(!overallFailed) {

            auto processResult = ProcessSubsection(sectionState);
            if(!processResult) {
                overallFailed = true;
            }
        }

        // Generation failed.
        if(overallFailed) {
            auto results = HandleFailure(currentSectionPos);

            timeTracker.AddRetryTime(TimeTracker::GetTime() - time);

            if(results == FailureResults::Abort) {
                failed = true;
                break;
            }
            if(results == FailureResults::Retry) {
                continue;
            }
        }

        // Generation succeeded.
        bool isFurthestIndex = m_currentIndex > m_furthestIndex || m_furthestIndex == 0;

        HandleSuccess(isFurthestIndex, currentSectionPos, sectionState);

        WriteToFullMap(sectionState, currentSectionPos);

        if(isFurthestIndex) {
            timeTracker.AddRequiredTime(TimeTracker::GetTime() - time);
        }
        else {
            timeTracker.AddRetryTime(TimeTracker::GetTime() - time);
        }
    }

    // If generation failed, we mark the remaining nodes as NoFoliage.
    // This is to help with debugging.
    if(failed) {
        logger::Log("GENERATION FAILED.");
    }

    logger::Log(
        std::format(
            "Total sections: {}. ",
            m_levelValues.subsectionCountX * m_levelValues.subsectionCountY) +
        std::format("Failed attempts: {}. ", m_overallAttempts));

    timeTracker.LogTime(m_overallAttempts, m_levelValues);

    return std::pair(m_fullFoliageMap, !failed);
};


SectionState MapProcessor::InitialiseSectionState() const {
    
    SectionState state;

    // Local foliage maps.
    state.foliageMap = Matrix<FoliageType>(
        m_levelValues.subsectionBorderedSideCountX,
        m_levelValues.subsectionBorderedSideCountY);
    state.foliageDataMap = Matrix<FoliageData>(
        m_levelValues.subsectionBorderedSideCountX,
        m_levelValues.subsectionBorderedSideCountY);

    state.requiresPropagationMap = Matrix<char>(
        m_levelValues.subsectionBorderedSideCountX,
        m_levelValues.subsectionBorderedSideCountY);

    return std::move(state);
}

const Vector2Int MapProcessor::GetNextSectionPos() {
    Vector2Int currentSectionPos;
    if(m_retryList.size() > 0) {
        if(m_retryIndex >= m_retryList.size()) {
            if(m_verboseLogging) {
                logger::Log(
                    std::format(
                        "Current: {0}."
                        "Overall {1}."
                        "Retries {2}."
                        "Count {3}."
                        "Furthest {4}."
                        "Last current ind: {5}",
                        m_currentSubsectionAttempts, m_overallAttempts,
                        m_retryIndex, m_retryList.size(),
                        m_furthestIndex, m_currentIndex));
            }
        }
        currentSectionPos = m_retryList[m_retryIndex];
        m_retryIndex++;
    }
    else {
        currentSectionPos = m_sectionStack.top();
        m_sectionStack.pop();
    }

    if(m_verboseLogging) {
        logger::Log(std::format(
            "STARTING SECTION ATTEMPT\n"
            "Section: {}"
            "Furthest: {}\n"
            "Section index: {}."
            "Furthest index: {}."
            "Remaining in stack: {}.",
            currentSectionPos.ToString(),
            m_furthestSectionPos.ToString(),
            (currentSectionPos.x+currentSectionPos.y),
            (m_furthestSectionPos.x+m_furthestSectionPos.y),
            m_sectionStack.size()));
    }

    return currentSectionPos;
}


void MapProcessor::PrepareFullData(
        Matrix<FoliageData>& fullFoliageDataMap) const {
    std::vector<Vector2Int> forcedWalkableSubsections = {};

    int halfX = (m_levelValues.subsectionCountX/2);
    int halfY = (m_levelValues.subsectionCountY/2);

    Vector2Int rangeX = {1,1};
    Vector2Int rangeY = {1,1};
    if(m_levelValues.subsectionCountX % 2 == 0) {
        rangeX.x = halfX-1;
        rangeX.y = halfX+1;
    }
    else {
        rangeX.x = halfX-1;
        rangeX.y = halfX+1;
    }

    if(m_levelValues.subsectionCountY % 2 == 0) {
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

    logger::Log(std::format(
        "Subsections: {},{}",
        m_levelValues.subsectionCountX,
        m_levelValues.subsectionCountY));

    const int mapForcedWalkableSectionBorder = 2;
    for(int secX = 0; secX < m_levelValues.subsectionCountX; secX++) {
        for(int secY = 0; secY < m_levelValues.subsectionCountY; secY++) {
            if(secX > mapForcedWalkableSectionBorder &&
                    secX <m_levelValues.subsectionCountX-mapForcedWalkableSectionBorder &&
                    secY > mapForcedWalkableSectionBorder &&
                    secY < m_levelValues.subsectionCountY-mapForcedWalkableSectionBorder) {
                continue;
            }
            forcedWalkableSubsections.push_back(Vector2Int(secX, secY));
        }
    }

    for(auto secPos : forcedWalkableSubsections) {
        for(int x = 0; x < m_levelValues.subsectionBorderedSideCountX; x++) {
            for(int y = 0; y < m_levelValues.subsectionBorderedSideCountY; y++) {
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

void MapProcessor::PrepareSectionData(
        SectionState& sectionState,
        const Vector2Int& currentSectionPos) {

    for(int x = 0; x < sectionState.foliageMap.dim_a(); x++) {
        for(int y = 0; y < sectionState.foliageMap.dim_b(); y++) {
            Vector2Int fullCoord = relations::convert_to_global(
                currentSectionPos.x, currentSectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;

            // Reset main array.
            sectionState.foliageMap[x][y] = FoliageHelpers::NO_SELECTION_INDEX;

            FoliageData foliageData = sectionState.foliageDataMap[x][y];

            // Outside main map.
            if(fullX < 0 || fullY < 0 ||
                    fullX >= m_fullFoliageDataMap.dim_a() ||
                    fullY >= m_fullFoliageDataMap.dim_b()) {
                foliageData.set_remaining(
                    m_biomeFoliageInfo.GetWalkableFoliagePriority());
                sectionState.foliageDataMap[x][y] = foliageData;
                sectionState.requiresPropagationMap[x][y] = true;
                continue;
            }

            // We must have checked if we are outside the full map
            // before getting the fullNode.
            FoliageData fullNodeFoliageData = m_fullFoliageDataMap[fullX][fullY];
            foliageData.isWalkableOnly = fullNodeFoliageData.isWalkableOnly;

            bool forcedWalkable = false;
            if(fullNodeFoliageData.isWalkableOnly) {
                forcedWalkable = true;
            }

            // Node has previous data.
            if(fullNodeFoliageData.hasData && fullNodeFoliageData.hasData) {
                if(x < 1 || y < 1 || x >= sectionState.foliageMap.dim_a()-1 || y >= sectionState.foliageMap.dim_b()-1) {
                    foliageData.set_remaining(
                        fullNodeFoliageData.GetRemaining());
                    sectionState.foliageDataMap[x][y] = foliageData;
                    sectionState.requiresPropagationMap[x][y] = true;
                    continue;
                }
            }

            // Node does not have previous data.
            else {
                // Constrain border edges.
                if(x < m_levelValues.borderSideCountX ||
                        y < m_levelValues.borderSideCountY ||
                        x >= sectionState.foliageMap.dim_a()-m_levelValues.borderSideCountX ||
                        y >= sectionState.foliageMap.dim_b()-m_levelValues.borderSideCountY) {
                    foliageData.set_remaining(
                        m_biomeFoliageInfo.GetWalkableFoliagePriority());
                    sectionState.requiresPropagationMap[x][y] = true;

                    sectionState.foliageDataMap[x][y] = foliageData;
                    continue;
                }
            }

            sectionState.requiresPropagationMap[x][y] = forcedWalkable;

            if(forcedWalkable) {
                foliageData.set_remaining(
                    m_biomeFoliageInfo.GetWalkableFoliagePriority());
            }
            else {
                foliageData.set_remaining(
                    m_biomeFoliageInfo.GetFoliagePriority());
            }
            sectionState.foliageDataMap[x][y] = foliageData;
        }
    }
}


bool MapProcessor::RunInitialPropagation(SectionState& sectionState) {
    bool overallFailed = false;

    // Run initial propagation!
    for(int i = 0; i < m_levelValues.subsectionBorderedSideCountX * m_levelValues.subsectionBorderedSideCountY; i++) {
        int x = i % m_levelValues.subsectionBorderedSideCountX;
        int y = static_cast<int>(
            std::floor(static_cast<float>(i) / m_levelValues.subsectionBorderedSideCountX));

        if(!sectionState.requiresPropagationMap[x][y]) {
            continue;
        }
        Vector2Int node (x,y);
        FoliageData nodeData = sectionState.foliageDataMap[node.x][node.y];
        bool success = UpdatePossibleTypes(
            sectionState,
            node,
            nodeData.GetRemaining());
        overallFailed = success ? overallFailed : true;
        if(overallFailed) {
            break;
        }
    }

    return overallFailed;
}


const MapProcessor::FailureResults MapProcessor::HandleFailure(const Vector2Int& currentSectionPos) {

    int secX = currentSectionPos.x;
    int secY = currentSectionPos.y;
    m_overallAttempts++;
    m_currentSubsectionAttempts++;
    m_currentRetryAttempts++;
    if(m_verboseLogging) {
        logger::Log(
            std::format(
            "Processing subsection ({},{}) failed. "
            "Attempts: {}. Overall attempts: {}.",
            secX, secY, m_currentSubsectionAttempts, m_overallAttempts));
    }

    if(m_overallAttempts >= m_levelValues.get_max_attempts()) {
        m_sectionStack.push({secX, secY});
        // failed = true;
        return FailureResults::Abort;
    }
    // If we are on the furthest attempted section,
    // i.e the same as the one we failed.
    if(m_currentIndex > m_furthestIndex || m_furthestIndex == 0) {
        m_currentSubsectionAttempts = 0;
        m_currentRetryAttempts = 0;
        m_retryIndex = 0;
        m_retryList.clear();
        if(secX == 0) {
            if(secY > 0) {
                m_retryList.push_back({secX, secY-1});
            }
            m_retryList.push_back({secX, secY});
            return FailureResults::Retry;
        }
        // The order the subsections are retried matters.
        // They must match the original execution,
        // otherwise the section's seams won't be treated correctly.
        if(secY > 0) {
            m_retryList.push_back({secX-1, secY-1});
        }
        m_retryList.push_back({secX-1, secY});
        if(secY < m_levelValues.subsectionCountY-2) {
            m_retryList.push_back({secX-1, secY+1});
        }
        if(secY > 0) {
            m_retryList.push_back({secX, secY-1});
        }
        m_retryList.push_back({secX, secY});

        // Clear retried data.
        for(Vector2Int retryPos : m_retryList) {
            ClearSubsectionFromFullGrid(retryPos);
        }
    }
    else if(m_currentSubsectionAttempts >= 2) {
        m_currentSubsectionAttempts = 0;
        m_retryIndex = 0;

        // Clear retried data.
        for(Vector2Int retryPos : m_retryList) {
            ClearSubsectionFromFullGrid(retryPos);
        }
    }
    // Default retry.
    else {
        m_retryIndex = std::max(m_retryIndex-2, 0);
        ClearSubsectionFromFullGrid(currentSectionPos);
        ClearSubsectionFromFullGrid(m_retryList[m_retryIndex]);
    }

    return FailureResults::Retry;
}

void  MapProcessor::HandleSuccess(
        bool isFurthestIndex,
        const Vector2Int& currentSectionPos,
        const SectionState& sectionState) {
    if(isFurthestIndex) {
        if(m_retryList.size() > 0) {
            m_retryIndex = 0;
            m_retryList = std::vector<Vector2Int>();
        }
        m_currentSubsectionAttempts = 0;
        m_currentRetryAttempts = 0;
        m_furthestIndex = m_currentIndex;
        m_furthestSectionPos = currentSectionPos;
    }

    if(m_verboseLogging) {
        std::cout << "Writing results: " << std::endl;
        logger::LogGrid(sectionState.foliageDataMap,
            m_biomeFoliageInfo.GetWalkableFoliagePriority());
        logger::log_grid(sectionState.foliageMap);

        logger::Log(std::format(
            "Recursion Count: {} / {} / {}",
            m_recursionCount,
            m_emptyRecursionCount,
            m_emptyRecursionEarlyCount));
    }
}

void MapProcessor::WriteToFullMap(
        SectionState& sectionState,
        const Vector2Int& sectionPos) {
    // Write back to full map without borders.
    // We only write back data to the top left border nodes.
    for(int x = 0; x < sectionState.foliageMap.dim_a(); x++) {
        for(int y = 0; y < sectionState.foliageMap.dim_b(); y++) {
            Vector2Int fullCoord = relations::convert_to_global(
                sectionPos.x, sectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;
            if(fullX < 0 || fullY < 0 ||
                    fullX >= m_fullFoliageMap.dim_a() ||
                    fullY >= m_fullFoliageMap.dim_b()) {
                continue;
            }
            m_fullFoliageMap[fullX][fullY] = sectionState.foliageMap[x][y];
            if(x >= 1 && y >= 1 &&
                    x < sectionState.foliageMap.dim_a()-1 &&
                    y < sectionState.foliageMap.dim_b()-1) {
                m_fullFoliageDataMap[fullX][fullY].set_remaining(
                    sectionState.foliageDataMap[x][y].GetRemaining());
            }
        }
    }
}

void MapProcessor::ClearSubsectionFromFullGrid(const Vector2Int& sectionPos) {

    const int subsectionFullSideCount = m_levelValues.subsectionBorderedSideCountX;

    for(int x = 0; x < subsectionFullSideCount; x++) {
        for(int y = 0; y < subsectionFullSideCount; y++) {
            Vector2Int fullCoord = relations::convert_to_global(sectionPos.x, sectionPos.y, x, y);
            int fullX = fullCoord.x;
            int fullY = fullCoord.y;

            if(fullX < 0 || fullY < 0 || fullX >= m_fullFoliageMap.dim_a() || fullY >= m_fullFoliageMap.dim_b()) {
                continue;
            }

            m_fullFoliageMap[fullX][fullY] = FoliageHelpers::NO_SELECTION_INDEX;

            if(x >= 1 && y >= 1 &&
                    x < subsectionFullSideCount-1 &&
                    y < subsectionFullSideCount-1) {
                // If we are within 1 tile border from the edge.
                if((x < m_levelValues.borderSideCountX ||
                        y < m_levelValues.borderSideCountY ||
                        y >= subsectionFullSideCount-m_levelValues.borderSideCountY) &&
                        // Exclude the right side border.
                        x < subsectionFullSideCount-m_levelValues.borderSideCountX) {
                    // If we are within a 1 tile border, but not within the proper border
                    // (right side border excluded).
                    if(m_fullFoliageDataMap[fullX][fullY].isWalkableOnly) {
                        m_fullFoliageDataMap[fullX][fullY].set_remaining(
                            m_biomeFoliageInfo.GetWalkableFoliagePriority());
                    }
                    else {
                        m_fullFoliageDataMap[fullX][fullY].set_remaining(
                            m_biomeFoliageInfo.GetFoliagePriority());
                    }
                }
                else {
                    m_fullFoliageDataMap[fullX][fullY].hasData = false;
                }
            }
            else if(x >= subsectionFullSideCount-1 &&
                    y >= 1 && y < subsectionFullSideCount-1) {
                m_fullFoliageDataMap[fullX][fullY].hasData = false;
            }
        }
    }
}

std::optional<std::pair<Matrix<FoliageType>, Matrix<FoliageData>>> MapProcessor::ProcessSubsection(
        SectionState& sectionState) {

    Matrix<FoliageType>& foliageMap = sectionState.foliageMap;
    Matrix<FoliageData>& foliageDataMap = sectionState.foliageDataMap;

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
        Vector2Int nextNode = GetNextBestNode(
            foliageDataMap,
            remainingNodes);
        remainingNodes.erase(nextNode);
        bool success = AssignFoliage(
            foliageMap[nextNode.x][nextNode.y],
            sectionState,
            nextNode);
        
        if(!success) {
            return std::nullopt;
        }
    }

    return std::pair(foliageMap, foliageDataMap);
}

Vector2Int MapProcessor::GetNextBestNode(
        Matrix<FoliageData>& foliageDataGrid,
        const std::unordered_set<Vector2Int>& remainingNodes) {
    float bestEntropy = std::numeric_limits<float>::max();
    Vector2Int bestNode = (*remainingNodes.begin());
    std::vector<Vector2Int> possibleBestNodes = {};
    for(Vector2Int node : remainingNodes) {

        // Naive entropy.
        float entropy = foliageDataGrid[node.x][node.y].GetRemainingLength();
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

bool MapProcessor::AssignFoliage(
        const FoliageType newType,
        SectionState& sectionState,
        const Vector2Int& currentNodePos) {

    FoliageData nodeData = sectionState.foliageDataMap[currentNodePos.x][currentNodePos.y];

    FoliageType currentFoliageType = newType;
    if(currentFoliageType == FoliageHelpers::NO_SELECTION_INDEX) {

        currentFoliageType = MakeSelection(
            nodeData.GetRemaining(),
            m_biomeFoliageInfo.GetFoliagePriority());
        if(currentFoliageType == FoliageHelpers::NO_SELECTION_INDEX) {
            // Something went wrong while selecting the next type.
            // Most likely there are no avilable foliage types left.
            // Generation will have to be re-done.
            std::cout << std::format(
                "Assigning foliage failed. Possible types: {}",
                nodeData.GetRemaining().size()) << std::endl;
            return false;
        }
    }

    // Update neighbour possible types with neighbour bonus values.
    auto& neighbourBonusList = m_biomeFoliageInfo.neighbourBonusList;
    size_t bonusTypeCount = neighbourBonusList.size_b(currentFoliageType);
    if(bonusTypeCount != 0) {
        auto neighbours = relations::GetNeighbours(
            currentNodePos, sectionState.foliageDataMap.dim_a());
        for(auto neighbourPos : neighbours) {
            auto selectedFoliageType = sectionState.foliageMap[neighbourPos.x][neighbourPos.y];
            if(selectedFoliageType != FoliageHelpers::NO_SELECTION_INDEX) {
                continue;
            }
            auto neighbourData = sectionState.foliageDataMap[neighbourPos.x][neighbourPos.y];
            for(int i = 0; i < bonusTypeCount; i++) {

                auto bonus = neighbourBonusList.at(currentFoliageType, i);
                if(bonus.second <= 0) {
                    continue;
                }

                auto remaining = neighbourData.GetRemaining();

                if(remaining[bonus.first] <= 0) {
                    continue;
                }
                remaining[bonus.first] += bonus.second;
            }
        }
    }

    sectionState.foliageMap[currentNodePos.x][currentNodePos.y] = currentFoliageType;

    std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> newRemainingPossibleTypes = {};
    newRemainingPossibleTypes[currentFoliageType] = 1;

    bool success = UpdatePossibleTypes(
        sectionState,
        currentNodePos,
        newRemainingPossibleTypes);

    return success;
}

template<size_t foliageCount>
bool MapProcessor::UpdatePossibleTypes(
        SectionState& sectionState,
        const Vector2Int& initialCurrentNodePos,
        const std::array<int, foliageCount>& remainingPossibleTypes) {

    Matrix<FoliageType>& foliageMap = sectionState.foliageMap;
    Matrix<FoliageData>& foliageDataMap = sectionState.foliageDataMap;

    Matrix<int> propagatedNodes (
        foliageDataMap.dim_a(),
        foliageDataMap.dim_b());
    auto actionQueue = std::queue<std::pair<Vector2Int,Vector2Int>>();

    for(int x = 0; x < propagatedNodes.dim_a(); x++) {
        for(int y = 0; y < propagatedNodes.dim_b(); y++) {
            propagatedNodes[x][y] = false;
        }
    }

    std::vector<Vector2Int> unfilteredNeighbours;
    unfilteredNeighbours = relations::GetNeighbours(
        initialCurrentNodePos, foliageDataMap.dim_a());

    for(Vector2Int nnp : unfilteredNeighbours) {

        if(foliageMap[nnp.x][nnp.y] != FoliageHelpers::NO_SELECTION_INDEX) {
            continue;
        }

        Vector2Int neighbourNodePos = nnp;

        propagatedNodes[nnp.x][nnp.y] = true;
        actionQueue.push({initialCurrentNodePos, neighbourNodePos});
    }

    while(actionQueue.size() > 0) {
        auto pair = actionQueue.front();
        actionQueue.pop();
        Vector2Int currentNodePos = pair.first;
        Vector2Int neighbourNodePos = pair.second;

        auto remaining = remainingPossibleTypes;

        if(currentNodePos != initialCurrentNodePos) {
            FoliageData nodeData = foliageDataMap[currentNodePos.x][currentNodePos.y];
            remaining = nodeData.GetRemaining();
        }

        bool success = UpdatePossibleTypesRecursively(
            sectionState,
            propagatedNodes,
            actionQueue,
            currentNodePos,
            neighbourNodePos,
            remaining);
        if(!success) {
            return false;
        }
    }
    return true;
}

template<size_t foliageCount>
bool MapProcessor::UpdatePossibleTypesRecursively(
        SectionState& sectionState,
        Matrix<int>& propagatedNodes,
        std::queue<std::pair<Vector2Int,Vector2Int>>& actionQueue,
        const Vector2Int& lastNodePos,
        const Vector2Int& currentNodePos,
        const std::array<int, foliageCount>& lastNodePossibleTypes) {

    m_recursionCount++;

    Matrix<FoliageType>& foliageMap = sectionState.foliageMap;
    Matrix<FoliageData>& foliageDataMap = sectionState.foliageDataMap;

    auto res = m_biomeFoliageInfo.GetRelationsFromNodes(lastNodePos, currentNodePos);
    auto lastNodeDirectionalRelations = res.first;
    Direction direction = res.second;

    // We find the types in the remove dict that any
    // remaining possible types all have in common.
    // These are no longer viable as possible neighbours.
    if(!lastNodeDirectionalRelations) {
        logger::Log("Directional WFC array does not exist, or could not be found.");
    }

    m_stillPossibleBuffer.fill(0);

    for(int foliageIndex = 0; foliageIndex < lastNodePossibleTypes.size(); foliageIndex++) {
        // This foliage was not possible from the last node.
        if(lastNodePossibleTypes[foliageIndex] <= 0) {
            continue;
        }
        FoliageRelation relationFoliageTypes = (*lastNodeDirectionalRelations)[foliageIndex];
        if(relationFoliageTypes.size() == 0) {
            continue;
        }
        for(size_t i = 0; i < relationFoliageTypes.size(); i++) {
            int foliageType = relationFoliageTypes[i];
            m_stillPossibleBuffer[foliageType] = 1;
        }
    }

    FoliageData& nodeData = foliageDataMap[currentNodePos.x][currentNodePos.y];
    auto remainingPossibleTypes = nodeData.GetRemaining();

    int remainingCount = 0;
    int newlyRemovedCount = 0;

    for(int foliageIndex = 0; foliageIndex < remainingPossibleTypes.size(); foliageIndex++) {

        if(remainingPossibleTypes[foliageIndex] <= 0) {
            continue;
        }

        if(m_stillPossibleBuffer[foliageIndex] <= 0) {
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

    nodeData.set_remaining(remainingPossibleTypes);

    if(remainingCount == 0) {
        if(m_verboseLogging) {
            std::vector<std::string> lastNodePossibleTypesStrings = {};
            for(int value : lastNodePossibleTypes) {
                lastNodePossibleTypesStrings.push_back(
                    std::to_string(value));}
            std::string lastNodePossibleTypesString = DiskManager::join(
                lastNodePossibleTypesStrings, ", ");
            logger::Log(std::format(
                "Node no longer has any remaining possible types. {0}\n"
                "Direction: {1}\n"
                "Removed types: {2}\n"
                "Last node possible types {3}: {4}",
                    currentNodePos.ToString(),
                    static_cast<int>(direction),
                    newlyRemovedCount,
                    lastNodePos.ToString(),
                    lastNodePossibleTypesString));

            logger::LogGrid(
                foliageDataMap,
                m_biomeFoliageInfo.GetWalkableFoliagePriority());
            logger::log_grid(foliageMap);

            auto neighbours = relations::GetNeighbours(
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
            logger::Log("No more possible types available. Last node: "+neighboursString);
        }
        return false;
    }

    bool use4WayNeighbours = false;

    // Find out what types are not impossible for neighbours.
    std::vector<Vector2Int> unfilteredNeighbours;
    if(use4WayNeighbours) {
        unfilteredNeighbours = relations::get_4_way_neighbours(
            currentNodePos, foliageDataMap.dim_a());
    }
    else {
        unfilteredNeighbours = relations::GetNeighbours(
            currentNodePos, foliageDataMap.dim_a());
    }


    for(Vector2Int nnp : unfilteredNeighbours) {
        if(nnp == lastNodePos) {
            continue;
        }
        if(propagatedNodes[nnp.x][nnp.y]) {
            continue;
        }
        if(foliageMap[nnp.x][nnp.y] != FoliageHelpers::NO_SELECTION_INDEX) {
            continue;
        }

        propagatedNodes[nnp.x][nnp.y] = true;

        Vector2Int neighbourNodePos = nnp;
        actionQueue.push({currentNodePos,neighbourNodePos});
    }

    return true;
}

template<size_t foliageCount>
int MapProcessor::MakeSelection(
        const std::array<int, foliageCount>& foliagePriority,
        const std::array<int, foliageCount>& mainWeights) {

    // Tally total score.
    int total = 0;
    for(int foliageIndex = 0; foliageIndex < foliagePriority.size(); foliageIndex++) {
        int typeWeight = foliagePriority[foliageIndex];
        // We ignore types that have been fully removed.
        if(typeWeight <= 0) {
            continue;
        }
        total += mainWeights[foliageIndex] + typeWeight;
    }

    if(total == 0) {
        return FoliageHelpers::NO_SELECTION_INDEX;
    }

    // We do this to have the value scale together with the node size.
    int randomVal = std::rand() % total;
    int currentVal = 0;
    for(int foliageIndex = 0; foliageIndex < foliagePriority.size(); foliageIndex++) {
        int typeWeight = foliagePriority[foliageIndex];
        if(typeWeight <= 0) {
            continue;
        }
        currentVal += mainWeights[foliageIndex] + typeWeight;
        if(randomVal < currentVal) {
            return static_cast<int>(foliageIndex);
        }
    }
    return FoliageHelpers::NO_SELECTION_INDEX;
}


