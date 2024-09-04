#pragma once

#include <vector>
#include <iostream>
#include <format>
#include <string>

#include "../foliage/foliage.h"
#include "../foliage_info.h"
#include "../map_definitions.h"


class FoliageChasmSetup {

    public:

    static const void Setup(
			std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> &mainDict,
			int mapIndexStart) {

        Colour colour (0.05, 0.05, 0.05);
        float foliageHeight = 0;

		std::vector<FoliageInfo> foliageList = {};

        std::vector<FoliageType> directionSet = {
            FoliageType::Foliage_ChasmWallUp,
			FoliageType::Foliage_ChasmWallDown,
			FoliageType::Foliage_ChasmWallLeft,
			FoliageType::Foliage_ChasmWallRight,
			// 4

			FoliageType::Foliage_ChasmWallUpLeft,
			FoliageType::Foliage_ChasmWallUpRight,
			FoliageType::Foliage_ChasmWallDownLeft,
			FoliageType::Foliage_ChasmWallDownRight,
			FoliageType::Foliage_ChasmMiddle,
			// 9

			FoliageType::Foliage_ChasmWallThreeUp,
			FoliageType::Foliage_ChasmWallThreeDown,
			FoliageType::Foliage_ChasmWallThreeLeft,
			FoliageType::Foliage_ChasmWallThreeRight,

			FoliageType::Foliage_ChasmWallFrontBackUp,
			FoliageType::Foliage_ChasmWallFrontBackLeft,

			FoliageType::Foliage_ChasmWallAll,
			// 16

			FoliageType::Foliage_ChasmWallCenterUpLeft,
			FoliageType::Foliage_ChasmWallCenterUpRight,
			FoliageType::Foliage_ChasmWallCenterDownLeft,
			FoliageType::Foliage_ChasmWallCenterDownRight,

			FoliageType::Foliage_ChasmWallInsideUp,
			FoliageType::Foliage_ChasmWallInsideDown,
			FoliageType::Foliage_ChasmWallInsideLeft,
			FoliageType::Foliage_ChasmWallInsideRight,
			// 24

			FoliageType::Foliage_ChasmWallCenterInsideUpLeft,
			FoliageType::Foliage_ChasmWallCenterInsideUpRight,
			FoliageType::Foliage_ChasmWallCenterInsideDownLeft,
			FoliageType::Foliage_ChasmWallCenterInsideDownRight,

			FoliageType::Foliage_ChasmWallInsideUpLeft,
			FoliageType::Foliage_ChasmWallInsideUpRight,
			FoliageType::Foliage_ChasmWallInsideDownLeft,
			FoliageType::Foliage_ChasmWallInsideDownRight,
			// 32

			FoliageType::Foliage_ChasmWallDiagonalUpLeft,
			FoliageType::Foliage_ChasmWallDiagonalUpRight,
			FoliageType::Foliage_ChasmWallDiagonalDownLeft,
			FoliageType::Foliage_ChasmWallDiagonalDownRight,
			// 36
        };

        foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallUp,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 1,
			LevelBiome::None,
			Direction::DirectionUp)
            .SetHeight(foliageHeight));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallDown,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 2,
			LevelBiome::None,
			Direction::DirectionDown)
			.SetPossibleRotations({180}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 3,
			LevelBiome::None,
			Direction::DirectionLeft)
			.SetPossibleRotations({270}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallRight,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 4,
			LevelBiome::None,
			Direction::DirectionRight)
			.SetPossibleRotations({90}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallUpLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 5,
			LevelBiome::None,
			Direction::DirectionUpLeft));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallUpRight,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 6,
			LevelBiome::None,
			Direction::DirectionUpRight)
			.SetPossibleRotations({90}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallDownLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 7,
			LevelBiome::None,
			Direction::DirectionDownLeft)
			.SetPossibleRotations({270}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallDownRight,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 8,
			LevelBiome::None,
			Direction::DirectionDownRight)
			.SetPossibleRotations({180}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmMiddle,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 9,
			LevelBiome::None,
			Direction::DirectionNone));


		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallThreeUp,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 10,
			LevelBiome::None,
			Direction::DirectionUp)
            .SetHeight(foliageHeight));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallThreeDown,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 11,
			LevelBiome::None,
			Direction::DirectionDown)
            .SetHeight(foliageHeight)
			.SetPossibleRotations({180}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallThreeLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 12,
			LevelBiome::None,
			Direction::DirectionLeft)
			.SetPossibleRotations({270}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallThreeRight,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 13,
			LevelBiome::None,
			Direction::DirectionRight)
			.SetPossibleRotations({90}));


		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallFrontBackUp,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 14,
			LevelBiome::None,
			Direction::DirectionUp)
			.SetPossibleRotations({90}));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallFrontBackLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 15,
			LevelBiome::None,
			Direction::DirectionDown));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallAll,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 16,
			LevelBiome::None,
			Direction::DirectionDown));


		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallCenterUpLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 17,
			LevelBiome::None,
			Direction::DirectionUpLeft));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallCenterUpRight,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 18,
			LevelBiome::None,
			Direction::DirectionUpRight));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallCenterDownLeft,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 19,
			LevelBiome::None,
			Direction::DirectionDownLeft));

		foliageList.push_back(FoliageInfo(
			FoliageType::Foliage_ChasmWallCenterDownRight,
			FoliageHelpers::LOW_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 20,
			LevelBiome::None,
			Direction::DirectionDownRight));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideUp)
			.SetMapIndex(mapIndexStart + 21)
			.SetDirection(Direction::DirectionUp)
            .SetHeight(foliageHeight));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideDown)
			.SetMapIndex(mapIndexStart + 22)
			.SetDirection(Direction::DirectionDown)
			.SetPossibleRotations({180}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideLeft)
			.SetMapIndex(mapIndexStart + 23)
			.SetDirection(Direction::DirectionLeft)
			.SetPossibleRotations({270}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideRight)
			.SetMapIndex(mapIndexStart + 24)
			.SetDirection(Direction::DirectionRight)
			.SetPossibleRotations({90}));


		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallCenterInsideUpLeft)
			.SetMapIndex(mapIndexStart + 25)
			.SetDirection(Direction::DirectionUpLeft));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallCenterInsideUpRight)
			.SetMapIndex(mapIndexStart + 26)
			.SetDirection(Direction::DirectionUpRight));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallCenterInsideDownLeft)
			.SetMapIndex(mapIndexStart + 27)
			.SetDirection(Direction::DirectionDownLeft));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallCenterInsideDownRight)
			.SetMapIndex(mapIndexStart + 28)
			.SetDirection(Direction::DirectionDownRight));


		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideUpLeft)
			.SetMapIndex(mapIndexStart + 29)
			.SetDirection(Direction::DirectionUpLeft)
			.SetPossibleRotations({0}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideUpRight)
			.SetMapIndex(mapIndexStart + 30)
			.SetDirection(Direction::DirectionUpRight)
			.SetPossibleRotations({90}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideDownLeft)
			.SetMapIndex(mapIndexStart + 31)
			.SetDirection(Direction::DirectionDownLeft)
			.SetPossibleRotations({270}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallInsideDownRight)
			.SetMapIndex(mapIndexStart + 32)
			.SetDirection(Direction::DirectionDownRight)
			.SetPossibleRotations({180}));


		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallDiagonalUpLeft)
			.SetMapIndex(mapIndexStart + 33)
			.SetDirection(Direction::DirectionUpLeft)
			.SetPossibleRotations({0}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallDiagonalUpRight)
			.SetMapIndex(mapIndexStart + 34)
			.SetDirection(Direction::DirectionUpRight)
			.SetPossibleRotations({90}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallDiagonalDownLeft)
			.SetMapIndex(mapIndexStart + 35)
			.SetDirection(Direction::DirectionDownLeft)
			.SetPossibleRotations({270}));

		foliageList.push_back(FoliageInfo()
			.SetFoliageType(FoliageType::Foliage_ChasmWallDiagonalDownRight)
			.SetMapIndex(mapIndexStart + 36)
			.SetDirection(Direction::DirectionDownRight)
			.SetPossibleRotations({180}));

		for(auto info : foliageList) {
			info.SetNodeType(FoliageHelpers::LOW_GROUND_NODE_TYPE)
				.SetNodeBiome(LevelBiome::None)
				.SetWalkable(false)
				.SetBuildable(false)
				.SetColour(colour)
				.SetHeight(foliageHeight)
				.SetDirectionSet(directionSet);

			// Write data to main dict.
			int foliageIndex = static_cast<int>(info.foliageType);
			if(foliageIndex < 0 || foliageIndex >=  static_cast<int>(FoliageType::Foliage_MAX)) {
				std::cerr << std::format("Foliage index {} not in range.", foliageIndex) << std::endl;
				continue;
			}
			mainDict[foliageIndex] = info;
		}
    }
};
