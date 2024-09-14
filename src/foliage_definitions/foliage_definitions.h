#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"
#include "logger.h"


class FoliageDefinitions {
public:

    static std::optional<FoliageDefinitions> foliageDefinitions;

    FoliageDefinitions() {
        create_foliage_definitions();
    }

    std::vector<FoliageInfo> foliageInfoElements;

    const size_t GetFoliageCount() const {
        return foliageInfoElements.size();
    }

    std::unordered_map<std::string, FoliageType>& get_name_to_foliage_index_map() {
        return m_nameToFoliageIndex;
    }

    const FoliageType toFI(std::string foliageName) {
        auto nameToFoliageIndex = get_name_to_foliage_index_map();
        auto itr = nameToFoliageIndex.find(foliageName);
        if(itr == nameToFoliageIndex.end()) {
            logger::log_error("Could not find entry for foliage: "+foliageName);
        }
        return itr->second;
    };

private:

    std::unordered_map<std::string, FoliageType> m_nameToFoliageIndex;

    void create_foliage_definitions();
};
