#include <catch2/catch_test_macros.hpp>

#include "biome_foliage_info.h"
#include "foliage_definitions.h"


class TestBiomeFoliage : public BiomeFoliageInfo {
    public:
    TestBiomeFoliage(FoliageDefinitions& fd) : BiomeFoliageInfo(fd) {

		allowedTypes = {
			{fd.toFI("NoFoliage"), 40},
			{fd.toFI("TreeFoliage"), 90},
			{fd.toFI("TreeSmallFoliage"), 10},
			{fd.toFI("TreeCliffedge"), 30},
		};

		walkableAllowedTypes = {
            {fd.toFI("NoFoliage"), 40},
			{fd.toFI("TreeFoliage"), 10},
			{fd.toFI("TreeSmallFoliage"), 2},
			{fd.toFI("FlowerClusterFoliage"), 2},
			{fd.toFI("PlantClusterFoliage"), 1},
		};

        auto relationsPath = std::filesystem::path (RESOURCE_PATH_TEST) / "test_relations_map.txt";
		setup(allowedTypes, walkableAllowedTypes, fd, relationsPath);
		startFoliagePriority[(fd.toFI("NoFoliage"))] =
			foliagePriority[(fd.toFI("NoFoliage"))];

        const size_t foliageCount = fd.get_foliage_count();
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


int count_relational_value(std::array<FoliageRelation, FoliageHelpers::MAX_FOLIAGE_COUNT> arr) {
    int valueSum = 0;
    for(auto relations : arr) {
        for(size_t i = 0; i < relations.size(); i++) {
            valueSum += relations[i];
        }
    }
    return valueSum;
}

struct BiomeFixture {

    FoliageDefinitions fd = FoliageDefinitions();
    TestBiomeFoliage biomeInfo = TestBiomeFoliage(fd);
};


TEST_CASE_PERSISTENT_FIXTURE(BiomeFixture, "Biome foliage relations correct.", "[biome_foliage]") {

    SECTION("Default data set correctly.") {
        CHECK( biomeInfo.allowedTypes.size() == 4 );
        CHECK( biomeInfo.walkableAllowedTypes.size() == 5 );
    }

    SECTION("Relational sum correct.") {
        CHECK( count_relational_value(biomeInfo.upRelations) == 134 );
        CHECK( count_relational_value(biomeInfo.downRelations) == 170 );
        CHECK( count_relational_value(biomeInfo.leftRelations) == 96 );
        CHECK( count_relational_value(biomeInfo.rightRelations) == 56 );

        CHECK( count_relational_value(biomeInfo.upLeftRelations) == 96 );
        CHECK( count_relational_value(biomeInfo.upRightRelations) == 94 );
        CHECK( count_relational_value(biomeInfo.downLeftRelations) == 97 );
        CHECK( count_relational_value(biomeInfo.downRightRelations) == 56 );
    }
}
