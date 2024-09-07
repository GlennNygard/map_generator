#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"


namespace wallhigher {

    template<size_t foliageSize>
    static void setup(std::array<FoliageInfo, foliageSize> &mainFoliageArray, int mapIndexStart) {

        Colour colour (0.2, 0.05, 0.05);

        std::vector<FoliageInfo> foliageList = {};

        std::vector<FoliageType> wallHigherDirections = {
            FoliageType::Foliage_WallHigherUp,
            FoliageType::Foliage_WallHigherDown,
            FoliageType::Foliage_WallHigherLeft,
            FoliageType::Foliage_WallHigherRight,
            FoliageType::Foliage_WallHigherUpLeft,
            FoliageType::Foliage_WallHigherUpRight,
            FoliageType::Foliage_WallHigherDownLeft,
            FoliageType::Foliage_WallHigherDownRight,
            FoliageType::Foliage_WallHigherMiddle,

            FoliageType::Foliage_WallHigherThreeUp,
            FoliageType::Foliage_WallHigherThreeDown,
            FoliageType::Foliage_WallHigherThreeLeft,
            FoliageType::Foliage_WallHigherThreeRight,

            FoliageType::Foliage_WallHigherFrontBackUp,
            FoliageType::Foliage_WallHigherFrontBackLeft,

            FoliageType::Foliage_WallHigherAll,
        };

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherUp,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 1,
            LevelBiome::None,
            Direction::DirectionUp));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherDown,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 2,
            LevelBiome::None,
            Direction::DirectionDown)
            .SetPossibleRotations({180}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherLeft,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 3,
            LevelBiome::None,
            Direction::DirectionLeft)
            .SetPossibleRotations({270}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherRight,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 4,
            LevelBiome::None,
            Direction::DirectionRight)
            .SetPossibleRotations({90}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherUpLeft,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 5,
            LevelBiome::None,
            Direction::DirectionUpLeft));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherUpRight,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 6,
            LevelBiome::None,
            Direction::DirectionUpRight)
            .SetPossibleRotations({90}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherDownLeft,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 7,
            LevelBiome::None,
            Direction::DirectionDownLeft)
            .SetPossibleRotations({270}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherDownRight,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 8,
            LevelBiome::None,
            Direction::DirectionDownRight)
            .SetPossibleRotations({180}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherMiddle,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 9,
            LevelBiome::None,
            Direction::DirectionNone));


        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherThreeUp,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 10,
            LevelBiome::None,
            Direction::DirectionUp));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherThreeDown,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 11,
            LevelBiome::None,
            Direction::DirectionDown)
            .SetPossibleRotations({180}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherThreeLeft,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 12,
            LevelBiome::None,
            Direction::DirectionLeft)
            .SetPossibleRotations({270}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherThreeRight,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 13,
            LevelBiome::None,
            Direction::DirectionRight)
            .SetPossibleRotations({90}));


        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherFrontBackUp,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 14,
            LevelBiome::None,
            Direction::DirectionUp)
            .SetPossibleRotations({90}));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherFrontBackLeft,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 15,
            LevelBiome::None,
            Direction::DirectionDown));

        foliageList.push_back(FoliageInfo(
            FoliageType::Foliage_WallHigherAll,
            FoliageHelpers::HIGH_GROUND_NODE_TYPE,
            false,
            false,
            mapIndexStart + 16,
            LevelBiome::None,
            Direction::DirectionDown));

        for(auto info : foliageList) {
            info.SetColour(colour);

            // Write data to main dict.
            int foliageIndex = static_cast<int>(info.foliageType);
            if(foliageIndex < 0 || foliageIndex >=  static_cast<int>(FoliageType::Foliage_MAX)) {
                std::cerr << std::format("Foliage index {} not in range.", foliageIndex) << std::endl;
                continue;
            }
            mainFoliageArray[foliageIndex] = info;
        }
    }
};
