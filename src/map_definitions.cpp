
#include <unordered_map>

#include "map_definitions.h"
#include "foliage/foliage.h"
#include "gmath.h"

const std::unordered_map<MapSize, Vector2Int> MapDefinitions::mapSizeMappings = {
    {MapSize::MapSize_Smaller, Vector2Int(4,4)},
    {MapSize::MapSize_Small, Vector2Int(5,5)},
    {MapSize::MapSize_Medium, Vector2Int(7,7)},
    {MapSize::MapSize_Large, Vector2Int(9,9)},
    {MapSize::MapSize_VeryLarge, Vector2Int(11,11)},
    {MapSize::MapSize_IncrediblyLarge, Vector2Int(13,13)},
    {MapSize::MapSize_Massive, Vector2Int(15,15)},
};
