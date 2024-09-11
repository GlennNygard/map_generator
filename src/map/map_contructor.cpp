
#include <format>

#include "map_constructor.h"
#include "foliage_processor.h"
#include "biome_foliage_info.h"
#include "grass_foliage_info.h"
#include "foliage_info.h"
#include "foliage_definitions.h"
#include "relations.h"
#include "logger.h"


MapConstructor::MapConstructor(LevelValues levelValues, bool verboseLogging) {
    m_levelValues = levelValues;
    BiomeFoliageInfo biomeFoliageInfo;
    if(levelValues.biome == LevelBiome::Grass) {
        biomeFoliageInfo = GrassFoliageInfo();
    }
    // Support for more biomes will be added in the future.
    // ...
    else {
        biomeFoliageInfo = GrassFoliageInfo();
    }
    m_foliageProcessor = std::make_unique<
        FoliageProcessor>(biomeFoliageInfo, verboseLogging);
}

std::optional<MapObject> MapConstructor::create_map(int currentIndex) {
    logger::log(std::format(
        "Starting map creation: {}",
        std::to_string(currentIndex)));

    auto mapPair = construct_random_map();

    MapObject mapObject = mapPair.first;
    bool success = mapPair.second;

    if(!success) {
        std::cerr << "Failed. Map: "+std::to_string(currentIndex) << std::endl;
        return std::nullopt;
    }

    return mapObject;
}

std::pair<MapObject, bool> MapConstructor::construct_random_map(int levelSeed) {

    if(levelSeed == -1) {
        levelSeed = time(0);
    }
    logger::log(std::format("Using level SEED: {0}", levelSeed));
    srand(levelSeed);

    std::pair<Matrix<int> , bool> randomFoliageMapPair = create_random_foliage_map(
        m_levelValues);

    auto fullFoliageMap = randomFoliageMapPair.first;
    bool success = randomFoliageMapPair.second;


    // Sine we are transitioning into making foliage the main source of truth,
    // we copy the foliage node type back into the node map.
    Matrix<int> fullNodeMap (m_levelValues.gridCountX, m_levelValues.gridCountY);
    for(int x = 0; x < fullNodeMap.dim_a(); x++) {
        for(int y = 0; y < fullNodeMap.dim_b(); y++) {
            int foliage = fullFoliageMap[x][y];
            FoliageInfo foliageInfo = foliagedef::get_foliage_definitions().foliageInfoElements[(int)foliage];
            fullNodeMap[x][y] = foliageInfo.nodeType;
        }
    }

    if(!success) {
        std::cerr << "Random map could not be generated." << std::endl;
    }

    auto mapObject = MapObject(fullNodeMap, fullFoliageMap);
    return std::pair(mapObject, success);
}

void MapConstructor::carve_main_room(
        Matrix<int> &foliageMap,
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

                float dist = square_dist(nodePos - Vector2Int(x,y));
                if(dist > shortRadius*shortRadius) {
                    continue;
                }
                if(!foliagedef::get_foliage_definitions().foliageInfoElements[
                        static_cast<int>(foliageMap[x][y])].walkable) {
                    foliageMap[x][y] = FoliageHelpers::NO_FOLIAGE_INDEX;
                }
            }
        }
    }
}

void MapConstructor::set_map_borders(
        Matrix<int> &foliageMap,
        int borderCount) {

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

std::pair<Matrix<int>, bool> MapConstructor::create_random_foliage_map(
        const LevelValues &levelValues) {
    // Create random terrain.
    // Full map.
    int maxAttempts = 3;
    int currentAttempt = 0;
    Matrix<int> fullFoliageGrid = {};
    bool success = true;
    while(fullFoliageGrid.empty() && currentAttempt < maxAttempts) {
        auto resultPair = m_foliageProcessor->mark_foliage_nodes(levelValues);
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

Matrix<int> MapConstructor::random_fill_section(
        int sectionCountX,
        int sectionCountY,
        int randomFillPercent) {

    Matrix<int> nodeMap(sectionCountX,sectionCountY);
    for(int x = 0; x < sectionCountX; x++) {
        for(int y = 0; y < sectionCountY; y++) {
            float multX = ease_out_cubic(std::abs(((float)(x)/sectionCountX)-0.5f)*0.5f);
            float multY = ease_out_cubic(std::abs(((float)(y)/sectionCountY)-0.5f)*0.5f);

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
