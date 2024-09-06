#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"


class FoliageBorderTreeSetup {

    public:

    template<size_t foliageSize>
    static void Setup(std::array<FoliageInfo, foliageSize> &mainFoliageArray, int mapIndexStart) {

        Colour colour (0.08, 0.24, 0.08);
        float foliageHeight = 0;

        std::vector<FoliageInfo> foliageList = {};

        std::vector<FoliageType> directionSet = {
            FoliageType::Foliage_BorderTreeUp,
            FoliageType::Foliage_BorderTreeDown,
            FoliageType::Foliage_BorderTreeLeft,
            FoliageType::Foliage_BorderTreeRight,

            FoliageType::Foliage_BorderTreeUpLeft,
            FoliageType::Foliage_BorderTreeUpRight,
            FoliageType::Foliage_BorderTreeDownLeft,
            FoliageType::Foliage_BorderTreeDownRight,

            FoliageType::Foliage_BorderTreeMiddle,
        };

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeUp)
            .SetMapIndex(mapIndexStart + 1)
            .SetDirection(Direction::DirectionUp));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeDown)
            .SetMapIndex(mapIndexStart + 2)
            .SetDirection(Direction::DirectionDown));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeLeft)
            .SetMapIndex(mapIndexStart + 3)
            .SetDirection(Direction::DirectionLeft));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeRight)
            .SetMapIndex(mapIndexStart + 4)
            .SetDirection(Direction::DirectionRight));


        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeUpLeft)
            .SetMapIndex(mapIndexStart + 5)
            .SetDirection(Direction::DirectionUpLeft));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeUpRight)
            .SetMapIndex(mapIndexStart + 6)
            .SetDirection(Direction::DirectionUpRight));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeDownLeft)
            .SetMapIndex(mapIndexStart + 7)
            .SetDirection(Direction::DirectionDownLeft));

        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeDownRight)
            .SetMapIndex(mapIndexStart + 8)
            .SetDirection(Direction::DirectionDownRight));


        foliageList.push_back(FoliageInfo()
            .SetFoliageType(FoliageType::Foliage_BorderTreeMiddle)
            .SetMapIndex(mapIndexStart + 9)
            .SetDirection(Direction::DirectionNone));

        for(auto info : foliageList) {
            info.SetNodeType(FoliageHelpers::FLOOR_NODE_TYPE)
                .SetNodeBiome(LevelBiome::None)
                .SetWalkable(true)
                .SetBuildable(true)
                .SetColour(colour)
                .SetHeight(foliageHeight)
                .SetDirectionSet(directionSet)
                .SetRandomRotation(true);

            // Write data to main dict.
            mainFoliageArray[static_cast<int>(info.foliageType)] = info;
        }
    }
};
