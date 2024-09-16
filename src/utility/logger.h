#pragma once

#include <iostream>
#include <format>

#include "foliage.h"
#include "foliage_data.h"
#include "foliage_info.h"
#include "matrix.h"


namespace logger {

    static const bool RUN_PRINTS = true;
    static const bool RUN_GRID_PRINTS = false;

    static void Log(std::string message) {
        if(!RUN_PRINTS) {
            return;
        }
        std::cout << message << std::endl;
    }

    static void LogError(std::string message) {
        if(!RUN_PRINTS) {
            return;
        }
        std::cerr << message << std::endl;
    }

    template<size_t foliageCount>
    static void LogGrid(
            const Matrix<FoliageData>& grid,
            const std::array<int, foliageCount> &walkablePossibleTypes) {

        if(!RUN_PRINTS) {
            return;
        }
        if(!RUN_GRID_PRINTS) {
            return;
        }

        int rows = grid.dim_a();
        int cols = grid.dim_b();

        // Loop through each row.
        std::string fullStringPossible = "";
        for(int y = rows-1; y >= 0; y--) {
            fullStringPossible += "\n";
            // Loop through each column.
            for(int x = 0; x < cols; x++) {
                FoliageData foliageData = grid[x][y];

                auto dataRemaining = foliageData.GetRemaining();

                auto defaultWalkable = walkablePossibleTypes;
                bool allSame = true;
                for(FoliageIndex fIndex = 0; fIndex < dataRemaining.size(); fIndex++) {
                    if(dataRemaining[fIndex] != defaultWalkable[fIndex]) {
                        allSame = false;
                        break;
                    }
                }

                if(allSame) {
                    fullStringPossible += std::format("{},{} - DEF WPT \t",x, y);
                    continue;
                }

                std::vector<int> remainingPossibleTypes = {};
                for(FoliageIndex fIndex = 0; fIndex < dataRemaining.size(); fIndex++) {
                    if(dataRemaining[fIndex] <= 0) {
                        continue;
                    }
                    remainingPossibleTypes.push_back(fIndex);
                }

                if(remainingPossibleTypes.size() == 1) {
                    fullStringPossible += std::format(
                        "{},{} - {} \t", x, y,
                        static_cast<int>(remainingPossibleTypes[0]));
                }
                else {
                    fullStringPossible += std::format(
                        "{},{} - {} \t\t", x, y,
                        remainingPossibleTypes.size());
                }
            }
        }
        logger::Log(fullStringPossible);
    }

    template<typename Index>
    static void LogGrid(
            const Matrix<Index>& grid) {
        if(!RUN_PRINTS) {
            return;
        }
        if(!RUN_GRID_PRINTS) {
            return;
        }

        int rows = grid.dim_a();
        int cols = grid.dim_b();

        // Loop through each row.
        std::string fullStringTypes = "";
        for(int y = rows-1; y >= 0; y--) {
            fullStringTypes += "\n";
            // Loop through each column.
            for (int x = 0; x < cols; x++) {
                auto foliageType = grid[x][y];
                fullStringTypes += std::format(
                    "{},{} - {} \t",
                    x, y, (foliageType));
            }
        }
        // Print the row to the Unity console.
        logger::Log(fullStringTypes);
    }
}
