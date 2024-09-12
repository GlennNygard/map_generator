#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"
#include "logger.h"


class FoliageDefinitions {
public:

    static std::optional<FoliageDefinitions> foliageDefinitions;

    static FoliageDefinitions& instance();

    std::unordered_map<std::string, int> nameToFoliageIndex;
    std::vector<FoliageInfo> foliageInfoElements;

    const size_t get_foliage_count() const {
        return foliageInfoElements.size();
    }

    std::unordered_map<std::string, int>& get_name_to_foliage_index_map() {
        return nameToFoliageIndex;
    }

private:
    FoliageDefinitions() {
        create_foliage_definitions();
    }
    void create_foliage_definitions();
};
