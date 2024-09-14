
#include <format>

#include "map_constructor.h"
#include "map_processor.h"
#include "biome_foliage_info.h"
#include "grass_foliage_info.h"
#include "foliage_info.h"
#include "foliage_definitions.h"
#include "relations.h"
#include "logger.h"


MapConstructor::MapConstructor(
        const LevelValues& levelValues,
        FoliageDefinitions& foliageDefinitions,
        bool verboseLogging) :
            m_levelValues(levelValues),
            m_verboseLogging(verboseLogging) {
    m_foliageDefinitions = std::make_unique<FoliageDefinitions>(foliageDefinitions);

    if(levelValues.biome == LevelBiome::Grass) {
        m_biomeFoliageInfo = std::make_unique<GrassFoliageInfo>(foliageDefinitions);
    }
    // ...
    // Support for more biomes will be added in the future.
    // <- Insert here.
    // ...
    else {
        m_biomeFoliageInfo = std::make_unique<GrassFoliageInfo>(foliageDefinitions);
    }
}

std::optional<MapObject> MapConstructor::CreateMap(int currentIndex) {
    logger::Log(std::format(
        "Starting map creation: {}",
        std::to_string(currentIndex)));

    auto mapPair = ConstructRandomMap();

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

void MapConstructor::CarveMainRoom(
        Matrix<int>& foliageMap,
        const Vector2Int worldPos,
        const float nodeDiameter,
        const int sectionCountX,
        const int sectionCountY) {

    float radius = 200; // 400

    float shortRadius = radius/nodeDiameter;
    int tileRange = static_cast<int>(std::round(shortRadius));
    Vector2Int nodePos = relations::node_grid_pos_from_grid_space_point(
        worldPos, sectionCountX, sectionCountY);

    for(int x = -tileRange + nodePos.x; x < tileRange + nodePos.x; x++) {
        for(int y = -tileRange + nodePos.y; y < tileRange + nodePos.y; y++) {
            if(x >= 0 &&
                    x < sectionCountX &&
                    y >= 0 &&
                    y < sectionCountY) {

                float dist = gmath::SqrDistance(nodePos - Vector2Int(x,y));
                if(dist > shortRadius*shortRadius) {
                    continue;
                }
                if(!m_foliageDefinitions->foliageInfoElements[
                        static_cast<int>(foliageMap[x][y])].walkable) {
                    foliageMap[x][y] = FoliageHelpers::NO_FOLIAGE_INDEX;
                }
            }
        }
    }
}

void MapConstructor::SetMapBorders(Matrix<int>& foliageMap, int borderCount) {
    for(int x = 0; x < foliageMap.dim_a(); x++) {
        for(int y = 0; y < foliageMap.dim_b(); y++) {
            if(x >= borderCount &&
                    x <= foliageMap.dim_a() - borderCount &&
                    y >= borderCount &&
                    y <= foliageMap.dim_b() - borderCount) {
                
                continue;
            }
            // For now we just set them all as no foliage.
            foliageMap[x][y] = FoliageHelpers::NO_FOLIAGE_INDEX;
        }
    }
}

std::pair<Matrix<FoliageType>, bool> MapConstructor::CreateRandomFoliageMap() {
    // Create random terrain.
    // Full map.
    int maxAttempts = 3;
    int currentAttempt = 0;
    Matrix<FoliageType> fullFoliageGrid = {};
    bool success = true;

    MapProcessor mapProcessor (
        *m_biomeFoliageInfo,
        m_levelValues,
        m_verboseLogging);
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

Matrix<int> MapConstructor::RandomFillSection(
        int sectionCountX,
        int sectionCountY,
        int randomFillPercent) {

    Matrix<int> nodeMap(sectionCountX,sectionCountY);
    for(int x = 0; x < sectionCountX; x++) {
        for(int y = 0; y < sectionCountY; y++) {
            float multX = gmath::EaseOutCubic(std::abs(((float)(x)/sectionCountX)-0.5f)*0.5f);
            float multY = gmath::EaseOutCubic(std::abs(((float)(y)/sectionCountY)-0.5f)*0.5f);

            int val = static_cast<int>(std::round(100 * (multX + multY)));
            if((rand() % val) < randomFillPercent) {
                nodeMap[x][y] = FoliageHelpers::HIGH_GROUND_NODE_TYPE;
            }
            else {
                nodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
            }
        }
    }
    return nodeMap;
}
