#pragma once

#include <vector>

#include "../foliage/foliage.h"
#include "../foliage_info.h"
#include "../map_definitions.h"


class FoliageWallSetup {

	public:

    static void Setup(std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> &mainDict, int mapIndexStart) {

        // var colour = new Color(0.25, 0.1, 0.1f);
		Colour colour (0.25, 0.1, 0.1);
        float foliageHeight = 0;

        std::vector<FoliageType> directionSet = {
            FoliageType::Foliage_WallUp,
            FoliageType::Foliage_WallDown,
            FoliageType::Foliage_WallLeft,
            FoliageType::Foliage_WallRight,

            FoliageType::Foliage_WallUpLeft,
            FoliageType::Foliage_WallUpRight,
            FoliageType::Foliage_WallDownLeft,
            FoliageType::Foliage_WallDownRight,
            FoliageType::Foliage_WallMiddle,

            FoliageType::Foliage_WallThreeUp,
            FoliageType::Foliage_WallThreeDown,
            FoliageType::Foliage_WallThreeLeft,
            FoliageType::Foliage_WallThreeRight,

            FoliageType::Foliage_WallFrontBackUp,
            FoliageType::Foliage_WallFrontBackLeft,

            FoliageType::Foliage_WallAll,

			FoliageType::Foliage_WallCenterInsideUpLeft,
            FoliageType::Foliage_WallCenterInsideUpRight,
            FoliageType::Foliage_WallCenterInsideDownLeft,
            FoliageType::Foliage_WallCenterInsideDownRight,

			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,

			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,

			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,
			FoliageType::Foliage_NoSelection,

			FoliageType::Foliage_WallDiagonalUpLeft,
			FoliageType::Foliage_WallDiagonalUpRight,
			FoliageType::Foliage_WallDiagonalDownLeft,
			FoliageType::Foliage_WallDiagonalDownRight,
        };

        mainDict[(int)FoliageType::Foliage_WallUp] = FoliageInfo(
			FoliageType::Foliage_WallUp,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 1,
			LevelBiome::None,
			Direction::DirectionUp
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallDown] = FoliageInfo(
			FoliageType::Foliage_WallDown,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 2,
			LevelBiome::None,
			Direction::DirectionDown
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({180});

		mainDict[(int)FoliageType::Foliage_WallLeft] = FoliageInfo(
			FoliageType::Foliage_WallLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 3,
			LevelBiome::None,
			Direction::DirectionLeft
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({270});

		mainDict[(int)FoliageType::Foliage_WallRight] = FoliageInfo(
			FoliageType::Foliage_WallRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 4,
			LevelBiome::None,
			Direction::DirectionRight
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({90});

		mainDict[(int)FoliageType::Foliage_WallUpLeft] = FoliageInfo(
			FoliageType::Foliage_WallUpLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 5,
			LevelBiome::None,
			Direction::DirectionUpLeft
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallUpRight] = FoliageInfo(
			FoliageType::Foliage_WallUpRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 6,
			LevelBiome::None,
			Direction::DirectionUpRight
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({90});

		mainDict[(int)FoliageType::Foliage_WallDownLeft] = FoliageInfo(
			FoliageType::Foliage_WallDownLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 7,
			LevelBiome::None,
			Direction::DirectionDownLeft
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({270});

		mainDict[(int)FoliageType::Foliage_WallDownRight] = FoliageInfo(
			FoliageType::Foliage_WallDownRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 8,
			LevelBiome::None,
			Direction::DirectionDownRight
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({180});

		mainDict[(int)FoliageType::Foliage_WallMiddle] = FoliageInfo(
			FoliageType::Foliage_WallMiddle,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 9,
			LevelBiome::None,
			Direction::DirectionNone
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({0,90,180,270});

		mainDict[(int)FoliageType::Foliage_WallThreeUp] = FoliageInfo(
			FoliageType::Foliage_WallThreeUp,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 10,
			LevelBiome::None,
			Direction::DirectionUp
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallThreeDown] = FoliageInfo(
			FoliageType::Foliage_WallThreeDown,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 11,
			LevelBiome::None,
			Direction::DirectionDown
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({180});

		mainDict[(int)FoliageType::Foliage_WallThreeLeft] = FoliageInfo(
			FoliageType::Foliage_WallThreeLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 12,
			LevelBiome::None,
			Direction::DirectionLeft
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({270});

		mainDict[(int)FoliageType::Foliage_WallThreeRight] = FoliageInfo(
			FoliageType::Foliage_WallThreeRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 13,
			LevelBiome::None,
			Direction::DirectionRight
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({90});


		mainDict[(int)FoliageType::Foliage_WallFrontBackUp] = FoliageInfo(
			FoliageType::Foliage_WallFrontBackUp,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 14,
			LevelBiome::None,
			Direction::DirectionUp
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallFrontBackLeft] = FoliageInfo(
			FoliageType::Foliage_WallFrontBackLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 15,
			LevelBiome::None,
			Direction::DirectionDown
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallAll] = FoliageInfo(
			FoliageType::Foliage_WallAll,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 16,
			LevelBiome::None,
			Direction::DirectionDown
            )
            .SetHeight(foliageHeight);


		mainDict[(int)FoliageType::Foliage_WallCenterInsideUpLeft] = FoliageInfo(
			FoliageType::Foliage_WallCenterInsideUpLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 17,
			LevelBiome::None,
			Direction::DirectionUpLeft
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallCenterInsideUpRight] = FoliageInfo(
			FoliageType::Foliage_WallCenterInsideUpRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 18,
			LevelBiome::None,
			Direction::DirectionUpRight
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallCenterInsideDownLeft] = FoliageInfo(
			FoliageType::Foliage_WallCenterInsideDownLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 19,
			LevelBiome::None,
			Direction::DirectionDownLeft
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallCenterInsideDownRight] = FoliageInfo(
			FoliageType::Foliage_WallCenterInsideDownRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 20,
			LevelBiome::None,
			Direction::DirectionDownRight
            )
            .SetHeight(foliageHeight);


		

		mainDict[(int)FoliageType::Foliage_WallDiagonalUpLeft] = FoliageInfo(
			FoliageType::Foliage_WallDiagonalUpLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 21,
			LevelBiome::None,
			Direction::DirectionUpLeft
            )
            .SetHeight(foliageHeight);

		mainDict[(int)FoliageType::Foliage_WallDiagonalUpRight] = FoliageInfo(
			FoliageType::Foliage_WallDiagonalUpRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 22,
			LevelBiome::None,
			Direction::DirectionUpRight
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({90});

		mainDict[(int)FoliageType::Foliage_WallDiagonalDownLeft] = FoliageInfo(
			FoliageType::Foliage_WallDiagonalDownLeft,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 23,
			LevelBiome::None,
			Direction::DirectionDownLeft
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({270});

		mainDict[(int)FoliageType::Foliage_WallDiagonalDownRight] = FoliageInfo(
			FoliageType::Foliage_WallDiagonalDownRight,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 24,
			LevelBiome::None,
			Direction::DirectionDownRight
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({180});


		mainDict[(int)FoliageType::Foliage_WallMiddleEmpty] = FoliageInfo(
			FoliageType::Foliage_WallMiddleEmpty,
			FoliageHelpers::HIGH_GROUND_NODE_TYPE,
			false,
			false,
			mapIndexStart + 25,
			LevelBiome::None,
			Direction::DirectionNone
            )
            .SetHeight(foliageHeight)
			.SetPossibleRotations({0,90,180,270});
    }
};

