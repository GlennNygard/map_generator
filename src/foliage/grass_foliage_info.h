#pragma once

#include <vector>
#include <unordered_map>

#include "foliage.h"
#include "biome_foliage_info.h"
#include "foliage_definitions.h"


class GrassFoliageInfo : public BiomeFoliageInfo {

	public:
    GrassFoliageInfo() {

		const size_t foliageCount = foliagedef::get_foliage_definitions().get_foliage_count();

		m_allowedTypes = {
			{toFI("RockClusterFoliage"), 40},
			{toFI("TreeFoliage"), 90},
			{toFI("TreeSmallFoliage"), 10},
			{toFI("TreeCliffedge"), 400},
			{toFI("FlowerClusterFoliage"), 30},
			{toFI("MonumentCluster"), 1},
			{toFI("RoundBoulder"), 1},
			{toFI("FlatStone"), 3},
			// Requires ground +3.
			{toFI("FlatStoneDesert"), 5},
			{toFI("StoneCircle"), 3},
			{toFI("NoFoliage"), 120},

			{toFI("ThrottenGround"), 10},
			{toFI("VeryThrottenGround"), 10},
			{toFI("CompletelyThrottenGround"), 10},

			{toFI("BorderTreeUp"), 30},
			{toFI("BorderTreeDown"), 30},
			{toFI("BorderTreeLeft"), 30},
			{toFI("BorderTreeRight"), 30},

			{toFI("BorderTreeUpLeft"), 4},
			{toFI("BorderTreeUpRight"), 4},
			{toFI("BorderTreeDownLeft"), 4},
			{toFI("BorderTreeDownRight"), 4},

			{toFI("BorderTreeMiddle"), 6},

			{toFI("ChasmWallUp"), 500},
			{toFI("ChasmWallDown"), 500},
			{toFI("ChasmWallLeft"), 500},
			{toFI("ChasmWallRight"), 500},

			{toFI("ChasmWallUpLeft"), 4},
			{toFI("ChasmWallUpRight"), 4},
			{toFI("ChasmWallDownLeft"), 4},
			{toFI("ChasmWallDownRight"), 4},

			{toFI("ChasmWallCenterUpLeft"), 4},
			{toFI("ChasmWallCenterUpRight"), 4},
			{toFI("ChasmWallCenterDownLeft"), 4},
			{toFI("ChasmWallCenterDownRight"), 4},



			{toFI("ChasmWallInsideUp"), 100},
			{toFI("ChasmWallInsideDown"), 100},
			{toFI("ChasmWallInsideLeft"), 100},
			{toFI("ChasmWallInsideRight"), 100},


			{toFI("ChasmWallCenterInsideUpLeft"), 4},
			{toFI("ChasmWallCenterInsideUpRight"), 4},
			{toFI("ChasmWallCenterInsideDownLeft"), 4},
			{toFI("ChasmWallCenterInsideDownRight"), 4},

			{toFI("ChasmWallInsideUpLeft"), 2},
			{toFI("ChasmWallInsideUpRight"), 2},
			{toFI("ChasmWallInsideDownLeft"), 2},
			{toFI("ChasmWallInsideDownRight"), 2},

			{toFI("ChasmWallDiagonalUpLeft"), 10},
			{toFI("ChasmWallDiagonalUpRight"), 10},
			{toFI("ChasmWallDiagonalDownLeft"), 10},
			{toFI("ChasmWallDiagonalDownRight"), 10},

			{toFI("ChasmWallFrontBackUp"), 30},
			{toFI("ChasmWallFrontBackLeft"), 30},

			{toFI("ChasmMiddle"), 20},



			{toFI("WallUp"), 20},
			{toFI("WallDown"), 20},
			{toFI("WallLeft"), 20},
			{toFI("WallRight"), 10},

			{toFI("WallUpLeft"), 10},
			{toFI("WallUpRight"), 10},
			{toFI("WallDownLeft"), 10},
			{toFI("WallDownRight"), 10},

			{toFI("WallCenterInsideUpLeft"), 20},
			{toFI("WallCenterInsideUpRight"), 20},
			{toFI("WallCenterInsideDownLeft"), 20},
			{toFI("WallCenterInsideDownRight"), 20},

			{toFI("WallThreeUp"), 2},
			{toFI("WallThreeDown"), 2},
			{toFI("WallThreeLeft"), 2},
			{toFI("WallThreeRight"), 2},

			{toFI("WallFrontBackUp"), 2},
			{toFI("WallFrontBackLeft"), 2},

			{toFI("WallAll"), 4},

			{toFI("WallMiddle"), 200},


			{toFI("WallDiagonalUpLeft"), 10},
			{toFI("WallDiagonalUpRight"), 10},
			{toFI("WallDiagonalDownLeft"), 10},
			{toFI("WallDiagonalDownRight"), 10},



			{toFI("WallHigherUp"), 200},
			{toFI("WallHigherDown"), 200},
			{toFI("WallHigherLeft"), 200},
			{toFI("WallHigherRight"), 200},

			{toFI("WallHigherUpLeft"), 4},
			{toFI("WallHigherUpRight"), 4},
			{toFI("WallHigherDownLeft"), 4},
			{toFI("WallHigherDownRight"), 4},

			{toFI("WallHigherThreeUp"), 4},
			{toFI("WallHigherThreeDown"), 4},
			{toFI("WallHigherThreeLeft"), 4},
			{toFI("WallHigherThreeRight"), 4},

			{toFI("WallHigherFrontBackUp"), 4},
			{toFI("WallHigherFrontBackLeft"), 4},

			// {WallHigherAll, 10},

			{toFI("WallHigherMiddle"), 80},
		};

		m_walkableAllowedTypes = {
			{toFI("TreeFoliage"), 10},
			{toFI("TreeSmallFoliage"), 2},
			{toFI("FlowerClusterFoliage"), 2},
			{toFI("PlantClusterFoliage"), 1},
			// {MonumentCluster, 1},
			// {RoundBoulder, 1},
			{toFI("FlatStone"), 3},
			{toFI("StoneCircle"), 6},
			{toFI("NoFoliage"), 220},

			{toFI("ThrottenGround"), 10},
			{toFI("VeryThrottenGround"), 10},
			{toFI("CompletelyThrottenGround"), 10},
		};

		setup(m_allowedTypes, m_walkableAllowedTypes, "GrassRelationsMap.txt");
		startPossibleTypes[(toFI("NoFoliage"))] =
			possibleTypes[(toFI("NoFoliage"))];

		neighbourBonusList[(toFI("NoFoliage"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("NoFoliage"), 120),
			std::pair<int, int>(toFI("ThrottenGround"), 40),
		});
		neighbourBonusList[(toFI("TreeFoliage"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), 80),
			std::pair<int, int>(toFI("FlowerClusterFoliage"), 20),
			// (RoundBoulder, 20),
		});
		neighbourBonusList[(toFI("FlowerClusterFoliage"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("FlowerClusterFoliage"), 20)
		});
		neighbourBonusList[(toFI("ThrottenGround"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("ThrottenGround"), 20),
			std::pair<int, int>(toFI("VeryThrottenGround"), 10),
		});

