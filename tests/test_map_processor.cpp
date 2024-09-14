#include <catch2/catch_test_macros.hpp>

#include "map_processor.h"
#include "biome_foliage_info.h"
#include "matrix.h"


class TestBiomeFoliage : public BiomeFoliageInfo {
    public:
    TestBiomeFoliage(FoliageDefinitions& fd) : BiomeFoliageInfo(fd) {

		allowedTypes = {
			{fd.toFI("NoFoliage"), 40},
			{fd.toFI("TreeFoliage"), 90},
			{fd.toFI("TreeSmallFoliage"), 10},
            {fd.toFI("FlowerClusterFoliage"), 2},
			{fd.toFI("PlantClusterFoliage"), 1},
		};
		walkableAllowedTypes = {
            {fd.toFI("NoFoliage"), 40},
			{fd.toFI("TreeSmallFoliage"), 2},
			{fd.toFI("FlowerClusterFoliage"), 2},
			{fd.toFI("PlantClusterFoliage"), 1},
		};

        auto relationsPath = std::filesystem::path (RESOURCE_PATH_TEST) / "test_relations_map_02.txt";
		Setup(allowedTypes, walkableAllowedTypes, fd, relationsPath);
		startFoliagePriority[(fd.toFI("NoFoliage"))] =
			foliagePriority[(fd.toFI("NoFoliage"))];

        const size_t foliageCount = fd.GetFoliageCount();
		auto tempList = std::vector<std::vector<std::pair<FoliageType, int>>>(foliageCount);
		tempList[(fd.toFI("NoFoliage"))] = {
			std::pair<int, int>(fd.toFI("NoFoliage"), 120),
			std::pair<int, int>(fd.toFI("TreeFoliage"), 40),
		};

        neighbourBonusList = FoliageSquashedList(tempList);
    }
	std::unordered_map<FoliageType, int> allowedTypes;
	std::unordered_map<FoliageType, int> walkableAllowedTypes;
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

    auto fd = FoliageDefinitions();

    bool verboseLogging = false;
    TestBiomeFoliage biomeInfo = TestBiomeFoliage(fd);

    MapProcessor mapProcessor (
        biomeInfo,
        create_test_levelvalues(),
        verboseLogging);
    
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
