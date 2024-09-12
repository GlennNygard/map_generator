#include <catch2/catch_test_macros.hpp>

#include "biome_foliage_info.h"
#include "foliage_definitions.h"


class TestBiomeFoliage : public BiomeFoliageInfo {
    public:
    TestBiomeFoliage() {

		allowedTypes = {
			{toFI("NoFoliage"), 40},
			{toFI("TreeFoliage"), 90},
			{toFI("TreeSmallFoliage"), 10},
			{toFI("TreeCliffedge"), 30},
		};

		walkableAllowedTypes = {
            {toFI("NoFoliage"), 40},
			{toFI("TreeFoliage"), 10},
			{toFI("TreeSmallFoliage"), 2},
			{toFI("FlowerClusterFoliage"), 2},
			{toFI("PlantClusterFoliage"), 1},
		};

        auto relationsPath = std::filesystem::path (RESOURCE_PATH_TEST) / "test_relations_map.txt";
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

    TestBiomeFoliage biomeInfo = TestBiomeFoliage();
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
