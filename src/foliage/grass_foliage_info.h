#pragma once

#include <vector>
#include <unordered_map>

#include "foliage.h"
#include "biome_foliage_info.h"


class GrassFoliageInfo : public BiomeFoliageInfo {

	public:
    GrassFoliageInfo() {

		Setup(m_allowedTypes, m_walkableAllowedTypes, "GrassRelationsMap.txt");
		startPossibleTypes[static_cast<int>(FoliageType::Foliage_NoFoliage)] =
			possibleTypes[static_cast<int>(FoliageType::Foliage_NoFoliage)];

		neighbourBonus[static_cast<int>(FoliageType::Foliage_NoFoliage)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_NoFoliage, 120),
			std::pair<FoliageType, int>(FoliageType::Foliage_ThrottenGround, 40),
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_TreeFoliage)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, 80),
			std::pair<FoliageType, int>(FoliageType::Foliage_FlowerClusterFoliage, 20),
			// (FoliageType::Foliage_RoundBoulder, 20),
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_FlowerClusterFoliage)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_FlowerClusterFoliage, 20)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_ThrottenGround)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_ThrottenGround, 20),
			std::pair<FoliageType, int>(FoliageType::Foliage_VeryThrottenGround, 10),
		};

		neighbourBonus[static_cast<int>(FoliageType::Foliage_VeryThrottenGround)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_ThrottenGround, 20),
			std::pair<FoliageType, int>(FoliageType::Foliage_VeryThrottenGround, 40),
			std::pair<FoliageType, int>(FoliageType::Foliage_CompletelyThrottenGround, 10),
		};

		neighbourBonus[static_cast<int>(FoliageType::Foliage_CompletelyThrottenGround)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_VeryThrottenGround, 10),
			std::pair<FoliageType, int>(FoliageType::Foliage_CompletelyThrottenGround, 20),
		};

		int treeCliffedgeToWallBonus = 4000;
		neighbourBonus[static_cast<int>(FoliageType::Foliage_TreeCliffedge)] = {
			// (FoliageType::Foliage_TreeCliffedge, 100),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallUp, treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallDown, treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallLeft, treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallRight, treeCliffedgeToWallBonus),

			std::pair<FoliageType, int>(FoliageType::Foliage_WallUpLeft, treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallUpRight, treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallDownLeft, treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(FoliageType::Foliage_WallDownRight, treeCliffedgeToWallBonus),

			// (FoliageType::Foliage_WallCenterInsideUpLeft, treeCliffedgeToWallBonus),
			// (FoliageType::Foliage_WallCenterInsideUpRight, treeCliffedgeToWallBonus),
			// (FoliageType::Foliage_WallCenterInsideDownLeft, treeCliffedgeToWallBonus),
			// (FoliageType::Foliage_WallCenterInsideDownRight, treeCliffedgeToWallBonus),
		};

		// NeighbourBonus[static_cast<int>(FoliageType::Foliage_WallUp] = new (FoliageType, int)[] {
		// 	(FoliageType::Foliage_TreeCliffedge, treeCliffedgeToWallBonus)
		// };
		// NeighbourBonus[static_cast<int>(FoliageType::Foliage_WallDown] = new (FoliageType, int)[] {
		// 	(FoliageType::Foliage_TreeCliffedge, treeCliffedgeToWallBonus)
		// };
		// NeighbourBonus[static_cast<int>(FoliageType::Foliage_WallLeft] = new (FoliageType, int)[] {
		// 	(FoliageType::Foliage_TreeCliffedge, treeCliffedgeToWallBonus)
		// };
		// NeighbourBonus[static_cast<int>(FoliageType::Foliage_WallRight] = new (FoliageType, int)[] {
		// 	(FoliageType::Foliage_TreeCliffedge, treeCliffedgeToWallBonus)
		// };

		int borderTreeToTreeBonus = 40;
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeUp)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeDown)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeLeft)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeRight)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};

		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeUpLeft)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeUpRight)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeDownLeft)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
		neighbourBonus[static_cast<int>(FoliageType::Foliage_BorderTreeDownRight)] = {
			std::pair<FoliageType, int>(FoliageType::Foliage_TreeFoliage, borderTreeToTreeBonus)
		};
    }

	std::unordered_map<FoliageType, int> m_allowedTypes = {
		{FoliageType::Foliage_RockClusterFoliage, 40},
		{FoliageType::Foliage_TreeFoliage, 90},
		{FoliageType::Foliage_TreeSmallFoliage, 10},
		{FoliageType::Foliage_TreeCliffedge, 400},
		{FoliageType::Foliage_FlowerClusterFoliage, 30},
		{FoliageType::Foliage_MonumentCluster, 1},
		{FoliageType::Foliage_RoundBoulder, 1},
		{FoliageType::Foliage_FlatStone, 3},
		// Requires ground +3.
		{FoliageType::Foliage_FlatStoneDesert, 5},
		{FoliageType::Foliage_StoneCircle, 3},
		{FoliageType::Foliage_NoFoliage, 120},

		{FoliageType::Foliage_ThrottenGround, 10},
		{FoliageType::Foliage_VeryThrottenGround, 10},
		{FoliageType::Foliage_CompletelyThrottenGround, 10},

		{FoliageType::Foliage_BorderTreeUp, 30},
		{FoliageType::Foliage_BorderTreeDown, 30},
		{FoliageType::Foliage_BorderTreeLeft, 30},
		{FoliageType::Foliage_BorderTreeRight, 30},

		{FoliageType::Foliage_BorderTreeUpLeft, 4},
		{FoliageType::Foliage_BorderTreeUpRight, 4},
		{FoliageType::Foliage_BorderTreeDownLeft, 4},
		{FoliageType::Foliage_BorderTreeDownRight, 4},

		{FoliageType::Foliage_BorderTreeMiddle, 6},

		// {FoliageType::Foliage_CliffsideDesertUp, 1},
		// {FoliageType::Foliage_CliffsideDesertDown, 1},
		// {FoliageType::Foliage_CliffsideDesertLeft, 1},
		// {FoliageType::Foliage_CliffsideDesertRight, 1},

		// {FoliageType::Foliage_CliffsideDesertUpLeft, 1},
		// {FoliageType::Foliage_CliffsideDesertUpRight, 1},
		// {FoliageType::Foliage_CliffsideDesertDownLeft, 1},
		// {FoliageType::Foliage_CliffsideDesertDownRight, 1},

		// {FoliageType::Foliage_CliffsideDesertMiddle, 10},



		{FoliageType::Foliage_ChasmWallUp, 500},
		{FoliageType::Foliage_ChasmWallDown, 500},
		{FoliageType::Foliage_ChasmWallLeft, 500},
		{FoliageType::Foliage_ChasmWallRight, 500},

		{FoliageType::Foliage_ChasmWallUpLeft, 4},
		{FoliageType::Foliage_ChasmWallUpRight, 4},
		{FoliageType::Foliage_ChasmWallDownLeft, 4},
		{FoliageType::Foliage_ChasmWallDownRight, 4},

		{FoliageType::Foliage_ChasmWallCenterUpLeft, 4},
		{FoliageType::Foliage_ChasmWallCenterUpRight, 4},
		{FoliageType::Foliage_ChasmWallCenterDownLeft, 4},
		{FoliageType::Foliage_ChasmWallCenterDownRight, 4},



		{FoliageType::Foliage_ChasmWallInsideUp, 100},
		{FoliageType::Foliage_ChasmWallInsideDown, 100},
		{FoliageType::Foliage_ChasmWallInsideLeft, 100},
		{FoliageType::Foliage_ChasmWallInsideRight, 100},


		{FoliageType::Foliage_ChasmWallCenterInsideUpLeft, 4},
		{FoliageType::Foliage_ChasmWallCenterInsideUpRight, 4},
		{FoliageType::Foliage_ChasmWallCenterInsideDownLeft, 4},
		{FoliageType::Foliage_ChasmWallCenterInsideDownRight, 4},

		{FoliageType::Foliage_ChasmWallInsideUpLeft, 2},
		{FoliageType::Foliage_ChasmWallInsideUpRight, 2},
		{FoliageType::Foliage_ChasmWallInsideDownLeft, 2},
		{FoliageType::Foliage_ChasmWallInsideDownRight, 2},

		{FoliageType::Foliage_ChasmWallDiagonalUpLeft, 10},
		{FoliageType::Foliage_ChasmWallDiagonalUpRight, 10},
		{FoliageType::Foliage_ChasmWallDiagonalDownLeft, 10},
		{FoliageType::Foliage_ChasmWallDiagonalDownRight, 10},


		// {FoliageType::Foliage_ChasmWallThreeUp, 2},
		// {FoliageType::Foliage_ChasmWallThreeDown, 2},
		// {FoliageType::Foliage_ChasmWallThreeLeft, 2},
		// {FoliageType::Foliage_ChasmWallThreeRight, 2},

		{FoliageType::Foliage_ChasmWallFrontBackUp, 30},
		{FoliageType::Foliage_ChasmWallFrontBackLeft, 30},

		// {FoliageType::Foliage_ChasmWallAll, 1},

		{FoliageType::Foliage_ChasmMiddle, 20},



		{FoliageType::Foliage_WallUp, 20},
		{FoliageType::Foliage_WallDown, 20},
		{FoliageType::Foliage_WallLeft, 20},
		{FoliageType::Foliage_WallRight, 10},

		{FoliageType::Foliage_WallUpLeft, 10},
		{FoliageType::Foliage_WallUpRight, 10},
		{FoliageType::Foliage_WallDownLeft, 10},
		{FoliageType::Foliage_WallDownRight, 10},

		{FoliageType::Foliage_WallCenterInsideUpLeft, 20},
		{FoliageType::Foliage_WallCenterInsideUpRight, 20},
		{FoliageType::Foliage_WallCenterInsideDownLeft, 20},
		{FoliageType::Foliage_WallCenterInsideDownRight, 20},

		{FoliageType::Foliage_WallThreeUp, 2},
		{FoliageType::Foliage_WallThreeDown, 2},
		{FoliageType::Foliage_WallThreeLeft, 2},
		{FoliageType::Foliage_WallThreeRight, 2},

		{FoliageType::Foliage_WallFrontBackUp, 2},
		{FoliageType::Foliage_WallFrontBackLeft, 2},

		{FoliageType::Foliage_WallAll, 4},

		{FoliageType::Foliage_WallMiddle, 200},


		{FoliageType::Foliage_WallDiagonalUpLeft, 10},
		{FoliageType::Foliage_WallDiagonalUpRight, 10},
		{FoliageType::Foliage_WallDiagonalDownLeft, 10},
		{FoliageType::Foliage_WallDiagonalDownRight, 10},



		{FoliageType::Foliage_WallHigherUp, 200},
		{FoliageType::Foliage_WallHigherDown, 200},
		{FoliageType::Foliage_WallHigherLeft, 200},
		{FoliageType::Foliage_WallHigherRight, 200},

		{FoliageType::Foliage_WallHigherUpLeft, 4},
		{FoliageType::Foliage_WallHigherUpRight, 4},
		{FoliageType::Foliage_WallHigherDownLeft, 4},
		{FoliageType::Foliage_WallHigherDownRight, 4},

		{FoliageType::Foliage_WallHigherThreeUp, 4},
		{FoliageType::Foliage_WallHigherThreeDown, 4},
		{FoliageType::Foliage_WallHigherThreeLeft, 4},
		{FoliageType::Foliage_WallHigherThreeRight, 4},

		{FoliageType::Foliage_WallHigherFrontBackUp, 4},
		{FoliageType::Foliage_WallHigherFrontBackLeft, 4},

		// {FoliageType::Foliage_WallHigherAll, 10},

		{FoliageType::Foliage_WallHigherMiddle, 80},
	};

	std::unordered_map<FoliageType, int> m_walkableAllowedTypes = {
		{FoliageType::Foliage_TreeFoliage, 10},
		{FoliageType::Foliage_TreeSmallFoliage, 2},
		{FoliageType::Foliage_FlowerClusterFoliage, 2},
		{FoliageType::Foliage_PlantClusterFoliage, 1},
		// {FoliageType::Foliage_MonumentCluster, 1},
		// {FoliageType::Foliage_RoundBoulder, 1},
		{FoliageType::Foliage_FlatStone, 3},
		{FoliageType::Foliage_StoneCircle, 6},
		{FoliageType::Foliage_NoFoliage, 220},

		{FoliageType::Foliage_ThrottenGround, 10},
		{FoliageType::Foliage_VeryThrottenGround, 10},
		{FoliageType::Foliage_CompletelyThrottenGround, 10},
	};
};
