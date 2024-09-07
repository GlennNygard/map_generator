#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"


namespace cliffsidehigh {

    template<size_t foliageSize>
    static void setup(std::array<FoliageInfo, foliageSize> &mainFoliageArray, int mapIndexStart) {

        Colour colour (0.6, 0.55, 0.5);
        float foliageHeight = 0;

        std::vector<FoliageInfo> foliageList = {};

        std::vector<FoliageType> directionSet = {
            FoliageType::Foliage_CliffsideHighUp,
            FoliageType::Foliage_CliffsideHighDown,
            FoliageType::Foliage_CliffsideHighLeft,
            FoliageType::Foliage_CliffsideHighRight,

            FoliageType::Foliage_CliffsideHighUpLeft,
            FoliageType::Foliage_CliffsideHighUpRight,
            FoliageType::Foliage_CliffsideHighDownLeft,
            FoliageType::Foliage_CliffsideHighDownRight,

            FoliageType::Foliage_CliffsideHighMiddle,
            // 9

            FoliageType::Foliage_NoSelection,
            FoliageType::Foliage_NoSelection,
            FoliageType::Foliage_NoSelection,
            FoliageType::Foliage_NoSelection,

            FoliageType::Foliage_NoSelection,
            FoliageType::Foliage_NoSelection,
            FoliageType::Foliage_NoSelection,
            // 16

            FoliageType::Foliage_CliffsideHighCenterUpLeft,
            FoliageType::Foliage_CliffsideHighCenterUpRight,
            FoliageType::Foliage_CliffsideHighCenterDownLeft,
            FoliageType::Foliage_CliffsideHighCenterDownRight,
        };

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighUp)
            .SetMapIndex(mapIndexStart + 1)
            .SetDirection(Direction::DirectionUp)
            .SetPossibleRotations({0}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighDown)
            .SetMapIndex(mapIndexStart + 2)
            .SetDirection(Direction::DirectionDown)
            .SetPossibleRotations({180}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighLeft)
            .SetMapIndex(mapIndexStart + 3)
            .SetDirection(Direction::DirectionLeft)
            .SetPossibleRotations({270}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighRight)
            .SetMapIndex(mapIndexStart + 4)
            .SetDirection(Direction::DirectionRight)
            .SetPossibleRotations({90}));


        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighUpLeft)
            .SetMapIndex(mapIndexStart + 5)
            .SetDirection(Direction::DirectionUpLeft)
            .SetPossibleRotations({0}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighUpRight)
            .SetMapIndex(mapIndexStart + 6)
            .SetDirection(Direction::DirectionUpRight)
            .SetPossibleRotations({90}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighDownLeft)
            .SetMapIndex(mapIndexStart + 7)
            .SetDirection(Direction::DirectionDownLeft)
            .SetPossibleRotations({270}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighDownRight)
            .SetMapIndex(mapIndexStart + 8)
            .SetDirection(Direction::DirectionDownRight)
            .SetPossibleRotations({180}));


        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighMiddle)
            .SetMapIndex(mapIndexStart + 9)
            .SetDirection(Direction::DirectionNone)
            .SetPossibleRotations({0,90,180,270}));


        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighCenterUpLeft)
            .SetMapIndex(mapIndexStart + 10)
            .SetDirection(Direction::DirectionUpLeft)
            .SetPossibleRotations({0,90,180,270}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighCenterUpRight)
            .SetMapIndex(mapIndexStart + 11)
            .SetDirection(Direction::DirectionUpRight)
            .SetPossibleRotations({0,90,180,270}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighCenterDownLeft)
            .SetMapIndex(mapIndexStart + 12)
            .SetDirection(Direction::DirectionDownLeft)
            .SetPossibleRotations({0,90,180,270}));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_CliffsideHighCenterDownRight)
            .SetMapIndex(mapIndexStart + 13)
            .SetDirection(Direction::DirectionDownRight)
            .SetPossibleRotations({0,90,180,270}));

        for(auto info : foliageList) {
            info.SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetNodeBiome(LevelBiome::None)
                .SetWalkable(false)
                .SetBuildable(false)
                .SetColour(colour)
                .SetHeight(foliageHeight)
                .SetDirectionSet(directionSet);

            // Write data to main dict.
            mainFoliageArray[static_cast<int>(info.foliageType)] = info;
        }
    }
};
