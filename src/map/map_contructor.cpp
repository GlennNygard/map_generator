
#include <format>

#include "map_constructor.h"
#include "map_processor.h"
#include "foliage_info.h"
#include "foliage_definitions.h"
#include "grass_biome_info.h"
#include "relations.h"
#include "logger.h"


MapConstructor::MapConstructor(
        const LevelValues& levelValues,
        FoliageDefinitions& foliageDefinitions,
        const int overrideSeed) :
            m_levelValues(levelValues),
            m_biomeFoliageInfo(CreateBiomeInfo(levelValues.biome, foliageDefinitions)),
            m_overrideSeed(overrideSeed) {
    m_foliageDefinitions = std::make_unique<FoliageDefinitions>(foliageDefinitions);
}

BiomeInfoVariant MapConstructor::CreateBiomeInfo(LevelBiome biome, FoliageDefinitions& foliageDefinitions) {
    if(biome == LevelBiome::Grass) {
        return GrassBiomeInfo(foliageDefinitions);
    }
    // ...
    // Support for more biomes will be added in the future.
    // <- Insert here.
    // ...
    
    return GrassBiomeInfo(foliageDefinitions);
}

std::optional<MapObject> MapConstructor::CreateMap(int currentIndex) {
    logger::Log(std::format(
        "Starting map creation: {}",
        std::to_string(currentIndex)));

    auto mapPair = ConstructRandomMap(m_overrideSeed);

    MapObject mapObject = mapPair.first;
    bool success = mapPair.second;

    if(!success) {
        std::cerr << "Failed. Map: "+std::to_string(currentIndex) << std::endl;
        return std::nullopt;
    }

    return mapObject;
}

std::pair<MapObject, bool> MapConstructor::ConstructRandomMap(int levelSeed) {

    if(levelSeed == -1) {
        levelSeed = time(0);
    }
    logger::Log(std::format("Using level SEED: {0}", levelSeed));
    srand(levelSeed);

    std::pair<Matrix<FoliageType> , bool> randomFoliageMapPair = CreateRandomFoliageMap();

    auto fullFoliageMap = randomFoliageMapPair.first;
    bool success = randomFoliageMapPair.second;

    // Sine we are transitioning into making foliage the main source of truth,
    // we copy the foliage node type back into the node map.
    Matrix<int> fullNodeMap (m_levelValues.gridCountX, m_levelValues.gridCountY);
    for(int x = 0; x < fullNodeMap.dim_a(); x++) {
        for(int y = 0; y < fullNodeMap.dim_b(); y++) {
            int foliage = fullFoliageMap[x][y];
            FoliageInfo foliageInfo = m_foliageDefinitions->foliageInfoElements[(int)foliage];
            fullNodeMap[x][y] = foliageInfo.nodeType;
        }
    }

    if(!success) {
        std::cerr << "Random map could not be generated." << std::endl;
    }

    auto mapObject = MapObject(fullNodeMap, fullFoliageMap);
    return std::pair(mapObject, success);
}

std::pair<Matrix<FoliageType>, bool> MapConstructor::CreateRandomFoliageMap() {
    // Create random terrain.
    // Full map.
    int maxAttempts = 3;
    int currentAttempt = 0;
    Matrix<FoliageType> fullFoliageGrid = {};
    bool success = true;

    MapProcessor mapProcessor (
        m_biomeFoliageInfo,
        m_levelValues);
    while(fullFoliageGrid.empty() && currentAttempt < maxAttempts) {

        auto resultPair = mapProcessor.RunProcessing();

        fullFoliageGrid = resultPair.first;
        success = resultPair.second;
        if(!success) {
            std::cerr << std::format(
                "Random map generation failed. Attempt {0} of {1}. Trying again...",
                currentAttempt+1, maxAttempts) << std::endl;
        }
        currentAttempt++;
    }

    return std::pair(fullFoliageGrid, success);
}
