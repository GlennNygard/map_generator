#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"

#include "foliage_common_setup.h"
#include "foliage_wall_setup.h"
#include "foliage_wall_higher_setup.h"
#include "foliage_chasm_setup.h"
#include "foliage_cliffside_high_setup.h"
#include "foliage_border_tree_setup.h"


class FoliageDefinitions {

    public:
    static const std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)> get_foliage_definitions();

    private:
    static std::optional<std::array<FoliageInfo, static_cast<size_t>(FoliageType::Foliage_MAX)>> _foliageDict;
};
