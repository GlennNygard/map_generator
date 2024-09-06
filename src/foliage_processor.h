#pragma once

#include <iostream>
#include <random>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <chrono>
#include <cmath>
#include <limits>

#include "map/map_definitions.h"
#include "foliage.h"
#include "biome_foliage_info.h"
#include "gmath.h"


struct FoliageData {

	public:

	bool hasData;
	bool isWalkableOnly;

	FoliageData() {
		hasData = false;
		isWalkableOnly = false;
		_dirty = false;
		_remainingLength = 0;
	}

	std::optional<std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>> get_remaining_possible_types() {
		_dirty = true;
		return _remainingPossibleTypes;
	};

	template<size_t size>
	void set_remaining_possible_types(std::array<int, size> dataArr) {
		_remainingPossibleTypes = dataArr;
		_dirty = true;
		hasData = true;
	};

	int get_remaining_length() {
		if(_dirty) {
			if(!hasData) {
				_remainingLength = static_cast<int>(FoliageType::Foliage_MAX);
			}
			else {
				int remCount = 0;
				for(int i = 0; i < static_cast<int>(FoliageType::Foliage_MAX); i++) {
					if((*_remainingPossibleTypes)[i] <= 0) {
						continue;
					}
					remCount++;
				}
				_remainingLength = remCount;
			}
			_dirty = false;
		}
		return _remainingLength;
	}

	template<size_t size>
	void set_remaining_from_array(std::array<int, size> dataArr) {
		if(!_remainingPossibleTypes) {
			_remainingPossibleTypes = std::optional<std::array<int, size>>();
		}

		int dataArrLength = size;
		for(int i = 0; i < dataArrLength; i++) {
			(*_remainingPossibleTypes)[i] = dataArr[i];
		}
		_dirty = true;
		hasData = true;
	}

	private:

	std::optional<std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>> _remainingPossibleTypes;
	int _remainingLength;
	bool _dirty;



	// FoliageData() {
	// 	HasData = false;
	// 	_dirty = false;
	// 	_remainingPossibleTypes = new int[FoliageHelpers.FoliageCount];
	// 	_remainingLength = FoliageHelpers.FoliageCount;
	// }
};


class FoliageProcessor {
	public:

