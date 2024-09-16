
#include <stdio.h>
#include <iostream>
#include <format>
#include <string>

#include "foliage.h"
#include "map_constructor.h"
#include "disk_manager.h"
#include "gmath.h"
#include "argparse.h"
#include "logger.h"


void CreateMaps(const LevelValues &levelValues, const argparse::ArgValues &argValues) {
    auto foliageDefinitions = FoliageDefinitions();
    MapConstructor mapConstructor(
        levelValues, foliageDefinitions, argValues.seed);
    auto mapDiskManager = DiskManager(foliageDefinitions);
    // Create map name prefix.
    std::string mapNamePrefix = mapDiskManager.GetMapPrefix(
        argValues, levelValues.biome);
    for(int currentIndex = 0; currentIndex < argValues.mapCount; currentIndex++) {

        // Create map name.
        std::string mapName = mapDiskManager.GetMapName(
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
    }
}

int main(int argc, char *argv[]) {
    logger::Log("Starting up...");

    auto argValuesOpt = argparse::ParseArgs(argc, argv);
    if(!argValuesOpt) {
        return 0;
    }
    argparse::ArgValues argValues = *argValuesOpt;
    logger::VERBOSE_LOGGING = argValues.verboseLogging;

    // Create level setup values.
    LevelValues levelValues = MapDefinitions::CreateLevelValues(argValues.mapSize);

    CreateMaps(levelValues, argValues);
}
