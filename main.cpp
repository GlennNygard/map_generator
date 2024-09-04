
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <format>
#include <string>
#include <string_view>
#include <algorithm>

#include "foliage/foliage.h"
#include "map_constructor.h"
#include "mapdisk_manager.h"
#include "gmath.h"


int main(int argc,char *argv[]) {

    std::copy(argv, argv + argc, std::ostream_iterator<char *>(std::cout, "\n"));

    std::cout << "Starting up..." << std::endl;

    auto mapDiskManager = MapDiskManager();
    LevelBiome mapBiome = LevelBiome::Grass;
    MapSize mapSize = MapSize::MapSize_Small;

    Vector2Int sectionCount = MapDefinitions::mapSizeMappings.at(mapSize);
    LevelValues levelValues = LevelValues::create_values(sectionCount, mapBiome);

    int currentIndex = 0;

    std::string mapNamePrefix = mapDiskManager.get_map_name_prefix(mapBiome, mapSize);
    std::string mapPath = mapDiskManager.get_map_path(mapBiome, mapSize);


    if(!MapDiskManager::get_path_exists(mapDiskManager.get_base_map_path())) {
        std::cerr << std::format("Path does not exist: {}",mapPath) << std::endl;
        return 1;
    }

    std::cout << std::format("Starting map creation: {}", std::to_string(currentIndex)) << std::endl;

    MapConstructor mapConstructor(levelValues);
    auto mapPair = mapConstructor.construct_random_map();
    MapObject mapObject = mapPair.first;
    bool success = mapPair.second;

    if(!success) {
        std::cerr << "Failed. Map: "+std::to_string(currentIndex) << std::endl;
        return 1;
    }

    std::string mapName = mapNamePrefix+"_"+std::to_string(currentIndex);

    std::filesystem::path finalPath(mapPath);
    finalPath /= (mapName+".txt");

    std::cout << "Saving map to path: "+finalPath.generic_string() << std::endl;

    mapDiskManager.save_map(mapObject.map, finalPath);
    std::cout << "Save completed! map: "+std::to_string(currentIndex) << std::endl;

    // Saving thumbnail.
    mapDiskManager.save_map_thumbnail(mapObject.map, mapName);

    return 0;
}
