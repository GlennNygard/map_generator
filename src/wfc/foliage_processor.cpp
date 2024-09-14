

#include "foliage_processor.h"
#include "map_processor.h"


FoliageProcessor::FoliageProcessor(
        BiomeFoliageInfo&& biomeFoliageInfo,
        const LevelValues& levelValues,
        bool verboseLogging) :
            m_biomeFoliageInfo(std::move(biomeFoliageInfo)),
            m_levelValues(levelValues) {
}


std::pair<Matrix<FoliageType>, bool> FoliageProcessor::ProcessNewMap() {

    MapProcessor mapProcessor (
        m_biomeFoliageInfo,
        m_levelValues,
        m_verboseLogging);
    return mapProcessor.RunProcessing();
}
