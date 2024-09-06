#pragma once

#include "foliage.h"
#include "map/map_definitions.h"

struct ArgValues {
    std::string mapNamePrefix = "";
    MapSize mapSize = MapSize::MapSize_Small;
    int mapCount = 1;
    bool verboseLogging = false;
};

std::optional<ArgValues> parse_args(int argc, char *argv[]);
