#pragma once

#include <vector>
#include <unordered_map>

#include "foliage.h"
#include "biome_info.h"


class GrassBiomeInfo : public BiomeInfo<GrassBiomeInfo> {

	public:

	virtual void VFunc() {};
    GrassBiomeInfo(FoliageDefinitions& foliageDefinitions) : BiomeInfo<GrassBiomeInfo>(foliageDefinitions) {

		const size_t foliageCount = foliageDefinitions.GetFoliageCount();
		auto& fd = foliageDefinitions;

		std::unordered_map<FoliageType, int> allowedTypes = {
			{fd.ToFI("RockClusterFoliage"), 40},
			{fd.ToFI("TreeFoliage"), 90},
			{fd.ToFI("TreeSmallFoliage"), 10},
			{fd.ToFI("TreeCliffedge"), 400},
			{fd.ToFI("FlowerClusterFoliage"), 30},
			{fd.ToFI("MonumentCluster"), 1},
			{fd.ToFI("RoundBoulder"), 1},
			{fd.ToFI("FlatStone"), 3},
			// Requires ground +3.
			{fd.ToFI("FlatStoneDesert"), 5},
			{fd.ToFI("StoneCircle"), 3},
			{fd.ToFI("NoFoliage"), 120},

			{fd.ToFI("ThrottenGround"), 10},
			{fd.ToFI("VeryThrottenGround"), 10},
			{fd.ToFI("CompletelyThrottenGround"), 10},

			{fd.ToFI("BorderTreeUp"), 30},
			{fd.ToFI("BorderTreeDown"), 30},
			{fd.ToFI("BorderTreeLeft"), 30},
			{fd.ToFI("BorderTreeRight"), 30},

			{fd.ToFI("BorderTreeUpLeft"), 4},
			{fd.ToFI("BorderTreeUpRight"), 4},
			{fd.ToFI("BorderTreeDownLeft"), 4},
			{fd.ToFI("BorderTreeDownRight"), 4},

			{fd.ToFI("BorderTreeMiddle"), 6},

			{fd.ToFI("ChasmWallUp"), 500},
			{fd.ToFI("ChasmWallDown"), 500},
			{fd.ToFI("ChasmWallLeft"), 500},
			{fd.ToFI("ChasmWallRight"), 500},

			{fd.ToFI("ChasmWallUpLeft"), 4},
			{fd.ToFI("ChasmWallUpRight"), 4},
			{fd.ToFI("ChasmWallDownLeft"), 4},
			{fd.ToFI("ChasmWallDownRight"), 4},

			{fd.ToFI("ChasmWallCenterUpLeft"), 4},
			{fd.ToFI("ChasmWallCenterUpRight"), 4},
			{fd.ToFI("ChasmWallCenterDownLeft"), 4},
			{fd.ToFI("ChasmWallCenterDownRight"), 4},



			{fd.ToFI("ChasmWallInsideUp"), 100},
			{fd.ToFI("ChasmWallInsideDown"), 100},
			{fd.ToFI("ChasmWallInsideLeft"), 100},
			{fd.ToFI("ChasmWallInsideRight"), 100},


			{fd.ToFI("ChasmWallCenterInsideUpLeft"), 4},
			{fd.ToFI("ChasmWallCenterInsideUpRight"), 4},
			{fd.ToFI("ChasmWallCenterInsideDownLeft"), 4},
			{fd.ToFI("ChasmWallCenterInsideDownRight"), 4},

			{fd.ToFI("ChasmWallInsideUpLeft"), 2},
			{fd.ToFI("ChasmWallInsideUpRight"), 2},
			{fd.ToFI("ChasmWallInsideDownLeft"), 2},
			{fd.ToFI("ChasmWallInsideDownRight"), 2},

			{fd.ToFI("ChasmWallDiagonalUpLeft"), 10},
			{fd.ToFI("ChasmWallDiagonalUpRight"), 10},
			{fd.ToFI("ChasmWallDiagonalDownLeft"), 10},
			{fd.ToFI("ChasmWallDiagonalDownRight"), 10},

			{fd.ToFI("ChasmWallFrontBackUp"), 30},
			{fd.ToFI("ChasmWallFrontBackLeft"), 30},

			{fd.ToFI("ChasmMiddle"), 20},



			{fd.ToFI("WallUp"), 20},
			{fd.ToFI("WallDown"), 20},
			{fd.ToFI("WallLeft"), 20},
			{fd.ToFI("WallRight"), 10},

			{fd.ToFI("WallUpLeft"), 10},
			{fd.ToFI("WallUpRight"), 10},
			{fd.ToFI("WallDownLeft"), 10},
			{fd.ToFI("WallDownRight"), 10},

			{fd.ToFI("WallCenterInsideUpLeft"), 20},
			{fd.ToFI("WallCenterInsideUpRight"), 20},
			{fd.ToFI("WallCenterInsideDownLeft"), 20},
			{fd.ToFI("WallCenterInsideDownRight"), 20},

			{fd.ToFI("WallThreeUp"), 2},
			{fd.ToFI("WallThreeDown"), 2},
			{fd.ToFI("WallThreeLeft"), 2},
			{fd.ToFI("WallThreeRight"), 2},

			{fd.ToFI("WallFrontBackUp"), 2},
			{fd.ToFI("WallFrontBackLeft"), 2},

			{fd.ToFI("WallAll"), 4},

			{fd.ToFI("WallMiddle"), 200},


			{fd.ToFI("WallDiagonalUpLeft"), 10},
			{fd.ToFI("WallDiagonalUpRight"), 10},
			{fd.ToFI("WallDiagonalDownLeft"), 10},
			{fd.ToFI("WallDiagonalDownRight"), 10},



			{fd.ToFI("WallHigherUp"), 200},
			{fd.ToFI("WallHigherDown"), 200},
			{fd.ToFI("WallHigherLeft"), 200},
			{fd.ToFI("WallHigherRight"), 200},

			{fd.ToFI("WallHigherUpLeft"), 4},
			{fd.ToFI("WallHigherUpRight"), 4},
			{fd.ToFI("WallHigherDownLeft"), 4},
			{fd.ToFI("WallHigherDownRight"), 4},

			{fd.ToFI("WallHigherThreeUp"), 4},
			{fd.ToFI("WallHigherThreeDown"), 4},
			{fd.ToFI("WallHigherThreeLeft"), 4},
			{fd.ToFI("WallHigherThreeRight"), 4},

			{fd.ToFI("WallHigherFrontBackUp"), 4},
			{fd.ToFI("WallHigherFrontBackLeft"), 4},

			// {WallHigherAll, 10},

			{fd.ToFI("WallHigherMiddle"), 80},
		};

		std::unordered_map<FoliageType, int> walkableAllowedTypes = {
			{fd.ToFI("TreeFoliage"), 10},
			{fd.ToFI("TreeSmallFoliage"), 2},
			{fd.ToFI("FlowerClusterFoliage"), 2},
			{fd.ToFI("PlantClusterFoliage"), 1},
			// {MonumentCluster, 1},
			// {RoundBoulder, 1},
			{fd.ToFI("FlatStone"), 3},
			{fd.ToFI("StoneCircle"), 6},
			{fd.ToFI("NoFoliage"), 220},

			{fd.ToFI("ThrottenGround"), 10},
			{fd.ToFI("VeryThrottenGround"), 10},
			{fd.ToFI("CompletelyThrottenGround"), 10},
		};

		auto relationsPath = DiskManager::GetRelationalMapPath("GrassRelationsMap.txt");

		Setup(allowedTypes, walkableAllowedTypes, foliageDefinitions, relationsPath);
		m_startFoliagePriority[(fd.ToFI("NoFoliage"))] =
			m_foliagePriority.at(fd.ToFI("NoFoliage"));
		

		auto tempList = std::vector<std::vector<std::pair<FoliageType, int>>>(foliageCount);


		tempList[(fd.ToFI("NoFoliage"))] = {
			std::pair<FoliageType, int>(fd.ToFI("NoFoliage"), 120),
			std::pair<FoliageType, int>(fd.ToFI("ThrottenGround"), 40),
		};
		tempList[(fd.ToFI("TreeFoliage"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), 80),
			std::pair<FoliageType, int>(fd.ToFI("FlowerClusterFoliage"), 20),
			// (RoundBoulder, 20),
		};
		tempList[(fd.ToFI("FlowerClusterFoliage"))] = {
			std::pair<FoliageType, int>(fd.ToFI("FlowerClusterFoliage"), 20)
		};
		tempList[(fd.ToFI("ThrottenGround"))] = {
			std::pair<FoliageType, int>(fd.ToFI("ThrottenGround"), 20),
			std::pair<FoliageType, int>(fd.ToFI("VeryThrottenGround"), 10),
		};

		tempList[(fd.ToFI("VeryThrottenGround"))] = {
			std::pair<FoliageType, int>(fd.ToFI("ThrottenGround"), 20),
			std::pair<FoliageType, int>(fd.ToFI("VeryThrottenGround"), 40),
			std::pair<FoliageType, int>(fd.ToFI("CompletelyThrottenGround"), 10),
		};

		tempList[(fd.ToFI("CompletelyThrottenGround"))] = {
			std::pair<FoliageType, int>(fd.ToFI("VeryThrottenGround"), 10),
			std::pair<FoliageType, int>(fd.ToFI("CompletelyThrottenGround"), 20),
		};

		int treeCliffedgeToWallBonus = 4000;
		tempList[(fd.ToFI("TreeCliffedge"))] = {
			// (TreeCliffedge, 100),
			std::pair<FoliageType, int>(fd.ToFI("WallUp"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.ToFI("WallDown"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.ToFI("WallLeft"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.ToFI("WallRight"), treeCliffedgeToWallBonus),

			std::pair<FoliageType, int>(fd.ToFI("WallUpLeft"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.ToFI("WallUpRight"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.ToFI("WallDownLeft"), treeCliffedgeToWallBonus),
			std::pair<FoliageType, int>(fd.ToFI("WallDownRight"), treeCliffedgeToWallBonus),
		};

		int borderTreeToTreeBonus = 40;
		tempList[(fd.ToFI("BorderTreeUp"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.ToFI("BorderTreeDown"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.ToFI("BorderTreeLeft"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.ToFI("BorderTreeRight"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};

		tempList[(fd.ToFI("BorderTreeUpLeft"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.ToFI("BorderTreeUpRight"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.ToFI("BorderTreeDownLeft"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};
		tempList[(fd.ToFI("BorderTreeDownRight"))] = {
			std::pair<FoliageType, int>(fd.ToFI("TreeFoliage"), borderTreeToTreeBonus)
		};

		neighbourBonusList = DefaultFoliageSquashedList(tempList);
    }
};
