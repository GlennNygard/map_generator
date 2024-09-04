#pragma once


struct Colour {
	uint8_t r,g,b;
	Colour() {r = 100; g = 0; b = 10;};
	Colour(uint8_t r, uint8_t g, uint8_t b){
		this->r = r; this->g = g; this->b = b;};
	Colour(double r, double g, double b){
		this->r = static_cast<uint8_t>(r*255.0);
		this->g = static_cast<uint8_t>(g*255.0);
		this->b = static_cast<uint8_t>(b*255.0);
	};
} __attribute__((__packed__));


enum class ThrottenAmount {
    None,
    Level01,
    Level02,
    Completely,
};

enum class FoliageFloor {
	Normal,
	High,
	Higher,
	Low,
};

enum class Direction {
    DirectionNone,
    DirectionUp,
    DirectionDown,
    DirectionLeft,
    DirectionRight,
    DirectionUpLeft,
    DirectionUpRight,
    DirectionDownLeft,
    DirectionDownRight,
};

enum class MapSize {
	MapSize_Smaller,
	MapSize_Small,
	MapSize_Medium,
	MapSize_Large,
	MapSize_VeryLarge,
	MapSize_IncrediblyLarge,
	MapSize_Massive,
};

// Must keep order.
enum class LevelBiome {
    None,
    Grass,
    Desert,
    Forrest,
    Snow,

    // Always keep at bottom.
    Debug,
};

enum class FoliageType {
	Foliage_NoSelection, // 0.
	Foliage_NoFoliage, // 1.
	Foliage_Path, // 2.
	Foliage_CantHaveFoliage, // 3.
	Foliage_ThrottenGround, // 4.
	Foliage_VeryThrottenGround, // 5.
	Foliage_CompletelyThrottenGround, // 6.
	Foliage_TreeFoliage, // 7.
	Foliage_TreeWallFoliage,
	Foliage_TreeCliffedge,
	Foliage_TreeSnowFoliage, // 8.
	Foliage_TreeSmallFoliage, // 19.
	Foliage_TreeSmallSnowFoliage, // 10.
	Foliage_RockClusterFoliage, // 11.
	Foliage_RockClusterSnowFoliage, // 12.
	Foliage_RoundBoulder,
	Foliage_FlowerClusterFoliage, // 13.
	Foliage_PlantClusterFoliage, // 14.
	Foliage_MonumentCluster, // 15.


	// UNBUILDABLE.
	Foliage_GroundUnbuildable,
	Foliage_ThrottenGroundUnbuildable,
	Foliage_VeryThrottenGroundUnbuildable,
	Foliage_CompletelyThrottenGroundUnbuildable,


	// CLIFFSIDE.
	Foliage_CliffsideShort, // 17.
	Foliage_CliffsideHigh, // 18.
	Foliage_CliffsideVeryHigh, // 19.
	Foliage_CliffsideMaxHeight, // 20.

	Foliage_Cliffside, // 16.

	Foliage_CliffsideUp,
	Foliage_CliffsideDown,
	Foliage_CliffsideLeft,
	Foliage_CliffsideRight,
	Foliage_CliffsideMiddle,

	Foliage_CliffsideHighUp,
	Foliage_CliffsideHighDown,
	Foliage_CliffsideHighLeft,
	Foliage_CliffsideHighRight,

	Foliage_CliffsideHighUpLeft,
	Foliage_CliffsideHighUpRight,
	Foliage_CliffsideHighDownLeft,
	Foliage_CliffsideHighDownRight,

	Foliage_CliffsideHighMiddle,

	Foliage_CliffsideHighCenterUpLeft,
	Foliage_CliffsideHighCenterUpRight,
	Foliage_CliffsideHighCenterDownLeft,
	Foliage_CliffsideHighCenterDownRight,

	// CLIFFSIDE DESERT.
	Foliage_CliffsideDesertUp,
	Foliage_CliffsideDesertDown,
	Foliage_CliffsideDesertLeft,
	Foliage_CliffsideDesertRight,
	Foliage_CliffsideDesertUpLeft,
	Foliage_CliffsideDesertUpRight,
	Foliage_CliffsideDesertDownLeft,
	Foliage_CliffsideDesertDownRight,

	Foliage_CliffsideDesertThreeUp,
	Foliage_CliffsideDesertThreeDown,
	Foliage_CliffsideDesertThreeLeft,
	Foliage_CliffsideDesertThreeRight,

	Foliage_CliffsideDesertFrontBackUp,
	Foliage_CliffsideDesertFrontBackLeft,

	Foliage_CliffsideDesertAll,

	Foliage_CliffsideDesertMiddle,


	// CHASM SIDE.
	Foliage_ChasmWallUp,
	Foliage_ChasmWallDown,
	Foliage_ChasmWallLeft,
	Foliage_ChasmWallRight,

	Foliage_ChasmWallUpLeft,
	Foliage_ChasmWallUpRight,
	Foliage_ChasmWallDownLeft,
	Foliage_ChasmWallDownRight,

	Foliage_ChasmWallThreeUp,
	Foliage_ChasmWallThreeDown,
	Foliage_ChasmWallThreeLeft,
	Foliage_ChasmWallThreeRight,

