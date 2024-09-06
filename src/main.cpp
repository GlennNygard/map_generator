
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <format>
#include <string>
#include <string_view>
#include <algorithm>

#include "foliage.h"
#include "map/map_constructor.h"
#include "disk_manager.h"
#include "gmath.h"
#include "argparse.h"


int main(int argc, char *argv[]) {

    auto argValuesOpt = parse_args(argc, argv);
    if(!argValuesOpt) {
        return 0;
    }
    ArgValues argValues = *argValuesOpt;
    std::cout << "Starting up..." << std::endl;

    // Create level setup values.
    LevelValues levelValues = MapDefinitions::create_level_values(argValues.mapSize);
    MapConstructor mapConstructor(levelValues, argValues.verboseLogging);
    auto mapDiskManager = DiskManager();

    // Create map name prefix.
    std::string mapNamePrefix = mapDiskManager.get_map_prefix(
        argValues, levelValues.biome);
    for(int currentIndex = 0; currentIndex < argValues.mapCount; currentIndex++) {
        
        // Create map name.
        std::string mapName = mapDiskManager.get_map_name(
            mapNamePrefix, currentIndex);
        
        // Create map.
        auto mapObjectOpt = mapConstructor.create_map(currentIndex);
        if(!mapObjectOpt) {
            return 1;
        }
        MapObject mapObject = *mapObjectOpt;

        // Save map.
        mapDiskManager.save_map(mapObject, mapName, mapNamePrefix, currentIndex);
        // Save thumbnail.
        mapDiskManager.save_map_thumbnail(mapObject.map, mapName, mapNamePrefix);
    }

    return 0;
}