    FoliageProcessor(BiomeFoliageInfo biomeFoliageInfo, bool verboseLogging) {
		_issuesFound = false;
		_biomeFoliageInfo = biomeFoliageInfo;
		_verboseLogging = verboseLogging;

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); // distribution in range [1, 6]
    }

	std::chrono::milliseconds getTime() {
		std::chrono::milliseconds startTime = duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		return startTime;
	}

	std::pair<Matrix<FoliageType>, bool> mark_foliage_nodes(
			Matrix<int> fullNodeMap,
			int gridStartX, int gridStartY,
			int gridEndX, int gridEndY,
			int sideCountX, int sideCountY
			) {

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
					if(RUN_PRINTS && _verboseLogging) {

					}
					// Logger.Log(
					// 	"Current: "+currentSubsectionAttempts + "    Overall: "+overallAttempts+ "   Retries: "+retryIndex+"   Count: "+retryList.Count
					// 	+"   Furthest: "+furthestIndex
					// 	+"   Last current ind: "+currentIndex
					// );
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

			if(RUN_PRINTS && _verboseLogging) {
				std::cout << "STARTING ATTEMPT "
					<< secX << ", " << secY
					<< ". Furthest: "
					<< furthestSectionPos.x << ", " << furthestSectionPos.y
					<< ". Remaining in stack: " << sectionStack.size()
					<< std::endl;
			}

			_recursionCount = 0;
			_emptyRecursionCount = 0;
			_emptyRecursionEarlyCount = 0;

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

			print_grid(foliageDataMap);

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
				// Logger.LogError("Initial propagation failed. Should not happen.");
				std::cout << "Initial propagation failed. Should not happen." << std::endl;
			}

			auto resultsFoliageMap = Matrix<FoliageType>(
				MapDefinitions::SUBSECTION_FULL_SIZE,
				MapDefinitions::SUBSECTION_FULL_SIZE);
			auto resultsFoliageDataMap = Matrix<FoliageData>(
				MapDefinitions::SUBSECTION_FULL_SIZE,
				MapDefinitions::SUBSECTION_FULL_SIZE);
			// FoliageData[,] resultsFoliageDataMap = null;

			print_grid(foliageDataMap);
			if(!overallFailed) {

				auto processResult = process_subsection(
					foliageMap,
					foliageDataMap
					// pseudoRandom
					);
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
				if(RUN_PRINTS && _verboseLogging) {
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
				// retryTime += Time.realtimeSinceStartupAsDouble - time;
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

			if(RUN_GRID_PRINTS && _verboseLogging) {
				std::cout << "Writing results: " << std::endl;
				print_grid(resultsFoliageDataMap);
				print_grid(resultsFoliageMap);
			}
			if(RUN_PRINTS && _verboseLogging) {
				std::cout << std::format(
					"Recursion Count: {} / {} / {}",
					_recursionCount,
					_emptyRecursionCount,
					_emptyRecursionEarlyCount) << std::endl;
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

			// Logger.Log("Time: "+(Time.time - time));

			// secureX = Mathf.Max(secX, secureX);
			// currentAttempts = 0;
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

		// std::cout << "Printing full grid." << std::endl;
		// print_grid(fullFoliageMap);

		std::cout <<
			std::format("Total sections: {}. ", subsectionCountX*subsectionCountY) <<
			std::format("Failed attempts: {}. ", overallAttempts)  <<
			std::format("Time: {} - {}. ", (getTime() - startTime).count(), startTime.count()) <<
			std::format("Time to execute. Effective: {}. All: {}. ",effectiveExecutionTime, executionTime) <<
			std::format("Avg time per subsection. Effective: {}. ",effectiveTimePerSubsection) <<
			std::format(" (All: {})",timePerSubsection) << std::endl;

		return std::pair(fullFoliageMap, !failed);
	};

	void clear_subsection(
			Matrix<FoliageType>& fullFoliageMap,
			Matrix<FoliageData>& fullFoliageDataMap,
			const Vector2Int sectionPos,
			const int gridStartX,
			const int gridStartY) {
		for(int x = 0; x < MapDefinitions::SUBSECTION_FULL_SIZE; x++) {
			for(int y = 0; y < MapDefinitions::SUBSECTION_FULL_SIZE; y++) {
				// int fullX = gridStartX + (MapDefinitions::SUBSECTION_SIZE * sectionPos.x) - MapDefinitions::SUBSECTION_BORDER + x;
				// int fullY = gridStartY + (MapDefinitions::SUBSECTION_SIZE * sectionPos.y) - MapDefinitions::SUBSECTION_BORDER + y;
				// (int fullX, int fullY) = convert_to_global(sectionPos.x, sectionPos.y, x, y);
				Vector2Int fullCoord = convert_to_global(sectionPos.x, sectionPos.y, x, y);
				int fullX = fullCoord.x;
				int fullY = fullCoord.y;

				if(fullX < 0 || fullY < 0 || fullX >= MapDefinitions::FULL_GRID_COUNT_X || fullY >= MapDefinitions::FULL_GRID_COUNT_Y) {
					continue;
				}

				fullFoliageMap[fullX][fullY] = FoliageType::Foliage_NoSelection;

				// if(x >= MapDefinitions::SUBSECTION_BORDER) {

				// }

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

	Vector2Int convert_to_global(int secX, int secY, int x, int y) {
		int fullX = ((MapDefinitions::SUBSECTION_SIZE * secX) - MapDefinitions::SUBSECTION_BORDER) + x;
		int fullY = ((MapDefinitions::SUBSECTION_SIZE * secY) - MapDefinitions::SUBSECTION_BORDER) + y;
		return {fullX,fullY};
	}

	void print_grid(Matrix<FoliageData> grid) {
		if(!RUN_PRINTS) {
			return;
		}
		if(!RUN_GRID_PRINTS) {
			return;
		}
		// if(!GlobalGameState.InDebugMode()) {
		// 	return;
		// }
        // int rows = grid.GetLength(0);
        // int cols = grid.GetLength(1);

		int rows = grid.dim_a();
        int cols = grid.dim_b();

        // Loop through each row.
		std::string fullStringPossible = "";
        for(int y = rows-1; y >= 0; y--) {
			fullStringPossible += "\n";
            // Loop through each column.
            for(int x = 0; x < cols; x++) {
                // rowString += grid[i, j] + "\t"; // Add each element of the grid followed by a tab
				FoliageData foliageData = grid[x][y];

				auto dataRemaining = (*foliageData.get_remaining_possible_types());

				auto defaultWalkable = _biomeFoliageInfo.WalkablePossibleTypes;
				bool allSame = true;
				for(size_t i = 0; i < dataRemaining.size(); i++) {
					if(dataRemaining[i] != defaultWalkable[i]) {
						allSame = false;
						break;
					}
				}

				if(allSame) {
					fullStringPossible += std::format("{},{} - DEF WPT \t",x, y);
					continue;
				}

				std::vector<FoliageType> remainingPossibleTypes = {};
				for(int i = 0; i < dataRemaining.size(); i++) {
					if(dataRemaining[i] <= 0) {
						continue;
					}
					remainingPossibleTypes.push_back((FoliageType)i);
				}

				if(remainingPossibleTypes.size() == 1) {
					fullStringPossible += std::format("{},{} - {} \t",x, y, static_cast<int>(remainingPossibleTypes[0]));
				}
				else {
					fullStringPossible += std::format("{},{} - {} \t\t",x, y, remainingPossibleTypes.size());
				}
            }

        }
		std::cout << fullStringPossible << std::endl;
    }

	void print_grid(Matrix<FoliageType> grid) {
		if(!RUN_PRINTS) {
			return;
		}
		if(!RUN_GRID_PRINTS) {
			return;
		}
		// if(!GlobalGameState.InDebugMode()) {
		// 	return;
		// }
        // int rows = grid.GetLength(0);
        // int cols = grid.GetLength(1);

		int rows = grid.dim_a();
        int cols = grid.dim_b();

        // Loop through each row.
		std::string fullStringTypes = "";
        for(int y = rows-1; y >= 0; y--) {
			fullStringTypes += "\n";
            // Loop through each column.
            for (int x = 0; x < cols; x++) {
                // rowString += grid[i, j] + "\t"; // Add each element of the grid followed by a tab
				auto foliageType = grid[x][y];
				// fullStringTypes += "" + {x,y} + " - "+type+ "\t";
				fullStringTypes += std::format(
					"{},{} - {} \t",x, y, static_cast<int>(foliageType));
            }
        }
		// Print the row to the Unity console.
		std::cout << fullStringTypes << std::endl;
    }

    std::optional<std::pair<Matrix<FoliageType>, Matrix<FoliageData>>> process_subsection(
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap
			) {

		_issuesFound = false;

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

	Vector2Int get_next_best_node(
			std::unordered_set<Vector2Int> remainingNodes,
			Matrix<FoliageData>& foliageDataGrid) {
		float bestEntropy = std::numeric_limits<float>::max();
		Vector2Int bestNode = (*remainingNodes.begin());
		std::vector<Vector2Int> possibleBestNodes = {};
		for(Vector2Int node : remainingNodes) {

			// Naive entropy.
			float entropy = foliageDataGrid[node.x][node.y].get_remaining_length();
			// Shannon entropy.
			// float entropy = GetShannonEntropy(foliageDataGrid[node.x, node.y]);
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
			// bestNode = possibleBestNodes[randomGenerator.Next(0,possibleBestNodes.Count)];
		}
		// Logger.Log("Getting next best node: "+bestNode);
		return bestNode;
	}

	// https://robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/
	float GetShannonEntropy(FoliageData data) {
		int remainingLength = data.get_remaining_length();

		auto remaining = (*data.get_remaining_possible_types());
		float weightSum = getSum(remaining);

		float weightSum2 = 0;
		for(int i = 0; i < remaining.size(); i++) {
			float w = remaining[i];
			if(w <= 0) {
				continue;
			}
			weightSum2 += w * std::log(w);
		}

		return std::log(weightSum) - (weightSum2 / weightSum);
	}

	template<size_t size>
	int getSum(std::array<int, size> arr) {
		int sum = 0;
		for(int val : arr) {
			sum += val;
		}
		return sum;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="currentNodePos"></param>
	/// <param name="newType">
	/// Lets us specify a given foliage type directly.
	/// If not set (FoliageType.NoSelection),
	/// one will be found randomly based on valid types.</param>
	/// <param name="foliageInfo"></param>
	/// <param name="gridStartX"></param>
	/// <param name="gridStartY"></param>
	/// <param name="gridEndX"></param>
	/// <param name="gridEndY"></param>
	/// <param name="foliageMap"></param>
	/// <param name="foliageDataMap"></param>
	bool assign_foliage(
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
		else {
			// This must be added in case the selected type is
			// not part of the standard default set.
			// Logger.LogError("Selected foliage is not part of the default set.");
			// throw new AlgorithmDeadlockedException();
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

		// Logger.Log("impossible " +impossibleTypes.Count +"  of  "+data.possibleTypes.Count);

		std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> newRemainingPossibleTypes = {};
		// newRemainingPossibleTypes[(int)currentFoliageType] = nodeData.RemainingPossibleTypes[(int)currentFoliageType];
		newRemainingPossibleTypes[static_cast<int>(currentFoliageType)] = 1;

		bool success = update_possible_types(
			currentNodePos,
			// new int[]{(currentFoliageType, nodeData.remainingPossibleTypes[(int)currentFoliageType])},
			newRemainingPossibleTypes,
			foliageInfo, foliageMap, foliageDataMap);

		return success;
	}

	template<size_t size>
	bool update_possible_types(
			const Vector2Int initialCurrentNodePos,
			const std::array<int, size>& remainingPossibleTypes,
			BiomeFoliageInfo& foliageInfo,
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap){

		// Logger.Log("impossible " +impossibleTypes.Count +"  of  "+data.possibleTypes.Count);

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
			// unfilteredNeighbours = Get4WayNeighbours(currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);

			// var propagatedNodes = new HashSet<Vector2Int>();
			// var actionQueue = new Queue<Func<bool>>();

			for(Vector2Int nnp : unfilteredNeighbours) {
			// while(unfilteredNeighbours.Count > 0) {
			// 	Vector2Int neighbourNodePos = unfilteredNeighbours[randomGenerator.Next(0,unfilteredNeighbours.Count)];
				// unfilteredNeighbours.Remove(neighbourNodePos);

				if(foliageMap[nnp.x][nnp.y] != FoliageType::Foliage_NoSelection) {
					continue;
				}

				Vector2Int neighbourNodePos = nnp;
				

				// propagatedNodes.Add(nnp);
				propagatedNodes[nnp.x][nnp.y] = true;

				// std::function<bool()> lam = [](bool value) {
				// 	// if(neighbourNodePos != null) {
				// 		// if(propagatedNodes.Contains(neighbourNodePos)) {
				// 		// 	return;
				// 		// }

				// 		bool success = UpdatePossibleTypesRecursively(
				// 			currentNodePos,
				// 			neighbourNodePos,
				// 			foliageInfo,
				// 			remainingPossibleTypes,
				// 			// nodeData.remainingPossibleTypes,
				// 			// new Dictionary<FoliageType, int>(){{currentFoliageType, 1}},
				// 			foliageMap,
				// 			foliageDataMap,
				// 			propagatedNodes,
				// 			_actionQueue);
				// 		return success;

				// 	// }
				// 	// return true;
				// }
				_actionQueue.push({initialCurrentNodePos, neighbourNodePos});
			}
		}

		while(_actionQueue.size() > 0) {
			// bool success = _actionQueue.pop()();
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
	bool update_possible_types_recursively(
			const Vector2Int lastNodePos,
			const Vector2Int currentNodePos,
			BiomeFoliageInfo& foliageInfo,
			// Dictionary<FoliageType, int> lastNodePossibleTypes,
			const std::array<int, size>& lastNodePossibleTypes,
			Matrix<FoliageType>& foliageMap,
			Matrix<FoliageData>& foliageDataMap,
			Matrix<int>& propagatedNodes,
			// std::queue<std::function<bool()>> actionQueue
			std::queue<std::pair<Vector2Int,Vector2Int>>& actionQueue
			) {

		// propagatedNodes.Add(currentNodePos);

		_recursionCount++;


		// Dictionary<FoliageType, Dictionary<FoliageType, int>> lastNodeDirectionalRelationsDict = null;
		// Dictionary<FoliageType, HashSet<FoliageType>> lastNodeDirectionalRelationsDict = null;
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

		// var stillPossibleItems = new HashSet<FoliageType>();
		// foreach(var possibleType in lastNodePossibleTypes) {
		for(int foliageIndex = 0; foliageIndex < lastNodePossibleTypes.size(); foliageIndex++) {
			// This foliage was not possible from the last node.
			if(lastNodePossibleTypes[foliageIndex] <= 0) {
				continue;
			}
			auto relationFoliageTypes = (*lastNodeDirectionalRelations)[foliageIndex];
			// if(relationFoliageTypes == null || relationFoliageTypes.Length == 0) {
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

		// var lastFoliageType = foliageMap[lastNodePos.x, lastNodePos.y];

		// foreach(var foliageType in remainingPossibleTypes) {
		for(int foliageIndex = 0; foliageIndex < remainingPossibleTypes.size(); foliageIndex++) {

			if(remainingPossibleTypes[foliageIndex] <= 0) {
				continue;
			}

			if(stillPossibleItems[foliageIndex] <= 0) {
				remainingPossibleTypes[foliageIndex] = 0;
				newlyRemovedCount++;
				continue;
			}

			// newRemainingItems[remainingIndex] = foliageType;
			remainingCount++;
		}

		// int removedTypesCount = prevRemainingCount - remainingCount;

		// Logger.Log(nextNode.gridX + "   "+nextNode.gridY);

		// Logger.Log("removed " +removedTypes +"  of  "+remainingTypes);

		if(newlyRemovedCount == 0) {
			// Logger.Log("Removed types is zero. Returning.");
			_emptyRecursionCount++;
			return true;
		}

		// Logger.Log(currentNodePos + "  Removed: "+newlyRemovedCount+ "   Remaining: "+remainingCount);

		// if(earlyOut) {
		// 	Logger.LogWarning("Early out misjudged.");
		// }

		// Array.Resize(ref newRemainingItems, remainingIndex);

		nodeData.set_remaining_possible_types(remainingPossibleTypes);
		foliageDataMap[currentNodePos.x][currentNodePos.y] = nodeData;

		// if(remainingPossibleTypes.Length == 0) {
		if(remainingCount == 0) {
			if(RUN_PRINTS && _verboseLogging) {
				// std::string lastNodePossibleTypesString = String.Concat(lastNodePossibleTypes.Select((np)=> np+", "));
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

				print_grid(foliageDataMap);
				print_grid(foliageMap);

				auto neighbours = get_neighbours(
					currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
				std::string neighboursString = "";

				std::vector<std::string> neighbourFoliageStrings = {};
				for(auto v2i : neighbours) {
					neighbourFoliageStrings.push_back(
						std::to_string(static_cast<int>(foliageMap[v2i.x][v2i.y])));}
				if(neighbourFoliageStrings.size() == 8) {
					auto nf = neighbourFoliageStrings;
					// var nf = neighbours.Select((np)=> foliageMap[np.x, np.y]).ToArray();
					neighboursString = std::format(
					"{0}, {1}, {2}"
					"{3}, SELECTING, {4}"
					"{5}, {6}, {7}",
						nf[0],nf[1],nf[2],nf[3],nf[4],nf[5],nf[6],nf[7]);
				}
				else {
					// neighboursString = String.Concat(neighbours.Select((np)=> foliageMap[np.x, np.y]+", "));
					std::string neighboursString = DiskManager::join(neighbourFoliageStrings, ", ");
				}
				std::cout << "No more possible types available. Last node: "+neighboursString << std::endl;
			}
			// throw new AlgorithmDeadlockedException();
			return false;
		}

		// Find out what types are not impossible for neighbours.

		std::vector<Vector2Int> unfilteredNeighbours = get_neighbours(
			currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);
		// unfilteredNeighbours = Get4WayNeighbours(currentNodePos, MapDefinitions::SUBSECTION_FULL_SIZE);

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
			// actionQueue.push([&] {

			// 	bool success = UpdatePossibleTypesRecursively(
			// 		currentNodePos,
			// 		neighbourNodePos,
			// 		foliageInfo,
			// 		*nodeData.get_remaining_possible_types(),
			// 		foliageMap,
			// 		foliageDataMap,
			// 		propagatedNodes,
			// 		actionQueue);
			// 	return success;
			// });
		}

		return true;

		// propagatedNodes.Add(currentNodePos);

		// propagatedNodes.Remove(currentNodePos);
	}

	std::vector<Vector2Int> get_neighbours(Vector2Int gridPos, int mapSize) {
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

	std::vector<Vector2Int> Get4WayNeighbours(Vector2Int gridPos, int mapSize) {
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
	FoliageType make_selection(
			const std::array<int, size>& possibleTypes,
			const std::array<int, size>& mainWeights) {

		// if(possibleTypes == null) {
		// 	return FoliageType::Foliage_NoSelection;
		// }

		// Tally total score.
		int total = 0;
		// foreach(var foliageType in possibleTypes) {
		for(int foliageIndex = 0; foliageIndex < possibleTypes.size(); foliageIndex++) {
			int typeWeight = possibleTypes[foliageIndex];
			// We ignore types that have been fully removed.
			if(typeWeight <= 0) {
				continue;
			}
			total += mainWeights[foliageIndex] + typeWeight;
			// total += mainWeights[foliageIndex];
		}

		if(total == 0) {
			return FoliageType::Foliage_NoSelection;
		}

		// We do this to have the value scale together with the node size.
		int randomVal = std::rand() % total;
		int currentVal = 0;
		// foreach(int foliageTypeValue in possibleTypes) {
		for(int foliageIndex = 0; foliageIndex < possibleTypes.size(); foliageIndex++) {
			int typeWeight = possibleTypes[foliageIndex];
			if(typeWeight <= 0) {
				continue;
			}
			currentVal += mainWeights[foliageIndex] + typeWeight;
			// currentVal += mainWeights[foliageIndex];
			if(randomVal < currentVal) {
				return static_cast<FoliageType>(foliageIndex);
			}
		}
		return FoliageType::Foliage_NoSelection;
	}


	private:

	const bool RUN_PRINTS = true;
	const bool RUN_GRID_PRINTS = false;

	bool _verboseLogging;
	bool _issuesFound;
    // System.Random randomGenerator;

	BiomeFoliageInfo _biomeFoliageInfo;

	int _recursionCount = 0;
	int _emptyRecursionCount = 0;
	int _emptyRecursionEarlyCount = 0;
};
