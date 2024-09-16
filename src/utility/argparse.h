#pragma once

#include "foliage.h"
#include "map_definitions.h"

struct ArgValues {
    std::string mapNamePrefix = "";
    MapSize mapSize = MapSize::MapSize_Small;
    // MapSize mapSize = MapSize::MapSize_Large;
    int mapCount;
    bool verboseLogging;
    int seed;
};

std::optional<ArgValues> ParseArgs(int argc, char *argv[]);
