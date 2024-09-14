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

    static std::vector<Vector2Int> get_4_way_neighbours(Vector2Int gridPos, int mapSize) {
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

    static Vector2Int convert_to_global(int secX, int secY, int x, int y) {
        int fullX = ((MapDefinitions::SUBSECTION_SIZE * secX) - MapDefinitions::SUBSECTION_BORDER) + x;
        int fullY = ((MapDefinitions::SUBSECTION_SIZE * secY) - MapDefinitions::SUBSECTION_BORDER) + y;
        return {fullX,fullY};
    }

    static Vector2Int node_grid_pos_from_grid_space_point(
            Vector2Int gridSpacePos, int countX, int countY) {

        float valX = (gridSpacePos.x + (((float)countX)/2));
        float valY = (gridSpacePos.y + (((float)countY)/2));

        int x = static_cast<int>(std::floorf(valX));
        int y = static_cast<int>(std::floorf(valY));

        return Vector2Int(x, y);
    }
}
