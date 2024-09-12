#include <catch2/catch_test_macros.hpp>

#include "foliage_processor.h"
#include "biome_foliage_info.h"
#include "matrix.h"


class TestBiomeFoliage : public BiomeFoliageInfo {
    public:
    TestBiomeFoliage() {

		allowedTypes = {
			{toFI("NoFoliage"), 40},
			{toFI("TreeFoliage"), 90},
			{toFI("TreeSmallFoliage"), 10},
            {toFI("FlowerClusterFoliage"), 2},
			{toFI("PlantClusterFoliage"), 1},
		};
		walkableAllowedTypes = {
            {toFI("NoFoliage"), 40},
			{toFI("TreeSmallFoliage"), 2},
			{toFI("FlowerClusterFoliage"), 2},
			{toFI("PlantClusterFoliage"), 1},
		};

        auto relationsPath = std::filesystem::path (RESOURCE_PATH_TEST) / "test_relations_map_02.txt";
		setup(allowedTypes, walkableAllowedTypes, relationsPath);
		startPossibleTypes[(toFI("NoFoliage"))] =
			possibleTypes[(toFI("NoFoliage"))];

		neighbourBonusList[(toFI("NoFoliage"))] = FoliageNeighbourBonus({
			std::pair<int, int>(toFI("NoFoliage"), 120),
			std::pair<int, int>(toFI("TreeFoliage"), 40),
		});
    }
	std::unordered_map<int, int> allowedTypes;
	std::unordered_map<int, int> walkableAllowedTypes;
};


LevelValues create_test_levelvalues() {
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

TEST_CASE("Foliage processor works correctly.", "[foliage_processor]") {

    std::srand(10);

    bool verboseLogging = false;
    TestBiomeFoliage biomeInfo = TestBiomeFoliage();

    FoliageProcessor processor = FoliageProcessor(
        biomeInfo, create_test_levelvalues(), verboseLogging);

    auto results = processor.mark_foliage_nodes();
    bool success = results.second;
    auto matrix = results.first;

    SECTION("Foliage processor finished sucessfully.") {
        REQUIRE(success);
        CHECK(matrix.dim_a() == 18);
        CHECK(matrix.dim_b() == 18);

        CHECK(matrix[10][10] == 5);
        CHECK(matrix[0][0] == 5);
        CHECK(matrix[12][6] == 5);
        CHECK(matrix[12][6] == 5);
        CHECK(matrix[5][5] == 1);
        CHECK(matrix[10][17] == 1);
    }
}
