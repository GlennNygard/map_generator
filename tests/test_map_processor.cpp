#include <catch2/catch_test_macros.hpp>

#include "map_processor.h"
#include "biome_info_import.h"
#include "matrix.h"


LevelValues CreateTestLevelValues() {
    auto lv = LevelValues();
    lv.biome = LevelBiome::Grass;

    const int sideCount = 6;

    lv.gridCountX = sideCount * 3;
    lv.gridCountY = sideCount * 3;

    lv.subsectionCountX = lv.gridCountX / sideCount;
    lv.subsectionCountY = lv.gridCountY / sideCount;

    lv.borderSideCountX = 3;
    lv.borderSideCountY = 3;

    lv.subsectionBorderedSideCountX = sideCount + lv.borderSideCountX*2;
    lv.subsectionBorderedSideCountY = sideCount + lv.borderSideCountY*2;

    lv.gridBoundsX = Vector2Int(0,lv.gridCountX);
    lv.gridBoundsY = Vector2Int(0,lv.gridCountY);

    lv.subsectionBorderCountX = 0;
    lv.subsectionBorderCountY = 0;

    return lv;
}


TEST_CASE("Map processor works correctly.", "[map_processor]") {
    logger::VERBOSE_LOGGING = false;

    std::srand(10);

    auto fd = FoliageDefinitions();
    BiomeInfoVariant testBiomeInfo = TestingBiomeInfo(fd);

    MapProcessor mapProcessor (
        testBiomeInfo,
        CreateTestLevelValues());
    
    auto resultPair = mapProcessor.RunProcessing();

    bool success = resultPair.second;
    auto matrix = resultPair.first;

    SECTION("Foliage processor finished sucessfully with correct foliage types.") {
        REQUIRE(success);
        CHECK(matrix.dim_a() == 18);
        CHECK(matrix.dim_b() == 18);

        CHECK(matrix[10][10] == 5);
        CHECK(matrix[0][0] == 5);
        CHECK(matrix[12][6] == 1);
        CHECK(matrix[12][7] == 5);
        CHECK(matrix[5][5] == 1);
        CHECK(matrix[10][17] == 5);
    }
}
