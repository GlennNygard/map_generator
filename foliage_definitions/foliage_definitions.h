#pragma once

#include <vector>

#include "../foliage/foliage.h"
#include "../foliage_info.h"
#include "../map_definitions.h"

#include "foliage_common_setup.h"
#include "foliage_wall_setup.h"
#include "foliage_wall_higher_setup.h"
#include "foliage_chasm_setup.h"
#include "foliage_cliffside_high_setup.h"
#include "foliage_border_tree_setup.h"


class FoliageDefinitions {

    public:

    static const std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> get_foliage_definitions() {
        if(FoliageDefinitions::_foliageDict) {
            return *FoliageDefinitions::_foliageDict;
        }

        std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> foliageDict = {};

        FoliageCommonSetup::Setup(foliageDict, 0);
        FoliageChasmSetup::Setup(foliageDict, 1400);
        FoliageWallHigherSetup::Setup(foliageDict, 1500);
        FoliageWallSetup::Setup(foliageDict, 1600);
        FoliageBorderTreeSetup::Setup(foliageDict, 1700);
        FoliageCliffsideHighSetup::Setup(foliageDict, 1800);

        FoliageDefinitions::_foliageDict = foliageDict;
        return foliageDict;
    }

    private:
    static std::optional<std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)>> _foliageDict;
};