	Foliage_ChasmWallFrontBackUp,
	Foliage_ChasmWallFrontBackLeft,

	Foliage_ChasmWallAll,

	Foliage_ChasmMiddle,

	Foliage_ChasmWallCenterUpLeft,
	Foliage_ChasmWallCenterUpRight,
	Foliage_ChasmWallCenterDownLeft,
	Foliage_ChasmWallCenterDownRight,


	Foliage_ChasmWallInsideUp,
	Foliage_ChasmWallInsideDown,
	Foliage_ChasmWallInsideLeft,
	Foliage_ChasmWallInsideRight,

	Foliage_ChasmWallCenterInsideUpLeft,
	Foliage_ChasmWallCenterInsideUpRight,
	Foliage_ChasmWallCenterInsideDownLeft,
	Foliage_ChasmWallCenterInsideDownRight,

	Foliage_ChasmWallInsideUpLeft,
	Foliage_ChasmWallInsideUpRight,
	Foliage_ChasmWallInsideDownLeft,
	Foliage_ChasmWallInsideDownRight,

	Foliage_ChasmWallDiagonalUpLeft,
	Foliage_ChasmWallDiagonalUpRight,
	Foliage_ChasmWallDiagonalDownLeft,
	Foliage_ChasmWallDiagonalDownRight,


	// WALLS.

	Foliage_WallUp,
	Foliage_WallDown,
	Foliage_WallLeft,
	Foliage_WallRight,

	Foliage_WallUpLeft,
	Foliage_WallUpRight,
	Foliage_WallDownLeft,
	Foliage_WallDownRight,

	Foliage_WallThreeUp,
	Foliage_WallThreeDown,
	Foliage_WallThreeLeft,
	Foliage_WallThreeRight,

	Foliage_WallFrontBackUp,
	Foliage_WallFrontBackLeft,

	Foliage_WallAll,

	Foliage_WallMiddle,
	Foliage_WallMiddleEmpty,


	Foliage_WallCenterInsideUpLeft,
	Foliage_WallCenterInsideUpRight,
	Foliage_WallCenterInsideDownLeft,
	Foliage_WallCenterInsideDownRight,


	Foliage_WallDiagonalUpLeft,
	Foliage_WallDiagonalUpRight,
	Foliage_WallDiagonalDownLeft,
	Foliage_WallDiagonalDownRight,


	Foliage_WallHigherUp,
	Foliage_WallHigherDown,
	Foliage_WallHigherLeft,
	Foliage_WallHigherRight,

	Foliage_WallHigherUpLeft,
	Foliage_WallHigherUpRight,
	Foliage_WallHigherDownLeft,
	Foliage_WallHigherDownRight,

	Foliage_WallHigherThreeUp,
	Foliage_WallHigherThreeDown,
	Foliage_WallHigherThreeLeft,
	Foliage_WallHigherThreeRight,

	Foliage_WallHigherFrontBackUp,
	Foliage_WallHigherFrontBackLeft,

	Foliage_WallHigherAll,

	Foliage_WallHigherMiddle,



	// BORDER TREES.
	Foliage_BorderTreeUp,
	Foliage_BorderTreeDown,
	Foliage_BorderTreeLeft,
	Foliage_BorderTreeRight,

	Foliage_BorderTreeUpLeft,
	Foliage_BorderTreeUpRight,
	Foliage_BorderTreeDownLeft,
	Foliage_BorderTreeDownRight,

	Foliage_BorderTreeMiddle,

	Foliage_TreeEvergreenFoliage, // 21.
	Foliage_TreeEvergreenHighFoliage, // 21.
	Foliage_TreeSmallEvergreenFoliage, // 22.
	Foliage_TreeSnowEvergreenFoliage, // 23.
	Foliage_HillSmall, // 24
	Foliage_HillSmallSnow, // 25
	Foliage_HillSmallDesert, // 26
	Foliage_HillSmall10, // 27
	Foliage_HillSmall20, // 28
	Foliage_HillSmall30, // 29
	Foliage_HillSmall40, // 30
	Foliage_HillSmall50, // 31
	Foliage_PillarSmallGrass, // 32
	Foliage_PillarSmallDesert, // 33
	Foliage_PillarSmallSnow, // 34
	Foliage_PillarSmall10, // 35
	Foliage_PillarSmall20, // 36
	Foliage_PillarSmall30, // 37
	Foliage_FlatStone, // 38
	Foliage_FlatStoneGrass, // 39
	Foliage_FlatStoneDesert, // 40
	Foliage_FlatStone10, // 41
	Foliage_FlatStone20, // 42
	Foliage_FlatStone30, // 43
	Foliage_FlatStone40, // 44

	Foliage_StoneCircle,

	Foliage_MAX = Foliage_StoneCircle + 1,
};

class FoliageHelpers {

    public:

	static const int FLOOR_NODE_TYPE = 0;
	static const int HIGH_GROUND_NODE_TYPE = 1;
	static const int LOW_GROUND_NODE_TYPE = 2;
	static const int BORDER_NODE_TYPE = 3;
    enum class FoliageFloor {
        Normal,
        High,
        Higher,
        Low,
    };
};

