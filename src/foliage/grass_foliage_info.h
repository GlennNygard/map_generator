#pragma once

#include <vector>
#include <unordered_map>

#include "foliage.h"
#include "biome_foliage_info.h"
#include "foliage_definitions.h"


class GrassFoliageInfo : public BiomeFoliageInfo {

	public:
    GrassFoliageInfo(FoliageDefinitions& foliageDefinitions) : BiomeFoliageInfo(foliageDefinitions) {

		const size_t foliageCount = foliageDefinitions.get_foliage_count();

		auto& fd = foliageDefinitions;

		m_allowedTypes = {
			{fd.toFI("RockClusterFoliage"), 40},
			{fd.toFI("TreeFoliage"), 90},
			{fd.toFI("TreeSmallFoliage"), 10},
			{fd.toFI("TreeCliffedge"), 400},
			{fd.toFI("FlowerClusterFoliage"), 30},
			{fd.toFI("MonumentCluster"), 1},
			{fd.toFI("RoundBoulder"), 1},
			{fd.toFI("FlatStone"), 3},
			// Requires ground +3.
			{fd.toFI("FlatStoneDesert"), 5},
			{fd.toFI("StoneCircle"), 3},
			{fd.toFI("NoFoliage"), 120},

			{fd.toFI("ThrottenGround"), 10},
			{fd.toFI("VeryThrottenGround"), 10},
			{fd.toFI("CompletelyThrottenGround"), 10},

			{fd.toFI("BorderTreeUp"), 30},
			{fd.toFI("BorderTreeDown"), 30},
			{fd.toFI("BorderTreeLeft"), 30},
			{fd.toFI("BorderTreeRight"), 30},

			{fd.toFI("BorderTreeUpLeft"), 4},
			{fd.toFI("BorderTreeUpRight"), 4},
			{fd.toFI("BorderTreeDownLeft"), 4},
			{fd.toFI("BorderTreeDownRight"), 4},

			{fd.toFI("BorderTreeMiddle"), 6},

			{fd.toFI("ChasmWallUp"), 500},
			{fd.toFI("ChasmWallDown"), 500},
			{fd.toFI("ChasmWallLeft"), 500},
			{fd.toFI("ChasmWallRight"), 500},

			{fd.toFI("ChasmWallUpLeft"), 4},
			{fd.toFI("ChasmWallUpRight"), 4},
			{fd.toFI("ChasmWallDownLeft"), 4},
			{fd.toFI("ChasmWallDownRight"), 4},

			{fd.toFI("ChasmWallCenterUpLeft"), 4},
			{fd.toFI("ChasmWallCenterUpRight"), 4},
			{fd.toFI("ChasmWallCenterDownLeft"), 4},
			{fd.toFI("ChasmWallCenterDownRight"), 4},



			{fd.toFI("ChasmWallInsideUp"), 100},
			{fd.toFI("ChasmWallInsideDown"), 100},
			{fd.toFI("ChasmWallInsideLeft"), 100},
			{fd.toFI("ChasmWallInsideRight"), 100},


			{fd.toFI("ChasmWallCenterInsideUpLeft"), 4},
			{fd.toFI("ChasmWallCenterInsideUpRight"), 4},
			{fd.toFI("ChasmWallCenterInsideDownLeft"), 4},
			{fd.toFI("ChasmWallCenterInsideDownRight"), 4},

			{fd.toFI("ChasmWallInsideUpLeft"), 2},
			{fd.toFI("ChasmWallInsideUpRight"), 2},
			{fd.toFI("ChasmWallInsideDownLeft"), 2},
			{fd.toFI("ChasmWallInsideDownRight"), 2},

			{fd.toFI("ChasmWallDiagonalUpLeft"), 10},
			{fd.toFI("ChasmWallDiagonalUpRight"), 10},
			{fd.toFI("ChasmWallDiagonalDownLeft"), 10},
			{fd.toFI("ChasmWallDiagonalDownRight"), 10},

			{fd.toFI("ChasmWallFrontBackUp"), 30},
			{fd.toFI("ChasmWallFrontBackLeft"), 30},

			{fd.toFI("ChasmMiddle"), 20},



			{fd.toFI("WallUp"), 20},
			{fd.toFI("WallDown"), 20},
			{fd.toFI("WallLeft"), 20},
			{fd.toFI("WallRight"), 10},

			{fd.toFI("WallUpLeft"), 10},
			{fd.toFI("WallUpRight"), 10},
			{fd.toFI("WallDownLeft"), 10},
			{fd.toFI("WallDownRight"), 10},

			{fd.toFI("WallCenterInsideUpLeft"), 20},
			{fd.toFI("WallCenterInsideUpRight"), 20},
			{fd.toFI("WallCenterInsideDownLeft"), 20},
			{fd.toFI("WallCenterInsideDownRight"), 20},

			{fd.toFI("WallThreeUp"), 2},
			{fd.toFI("WallThreeDown"), 2},
			{fd.toFI("WallThreeLeft"), 2},
			{fd.toFI("WallThreeRight"), 2},

			{fd.toFI("WallFrontBackUp"), 2},
			{fd.toFI("WallFrontBackLeft"), 2},

			{fd.toFI("WallAll"), 4},

			{fd.toFI("WallMiddle"), 200},


			{fd.toFI("WallDiagonalUpLeft"), 10},
			{fd.toFI("WallDiagonalUpRight"), 10},
			{fd.toFI("WallDiagonalDownLeft"), 10},
			{fd.toFI("WallDiagonalDownRight"), 10},



			{fd.toFI("WallHigherUp"), 200},
			{fd.toFI("WallHigherDown"), 200},
			{fd.toFI("WallHigherLeft"), 200},
			{fd.toFI("WallHigherRight"), 200},

			{fd.toFI("WallHigherUpLeft"), 4},
			{fd.toFI("WallHigherUpRight"), 4},
			{fd.toFI("WallHigherDownLeft"), 4},
			{fd.toFI("WallHigherDownRight"), 4},

			{fd.toFI("WallHigherThreeUp"), 4},
			{fd.toFI("WallHigherThreeDown"), 4},
			{fd.toFI("WallHigherThreeLeft"), 4},
			{fd.toFI("WallHigherThreeRight"), 4},

			{fd.toFI("WallHigherFrontBackUp"), 4},
			{fd.toFI("WallHigherFrontBackLeft"), 4},

			// {WallHigherAll, 10},

			{fd.toFI("WallHigherMiddle"), 80},
		};

		m_walkableAllowedTypes = {
			{fd.toFI("TreeFoliage"), 10},
			{fd.toFI("TreeSmallFoliage"), 2},
			{fd.toFI("FlowerClusterFoliage"), 2},
			{fd.toFI("PlantClusterFoliage"), 1},
			// {MonumentCluster, 1},
			// {RoundBoulder, 1},
			{fd.toFI("FlatStone"), 3},
			{fd.toFI("StoneCircle"), 6},
			{fd.toFI("NoFoliage"), 220},

			{fd.toFI("ThrottenGround"), 10},
			{fd.toFI("VeryThrottenGround"), 10},
			{fd.toFI("CompletelyThrottenGround"), 10},
		};

		auto relationsPath = DiskManager::get_relational_map_path("GrassRelationsMap.txt");

		setup(m_allowedTypes, m_walkableAllowedTypes, foliageDefinitions, relationsPath);
		startFoliagePriority[(fd.toFI("NoFoliage"))] =
			foliagePriority[(fd.toFI("NoFoliage"))];
		

		auto tempList = std::vector<std::vector<std::pair<FoliageType, int>>>(foliageCount);


		tempList[(fd.toFI("NoFoliage"))] = {
			std::pair<FoliageType, int>(fd.toFI("NoFoliage"), 120),
			std::pair<FoliageType, int>(fd.toFI("ThrottenGround"), 40),
		};
		tempList[(fd.toFI("TreeFoliage"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), 80),
			std::pair<FoliageType, int>(fd.toFI("FlowerClusterFoliage"), 20),
			// (RoundBoulder, 20),
		};
		tempList[(fd.toFI("FlowerClusterFoliage"))] = {
			std::pair<FoliageType, int>(fd.toFI("FlowerClusterFoliage"), 20)
		};
		tempList[(fd.toFI("ThrottenGround"))] = {
			std::pair<FoliageType, int>(fd.toFI("ThrottenGround"), 20),
			std::pair<FoliageType, int>(fd.toFI("VeryThrottenGround"), 10),
		};

		tempList[(fd.toFI("VeryThrottenGround"))] = {
			std::pair<FoliageType, int>(fd.toFI("ThrottenGround"), 20),
			std::pair<FoliageType, int>(fd.toFI("VeryThrottenGround"), 40),
			std::pair<FoliageType, int>(fd.toFI("CompletelyThrottenGround"), 10),
		};

		tempList[(fd.toFI("CompletelyThrottenGround"))] = {
			std::pair<FoliageType, int>(fd.toFI("VeryThrottenGround"), 10),
			std::pair<FoliageType, int>(fd.toFI("CompletelyThrottenGround"), 20),
		};

		int treeCliffedgeToWallBonus = 4000;
		tempList[(fd.toFI("TreeCliffedge"))] = {
			// (TreeCliffedge, 100),
			std::pair<FoliageType, int>(fd.toFI("WallUp"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.toFI("WallDown"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.toFI("WallLeft"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.toFI("WallRight"), treeCliffedgeToWallBonus),

			std::pair<FoliageType, int>(fd.toFI("WallUpLeft"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.toFI("WallUpRight"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.toFI("WallDownLeft"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.toFI("WallDownRight"), treeCliffedgeToWallBonus),
		};

		int borderTreeToTreeBonus = 40;
		tempList[(fd.toFI("BorderTreeUp"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.toFI("BorderTreeDown"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.toFI("BorderTreeLeft"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.toFI("BorderTreeRight"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};

		tempList[(fd.toFI("BorderTreeUpLeft"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.toFI("BorderTreeUpRight"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.toFI("BorderTreeDownLeft"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.toFI("BorderTreeDownRight"))] = {
			std::pair<FoliageType, int>(fd.toFI("TreeFoliage"), borderTreeToTreeBonus)
		};

		neighbourBonusList = FoliageSquashedList(tempList);
    }

private:

	std::unordered_map<FoliageType, int> m_allowedTypes;
	std::unordered_map<FoliageType, int> m_walkableAllowedTypes;
};
