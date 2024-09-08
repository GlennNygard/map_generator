#pragma once

#include "foliage.h"
#include "map_definitions.h"

struct ArgValues {
    std::string mapNamePrefix = "";
    // MapSize mapSize = MapSize::MapSize_Small;
    MapSize mapSize = MapSize::MapSize_Large;
    int mapCount = 1;
    bool verboseLogging = true;
};

std::optional<ArgValues> parse_args(int argc, char *argv[]);
