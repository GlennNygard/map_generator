#pragma once

#include <iostream>

#include "foliage.h"
#include "foliage_data.h"
#include "foliage_info.h"
#include "matrix.h"


namespace logger {

    static const bool RUN_PRINTS = true;
    static const bool RUN_GRID_PRINTS = false;

    static void log(std::string message) {
        if(!RUN_PRINTS) {
            return;
        }
        std::cout << message << std::endl;
    }

    static void log_error(std::string message) {
        if(!RUN_PRINTS) {
            return;
        }
        std::cerr << message << std::endl;
    }

    template<size_t foliageSize>
    static void log_grid(
            const Matrix<FoliageData>& grid,
            const std::array<int, foliageSize> &walkablePossibleTypes) {

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

                auto dataRemaining = foliageData.get_remaining_possible_types();

                auto defaultWalkable = walkablePossibleTypes;
                bool allSame = true;
                for(size_t i = 0; i < dataRemaining.size(); i++) {
                    if(dataRemaining[i] != defaultWalkable[i]) {
                        allSame = false;
                        break;
                    }
                }

                if(allSame) {
                    fullStringPossible += std::format("{},{} - DEF WPT \t",x, y);
                    continue;
                }

                std::vector<FoliageType> remainingPossibleTypes = {};
                for(int i = 0; i < dataRemaining.size(); i++) {
                    if(dataRemaining[i] <= 0) {
                        continue;
                    }
                    remainingPossibleTypes.push_back((FoliageType)i);
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
        std::cout << fullStringPossible << std::endl;
    }

    static void log_grid(
            const Matrix<FoliageType>& grid) {
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
                    x, y, static_cast<int>(foliageType));
            }
        }
        // Print the row to the Unity console.
        std::cout << fullStringTypes << std::endl;
    }
}
