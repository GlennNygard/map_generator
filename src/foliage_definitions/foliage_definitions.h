#pragma once

#include <vector>

#include "foliage.h"
#include "foliage_info.h"
#include "map_definitions.h"
#include "logger.h"


using FoliageArray = std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>;

class FoliageDefinitions {
public:

    static std::optional<FoliageDefinitions> foliageDefinitions;

    FoliageDefinitions() {
        CreateFoliageDefinitions();
    }

    std::vector<FoliageInfo> foliageInfoElements;

    const size_t GetFoliageCount() const {
        return foliageInfoElements.size();
    }

    const FoliageType ToFI(std::string foliageName) const {
        auto itr = m_nameToFoliageIndex.find(foliageName);
        if(itr == m_nameToFoliageIndex.end()) {
            logger::LogError("Could not find entry for foliage: "+foliageName);
        }
        return itr->second;
    };

private:

    std::unordered_map<std::string, FoliageType> m_nameToFoliageIndex;

    void CreateFoliageDefinitions();
};