		neighbourBonusList[(toFI("VeryThrottenGround"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("ThrottenGround"), 20),
			std::pair<int, int>(toFI("VeryThrottenGround"), 40),
			std::pair<int, int>(toFI("CompletelyThrottenGround"), 10),
		});

		neighbourBonusList[(toFI("CompletelyThrottenGround"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("VeryThrottenGround"), 10),
			std::pair<int, int>(toFI("CompletelyThrottenGround"), 20),
		});

		int treeCliffedgeToWallBonus = 4000;
		neighbourBonusList[(toFI("TreeCliffedge"))] = FoliageNeighbourBonus({
			// (TreeCliffedge, 100),
			std::pair<int, int>(toFI("WallUp"), treeCliffedgeToWallBonus),
			std::pair<int, int>(toFI("WallDown"), treeCliffedgeToWallBonus),
			std::pair<int, int>(toFI("WallLeft"), treeCliffedgeToWallBonus),
			std::pair<int, int>(toFI("WallRight"), treeCliffedgeToWallBonus),

			std::pair<int, int>(toFI("WallUpLeft"), treeCliffedgeToWallBonus),
			std::pair<int, int>(toFI("WallUpRight"), treeCliffedgeToWallBonus),
			std::pair<int, int>(toFI("WallDownLeft"), treeCliffedgeToWallBonus),
			std::pair<int, int>(toFI("WallDownRight"), treeCliffedgeToWallBonus),
		});

		int borderTreeToTreeBonus = 40;
		neighbourBonusList[(toFI("BorderTreeUp"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
		neighbourBonusList[(toFI("BorderTreeDown"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
		neighbourBonusList[(toFI("BorderTreeLeft"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
		neighbourBonusList[(toFI("BorderTreeRight"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});

		neighbourBonusList[(toFI("BorderTreeUpLeft"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
		neighbourBonusList[(toFI("BorderTreeUpRight"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
		neighbourBonusList[(toFI("BorderTreeDownLeft"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
		neighbourBonusList[(toFI("BorderTreeDownRight"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("TreeFoliage"), borderTreeToTreeBonus)
		});
    }

	std::unordered_map<int, int> m_allowedTypes;

	std::unordered_map<int, int> m_walkableAllowedTypes;
};
