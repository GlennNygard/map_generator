#pragma once

#include <queue>

#include "map_definitions.h"
#include "foliage_data.h"
#include "foliage.h"
#include "biome_foliage_info.h"


struct TimeTracker {
	long requiredTime = 0;
	long retryTime = 0;
	std::chrono::milliseconds startTime;

    TimeTracker() {
		startTime = GetTime();
	}

	TimeTracker(std::chrono::milliseconds time) {
		startTime = time;
	}

	void AddRetryTime(std::chrono::milliseconds time) {
		retryTime += time.count();
	}

	void AddRequiredTime(std::chrono::milliseconds time) {
		requiredTime += time.count();
	}

	void LogTime(int overallAttempts, const LevelValues& levelValues) {
		float executionTime = format_time(requiredTime + retryTime);
		float timePerSubsection = (executionTime/(
			levelValues.subsectionCountX * levelValues.subsectionCountY));
		float effectiveExecutionTime = format_time(requiredTime);
		float effectiveTimePerSubsection = (effectiveExecutionTime/((
			levelValues.subsectionCountX * levelValues.subsectionCountY)-overallAttempts));

		logger::Log(
			std::format("Time: {}. ", format_time((GetTime() - startTime).count())) +
			std::format("Time to execute. Effective: {:.2f}s. All: {:.2f}s. ",
				effectiveExecutionTime, executionTime) +
			std::format("Avg time per subsection. Effective: {:.2f}s. ",
				effectiveTimePerSubsection) +
			std::format(" (All: {:.2f}s)",timePerSubsection));
	}

	static const std::chrono::milliseconds GetTime() {
		std::chrono::milliseconds startTime = duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		return startTime;
	}

	static const float format_time(long count) {
		return static_cast<float>(count) / 1000;
	}
};


struct SectionState {
	Matrix<FoliageType> foliageMap;
	Matrix<FoliageData> foliageDataMap;
	Matrix<char> requiresPropagationMap;

    SectionState() = default;

    SectionState (const SectionState& other) = delete;
    void operator= (const SectionState& other) = delete;

    // Move constructor
    SectionState(SectionState&& other) noexcept
        : foliageMap(std::move(other.foliageMap)),
          foliageDataMap(std::move(other.foliageDataMap)),
          requiresPropagationMap(std::move(other.requiresPropagationMap))
    {}

    // Move assignment operator
    SectionState& operator=(SectionState&& other) noexcept {
        if (this != &other) {
            foliageMap = std::move(other.foliageMap);
            foliageDataMap = std::move(other.foliageDataMap);
            requiresPropagationMap = std::move(other.requiresPropagationMap);
        }
        return *this;
    }
};


class MapProcessor {


public:

    enum class FailureResults {
        Abort,
        Retry,
    };

    MapProcessor(
        BiomeFoliageInfo& biomeFoliageInfo,
        const LevelValues& levelValues,
        bool verboseLogging);

    std::pair<Matrix<FoliageType>, bool> RunProcessing();


private:

    Matrix<FoliageType> m_fullFoliageMap;
	Matrix<FoliageData> m_fullFoliageDataMap;
	std::stack<Vector2Int> m_sectionStack;
	std::vector<Vector2Int> m_retryList;
	int m_retryIndex = 0;
	int m_currentIndex = 0;
	int m_furthestIndex = 0;
	Vector2Int m_furthestSectionPos;
	int m_overallAttempts = 0;
	int m_currentSubsectionAttempts = 0;
	int m_currentRetryAttempts = 0;

    bool m_verboseLogging;
	bool m_issuesFound;

	BiomeFoliageInfo m_biomeFoliageInfo;
	const LevelValues m_levelValues;

	int m_recursionCount;
	int m_emptyRecursionCount;
	int m_emptyRecursionEarlyCount;

    std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> m_stillPossibleBuffer;


    SectionState InitialiseSectionState() const;

	void PrepareFullData (
		Matrix<FoliageData>& fullFoliageDataMap) const;

	void PrepareSectionData(
        SectionState& sectionState,
        const Vector2Int& currentSectionPos);

    const Vector2Int GetNextSectionPos();

    void HandleSuccess(
        bool isFurthestIndex,
        const Vector2Int& currentSectionPos,
        const SectionState& sectionState);
    const FailureResults HandleFailure(const Vector2Int& currentSectionPos);

	void WriteToFullMap(
        SectionState& sectionState,
        const Vector2Int& sectionPos);

	void ClearSubsectionFromFullGrid(const Vector2Int& sectionPos);

    std::optional<std::pair<Matrix<FoliageType>, Matrix<FoliageData>>> ProcessSubsection(
		SectionState& sectionState);

	Vector2Int GetNextBestNode(
		Matrix<FoliageData>& foliageDataGrid,
		const std::unordered_set<Vector2Int>& remainingNodes);

    bool RunInitialPropagation(SectionState& sectionState);

	bool AssignFoliage(
		const FoliageType newType,
		SectionState& sectionState,
		const Vector2Int& currentNodePos);

	template<size_t foliageCount>
	bool UpdatePossibleTypes(
		SectionState& sectionState,
		const Vector2Int& initialCurrentNodePos,
		const std::array<int, foliageCount>& remainingPossibleTypes);

	template<size_t foliageCount>
	bool UpdatePossibleTypesRecursively(
		SectionState& sectionState,
		Matrix<int>& propagatedNodes,
		std::queue<std::pair<Vector2Int,Vector2Int>>& actionQueue,
		const Vector2Int& lastNodePos,
		const Vector2Int& currentNodePos,
		const std::array<int, foliageCount>& lastNodePossibleTypes);

	template<size_t foliageCount>
	int MakeSelection(
		const std::array<int, foliageCount>& foliagePriority,
		const std::array<int, foliageCount>& mainWeights);
};
