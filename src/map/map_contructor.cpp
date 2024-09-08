

#include "map_constructor.h"
#include "foliage_processor.h"
#include "biome_foliage_info.h"
#include "grass_foliage_info.h"
#include "foliage_info.h"
#include "foliage_definitions.h"
#include "relations.h"


MapConstructor::MapConstructor(LevelValues levelValues, bool verboseLogging) {
    m_levelValues = levelValues;
    BiomeFoliageInfo biomeFoliageInfo;
    if(levelValues.biome == LevelBiome::Grass) {
        biomeFoliageInfo = GrassFoliageInfo();
    }
    else if(levelValues.biome == LevelBiome::Desert) {
        // We don't have any specific desert foliage info for now.
        biomeFoliageInfo = GrassFoliageInfo();
    }
    // else if(levelValues.biome == LevelBiome::Snow) {
    // 	biomeFoliageInfo = new SnowFoliageInfo();
    // }
    else {
        biomeFoliageInfo = GrassFoliageInfo();
    }
    m_foliageProcessor = std::make_unique<
        FoliageProcessor>(biomeFoliageInfo, verboseLogging);
}

std::optional<MapObject> MapConstructor::create_map(int currentIndex) {
    std::cout << std::format(
        "Starting map creation: {}",
        std::to_string(currentIndex)) << std::endl;

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
    std::cout << std::format("Using level SEED: {0}", levelSeed) << std::endl;
    srand(levelSeed);

    std::pair<Matrix<FoliageType> , bool> randomFoliageMapPair = create_random_foliage_map(
        m_levelValues);

    auto fullFoliageMap = randomFoliageMapPair.first;
    bool success = randomFoliageMapPair.second;


    // Sine we are transitioning into making foliage the main source of truth,
    // we copy the foliage node type back into the node map.
    Matrix<int> fullNodeMap (m_levelValues.gridCountX, m_levelValues.gridCountY);
    for(int x = 0; x < fullNodeMap.dim_a(); x++) {
        for(int y = 0; y < fullNodeMap.dim_b(); y++) {
            FoliageType foliage = fullFoliageMap[x][y];
            FoliageInfo foliageInfo = FoliageDefinitions::get_foliage_definitions()[(int)foliage];
            fullNodeMap[x][y] = foliageInfo.nodeType;
        }
    }

    if(!success) {
        std::cerr << "Random map could not be generated." << std::endl;
    }

    auto mapObject = MapObject(fullNodeMap, fullFoliageMap);
    return std::pair(mapObject, success);
}

Matrix<int> MapConstructor::carve_main_room(
        Matrix<int> nodeMap, Vector2Int worldPos,
        float nodeDiameter,
        int sectionCountX, int sectionCountY) {

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
                if(dist>shortRadius*shortRadius) {
                    continue;
                }
                if(nodeMap[x][y] == FoliageHelpers::HIGH_GROUND_NODE_TYPE) {
                    nodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
                }
            }
        }
    }

    return nodeMap;
}

Matrix<FoliageType> MapConstructor::carve_main_room(
        Matrix<FoliageType> foliageMap,
        Vector2Int worldPos,
        float nodeDiameter,
        int sectionCountX, int sectionCountY) {

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
                if(!FoliageDefinitions::get_foliage_definitions()[
                        static_cast<int>(foliageMap[x][y])].walkable) {
                    foliageMap[x][y] = FoliageType::Foliage_NoFoliage;
                }
            }
        }
    }

    return foliageMap;
}

Matrix<int> MapConstructor::generate_section_walkable_representation(
        Matrix<int> nodeMap, int smoothIterations) {

    // Previously 12.
    for(int i = 0; i < smoothIterations; i ++) {
        smooth_section(nodeMap);
    }

    int borderSize = 1;

    for (int x = 0; x < nodeMap.dim_a(); x ++) {
        for (int y = 0; y < nodeMap.dim_b(); y ++) {
            int nodeType = nodeMap[x][y];
            if(x >= borderSize &&
                    x < nodeMap.dim_a() - borderSize &&
                    y >= borderSize &&
                    y < nodeMap.dim_b() - borderSize) {
            }
            // If the node is a border.
            else {
                nodeType = FoliageHelpers::BORDER_NODE_TYPE;
            }
            nodeMap[x][y] = nodeType;
        }
    }
    return nodeMap;
}

Matrix<int> MapConstructor::smooth_section(Matrix<int> nodeMap) {
    int countX = nodeMap.dim_a();
    int countY = nodeMap.dim_b();
    for(int x = 0; x < countX; x ++) {
        for (int y = 0; y < countY; y ++) {
            int neighbourWallTiles = get_surrounding_wall_count(nodeMap, x,y);

            if (neighbourWallTiles > 4) {
                nodeMap[x][y] = FoliageHelpers::HIGH_GROUND_NODE_TYPE;
            }
            else if (neighbourWallTiles < 4) {
                nodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
            }
        }
    }
    return nodeMap;
}

int MapConstructor::get_surrounding_wall_count(Matrix<int> nodeMap, int gridX, int gridY) {

    // Returns how many walls are around each node in grid.
    int wallCount = 0;
    for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX ++) {
        for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY ++) {
            if (neighbourX >= 0 &&
                    neighbourX < nodeMap.dim_a() &&
                    neighbourY >= 0 &&
                    neighbourY < nodeMap.dim_b())
                {
                
                // If neighbour is within borders.
                if (neighbourX != gridX || neighbourY != gridY) {
                    // Add 1 if wall, 0 otherwise.
                    if(nodeMap[neighbourX][neighbourY] == FoliageHelpers::HIGH_GROUND_NODE_TYPE) {
                        wallCount += 1;
                    }
                }
            }
            else {
                wallCount ++;
            }
        }
    }
    
    return wallCount;
}


Matrix<FoliageType> MapConstructor::set_map_borders(
        Matrix<FoliageType> foliageMap,
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
            foliageMap[x][y] = FoliageType::Foliage_NoFoliage;
        }
    }

    return foliageMap;
}

std::pair<Matrix<FoliageType>, bool> MapConstructor::create_random_foliage_map(
        LevelValues levelValues) {
    // Create random terrain.
    // Full map.
    int maxAttempts = 3;
    int currentAttempt = 0;
    Matrix<FoliageType> fullFoliageGrid = {};
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

            // Node node = grid.GetGridNode(x + section.gridStartX, y + section.gridStartY);
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
