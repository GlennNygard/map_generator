#include <catch2/catch_test_macros.hpp>

#include "biome_info.h"
#include "foliage_definitions.h"


class TestBiome_BiomeInfo : public BiomeInfo<TestBiome_BiomeInfo> {
    public:

    DefinitionsArray t_upRelations;
    DefinitionsArray t_downRelations;
    DefinitionsArray t_leftRelations;
    DefinitionsArray t_rightRelations;

    DefinitionsArray t_upLeftRelations;
    DefinitionsArray t_upRightRelations;
    DefinitionsArray t_downLeftRelations;
    DefinitionsArray t_downRightRelations;

    std::unordered_map<FoliageType, int> t_allowedTypes;
	std::unordered_map<FoliageType, int> t_walkableAllowedTypes;

    virtual void VFunc() {};
    TestBiome_BiomeInfo(FoliageDefinitions& fd) : BiomeInfo(fd) {

		t_allowedTypes = {
			{fd.ToFI("NoFoliage"), 40},
			{fd.ToFI("TreeFoliage"), 90},
			{fd.ToFI("TreeSmallFoliage"), 10},
			{fd.ToFI("TreeCliffedge"), 30},
		};

		t_walkableAllowedTypes = {
            {fd.ToFI("NoFoliage"), 40},
			{fd.ToFI("TreeFoliage"), 10},
			{fd.ToFI("TreeSmallFoliage"), 2},
			{fd.ToFI("FlowerClusterFoliage"), 2},
			{fd.ToFI("PlantClusterFoliage"), 1},
		};

        auto relationsPath = std::filesystem::path (RESOURCE_PATH_TEST) / "test_relations_map.txt";
		Setup(t_allowedTypes, t_walkableAllowedTypes, fd, relationsPath);
		m_startFoliagePriority[(fd.ToFI("NoFoliage"))] =
			m_foliagePriority[(fd.ToFI("NoFoliage"))];

        const size_t foliageCount = fd.GetFoliageCount();
        auto tempList = std::vector<std::vector<std::pair<FoliageType, int>>>(foliageCount);
		tempList[(fd.ToFI("NoFoliage"))] = {
			std::pair<int, int>(fd.ToFI("NoFoliage"), 120),
			std::pair<int, int>(fd.ToFI("TreeFoliage"), 40),
		};

        neighbourBonusList = DefaultFoliageSquashedList(tempList);

        t_upRelations = m_upRelations;
        t_downRelations = m_downRelations;
        t_leftRelations = m_leftRelations;
        t_rightRelations = m_rightRelations;

        t_upLeftRelations = m_upLeftRelations;
        t_upRightRelations = m_upRightRelations;
        t_downLeftRelations = m_downLeftRelations;
        t_downRightRelations = m_downRightRelations;
    }
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
    TestBiome_BiomeInfo biomeInfo = TestBiome_BiomeInfo(fd);
};

TEST_CASE_PERSISTENT_FIXTURE(BiomeFixture, "Biome foliage relations correct.", "[biome_foliage]") {

    SECTION("Default data set correctly.") {
        CHECK( biomeInfo.t_allowedTypes.size() == 4 );
        CHECK( biomeInfo.t_walkableAllowedTypes.size() == 5 );
    }

    SECTION("Relational sum correct.") {
        CHECK( count_relational_value(biomeInfo.t_upRelations) == 134 );
        CHECK( count_relational_value(biomeInfo.t_downRelations) == 170 );
        CHECK( count_relational_value(biomeInfo.t_leftRelations) == 96 );
        CHECK( count_relational_value(biomeInfo.t_rightRelations) == 56 );

        CHECK( count_relational_value(biomeInfo.t_upLeftRelations) == 96 );
        CHECK( count_relational_value(biomeInfo.t_upRightRelations) == 94 );
        CHECK( count_relational_value(biomeInfo.t_downLeftRelations) == 97 );
        CHECK( count_relational_value(biomeInfo.t_downRightRelations) == 56 );
    }
}
