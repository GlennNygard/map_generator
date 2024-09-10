#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"
#include "logger.h"


class FoliageDefinitions {
public:

    int foliageNoSelection;

    FoliageDefinitions() {
        create_foliage_definitions();
    }

    const size_t get_foliage_count() const {
        return foliageInfoElements.size();
    }

    std::unordered_map<std::string, int>& get_name_to_foliage_index_map() {
        return nameToFoliageIndex;
    }

    std::unordered_map<std::string, int> nameToFoliageIndex;
    std::vector<FoliageInfo> foliageInfoElements;

private:
    void create_foliage_definitions();
};

namespace foliagedef {
    static std::optional<FoliageDefinitions> foliageDefinitions;

    static FoliageDefinitions get_foliage_definitions() {
        if(foliageDefinitions) {
            return *foliageDefinitions;
        }

        foliageDefinitions = FoliageDefinitions();
        return *foliageDefinitions;
    }
}
