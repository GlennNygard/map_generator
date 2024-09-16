#pragma once

#include <vector>

#include "gmath.h"
#include "map_definitions.h"


namespace relations {

    static std::vector<Vector2Int> GetNeighbours(Vector2Int gridPos, int mapSize) {
        int lengthX = mapSize;
        int lengthY = mapSize;
        std::vector<Vector2Int> vecList = {};
        for(int y = 1; y >= -1; y--) {
            for(int x = -1; x <= 1; x++) {

                if(x == 0 && y == 0) {
                    continue;
                }

                int gridX = gridPos.x + x;
                int gridY = gridPos.y + y;

                if(gridX >= lengthX || gridX < 0 || gridY >= lengthY || gridY < 0) {
                    continue;
                }

                vecList.push_back({gridX, gridY});
            }
        }
        return vecList;
    }

    static std::vector<Vector2Int> Get4WayNeighbours(Vector2Int gridPos, int mapSize) {
        int lengthX = mapSize;
        int lengthY = mapSize;
        std::vector<Vector2Int> vecList = {};
        if(gridPos.x-1 >= 0) {
            vecList.push_back({gridPos.x-1, gridPos.y});
        }
        if(gridPos.x+1 < lengthX) {
            vecList.push_back({gridPos.x+1, gridPos.y});
        }
        if(gridPos.y-1 >= 0) {
            vecList.push_back({gridPos.x, gridPos.y-1});
        }
        if(gridPos.y+1 < lengthY) {
            vecList.push_back({gridPos.x, gridPos.y+1});
        }
        return vecList;
    }

    static Vector2Int ConvertToGlobal(int secX, int secY, int x, int y) {
        int fullX = ((MapDefinitions::SUBSECTION_SIZE * secX) - MapDefinitions::SUBSECTION_BORDER) + x;
        int fullY = ((MapDefinitions::SUBSECTION_SIZE * secY) - MapDefinitions::SUBSECTION_BORDER) + y;
        return {fullX,fullY};
    }
}
