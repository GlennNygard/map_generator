#pragma once

#include <vector>
#include <iostream>
#include <format>
#include <string>

#include "../foliage/foliage.h"
#include "../foliage_info.h"
#include "../map_definitions.h"


class FoliageCommonSetup {

    public:

    static const void Setup(
			std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> &mainDict,
			int mapIndexStart) {

		std::vector<FoliageInfo> foliageList = {};

        const std::vector<FoliageType> cliffsideDesertDirections = {
            FoliageType::Foliage_CliffsideDesertUp,
            FoliageType::Foliage_CliffsideDesertDown,
            FoliageType::Foliage_CliffsideDesertLeft,
            FoliageType::Foliage_CliffsideDesertRight,
            FoliageType::Foliage_CliffsideDesertUpLeft,
            FoliageType::Foliage_CliffsideDesertUpRight,
            FoliageType::Foliage_CliffsideDesertDownLeft,
            FoliageType::Foliage_CliffsideDesertDownRight,
            FoliageType::Foliage_CliffsideDesertMiddle,

            FoliageType::Foliage_CliffsideDesertThreeUp,
            FoliageType::Foliage_CliffsideDesertThreeDown,
            FoliageType::Foliage_CliffsideDesertThreeLeft,
            FoliageType::Foliage_CliffsideDesertThreeRight,

            FoliageType::Foliage_CliffsideDesertFrontBackUp,
            FoliageType::Foliage_CliffsideDesertFrontBackLeft,

            FoliageType::Foliage_CliffsideDesertAll,
        };

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_NoSelection)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(0)
                .SetNodeBiome(LevelBiome::None)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_NoFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(1)
                .SetNodeBiome(LevelBiome::None)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CantHaveFoliage)
                .SetNodeType(FoliageHelpers::HIGH_GROUND_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(3)
                .SetNodeBiome(LevelBiome::None)
                .SetColour(Colour(0.05, 0.05, 0.05)));


            // TREES.
            
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeSmallFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(9)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeSmallFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(10)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(7)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeSnowFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(8)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeWallFoliage)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetFloorType(FoliageFloor::High)
                .SetNodeType(FoliageHelpers::HIGH_GROUND_NODE_TYPE)
                .SetMapIndex(52)
                .SetRandomRotation(true)
                .SetColour(Colour(0.075, 0.225, 0.125)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeEvergreenFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(21)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true)
                .SetColour(Colour(0.05, 0.05, 0.05)));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeEvergreenHighFoliage)
                .SetMapIndex(51)
                .SetRandomRotation(true)
                .SetColour(Colour(0.075, 0.225, 0.125))
                );

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeSmallEvergreenFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(22)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeSnowEvergreenFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(23)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true));

            // ROCKS.
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_RockClusterFoliage)
                .SetMapIndex(11)
                .SetRandomRotation(true)
                .SetColour(Colour(0.41, 0.41, 0.41))
                );

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_RockClusterSnowFoliage)
                .SetMapIndex(12)
                .SetRandomRotation(true)
                .SetColour(Colour(0.41, 0.41, 0.41)));

            // BOULDERS.
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_RoundBoulder)
                .SetMapIndex(57)
                .SetBuildable(false)
                .SetWalkable(false)

                // For now we offset the shadows in a fixed direction,
                // so disabling random rotation.
                // Shouldn't matter that much for round boulders anyways.
                .SetRandomRotation(false)
                .SetColour(Colour(0.75, 0.55, 0.5)));

            // UNBUILDABLE.
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_GroundUnbuildable)
                .SetWalkable(true)
                .SetBuildable(false)
                .SetMapIndex(53)
                .SetColour(Colour(0.5, 0.5, 0.55))
                );

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_ThrottenGroundUnbuildable)
                .SetWalkable(true)
                .SetBuildable(false)
                .SetThrotten(ThrottenAmount::Level01)
                .SetMapIndex(54)
                .SetColour(Colour(0.3, 0.25, 0.35))
                );

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_VeryThrottenGroundUnbuildable)
                .SetWalkable(true)
                .SetBuildable(false)
                .SetThrotten(ThrottenAmount::Level02)
                .SetMapIndex(55)
                .SetColour(Colour(0.35, 0.3, 0.35))
                );
            
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CompletelyThrottenGroundUnbuildable)
                .SetWalkable(true)
                .SetBuildable(false)
                .SetThrotten(ThrottenAmount::Completely)
                .SetMapIndex(56)
                .SetColour(Colour(0.45, 0.4, 0.35))
                );

            // GROUND.
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_ThrottenGround)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(4)
                .SetNodeBiome(LevelBiome::Desert)
                .SetColour(Colour(0.3, 0.25, 0.25))
                .SetThrotten(ThrottenAmount::Level01));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_VeryThrottenGround)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(5)
                .SetNodeBiome(LevelBiome::Desert)
                .SetColour(Colour(0.35, 0.3, 0.25))
                .SetThrotten(ThrottenAmount::Level02));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CompletelyThrottenGround)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(6)
                .SetNodeBiome(LevelBiome::Desert)
                .SetThrotten(ThrottenAmount::Completely)
                .SetColour(Colour(0.45, 0.4, 0.25)));

        
        // HILLS.

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_HillSmall)
            .SetMapIndex(24)
            .SetNodeBiome(LevelBiome::Desert)
            .SetRandomRotation(true)
            .SetColour(Colour(0.75, 0.6, 0.55)));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_HillSmallDesert)
            .SetMapIndex(26)
            .SetNodeBiome(LevelBiome::Desert)
            // .SetBiomeResourcePrefabs(LevelBiome::Desert,
            // 	new std::vector<ResourceType> {ResourceType.HillSmall})
            // .SetBiomeResourcePrefabs(LevelBiome::Grass,
            // 	new std::vector<ResourceType> {ResourceType.HillSmall})
            // .SetBiomeResourcePrefabs(LevelBiome::Snow,
            // 	new std::vector<ResourceType> {ResourceType.HillSmall})
            .SetRandomRotation(true)
            .SetColour(Colour(0.75, 0.6, 0.55)));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_HillSmallSnow)
            .SetMapIndex(25)
            .SetNodeBiome(LevelBiome::Snow)
            .SetRandomRotation(true)
            .SetColour(Colour(0.55, 0.6, 0.75)));



        // CLIFFSIDES.

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertMiddle)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1301)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionNone)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertUp)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1201)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionUp)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertDown)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1202)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionDown)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertLeft)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1203)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionLeft)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertRight)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1204)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionRight)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertUpLeft)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1205)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionUpLeft)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertUpRight)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1206)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionUpRight)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertDownLeft)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1207)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionDownLeft)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideDesertDownRight)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(1208)
                .SetNodeBiome(LevelBiome::None)
                .SetDirection(Direction::DirectionDownRight)
                .SetDirectionSet(cliffsideDesertDirections));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_Cliffside)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(16)
                .SetNodeBiome(LevelBiome::None)
                .SetThrotten(ThrottenAmount::Completely));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideShort)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(17)
                .SetNodeBiome(LevelBiome::None)
                .SetThrotten(ThrottenAmount::Completely));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_CliffsideHigh)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetMapIndex(18)
                .SetNodeBiome(LevelBiome::None)
                .SetThrotten(ThrottenAmount::Completely));

            // PLANTS.
        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_PlantClusterFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(14)
                .SetNodeBiome(LevelBiome::None));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_FlowerClusterFoliage)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(13)
                .SetNodeBiome(LevelBiome::None));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_MonumentCluster)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(15)
                .SetNodeBiome(LevelBiome::None));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_PillarSmallDesert)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(33)
                .SetNodeBiome(LevelBiome::Desert)
                .SetThrotten(ThrottenAmount::Completely)
                .SetColour(Colour(0.55, 0.5, 0.25))
                );

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_FlatStone)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(38)
                .SetNodeBiome(LevelBiome::None));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_FlatStoneDesert)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetMapIndex(40)
                .SetNodeBiome(LevelBiome::Desert)
                .SetThrotten(ThrottenAmount::Completely));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_StoneCircle)
                .SetMapIndex(58)
                .SetNodeBiome(LevelBiome::None)
                .SetRandomRotation(true));

        foliageList.push_back(FoliageInfo()
                .SetFoliageType(FoliageType::Foliage_TreeCliffedge)
                .SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetWalkable(true)
                .SetBuildable(false)
                .SetMapIndex(50)
                .SetNodeBiome(LevelBiome::Grass)
                .SetRandomRotation(true));

		for(auto info : foliageList) {
			info.SetFoliageType(info.foliageType);

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
