
#include <stdio.h>
#include <iostream>
#include <format>
#include <string>
#include <algorithm>

#include "foliage.h"
#include "map_constructor.h"
#include "disk_manager.h"
#include "gmath.h"
#include "argparse.h"
#include "logger.h"


void CreateMaps(const LevelValues &levelValues, const ArgValues &argValues) {
    auto foliageDefinitions = FoliageDefinitions();
    MapConstructor mapConstructor(
        levelValues, foliageDefinitions, argValues.verboseLogging);
    auto mapDiskManager = DiskManager(foliageDefinitions);
    // Create map name prefix.
    std::string mapNamePrefix = mapDiskManager.get_map_prefix(
        argValues, levelValues.biome);
    for(int currentIndex = 0; currentIndex < argValues.mapCount; ++currentIndex) {

        // Create map name.
        std::string mapName = mapDiskManager.get_map_name(
            mapNamePrefix, currentIndex);

        // Create map.
        auto mapObjectOpt = mapConstructor.CreateMap(currentIndex);
        if(!mapObjectOpt) {
            continue;
        }
        MapObject mapObject = *mapObjectOpt;

        // Save map.
        mapDiskManager.SaveMap(mapObject, mapName, mapNamePrefix, currentIndex);
        // Save thumbnail.
        mapDiskManager.SaveMapThumbnail(mapObject.map, mapName, mapNamePrefix);
        currentIndex++;
    }
}

int main(int argc, char *argv[]) {
    logger::Log("Starting up...");

    auto argValuesOpt = ParseArgs(argc, argv);
    if(!argValuesOpt) {
        return 0;
    }
    ArgValues argValues = *argValuesOpt;

    // Create level setup values.
    LevelValues levelValues = MapDefinitions::CreateLevelValues(argValues.mapSize);

    CreateMaps(levelValues, argValues);
}
