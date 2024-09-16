#pragma once

#include "biome_info.h"


class TestingBiomeInfo : public BiomeInfo<TestingBiomeInfo> {
    public:

    virtual void VFunc() {};
    TestingBiomeInfo(FoliageDefinitions& fd) : BiomeInfo<TestingBiomeInfo>(fd) {

		std::unordered_map<FoliageType, int> allowedTypes = {
			{fd.ToFI("NoFoliage"), 40},
			{fd.ToFI("TreeFoliage"), 90},
			{fd.ToFI("TreeSmallFoliage"), 10},
			{fd.ToFI("TreeCliffedge"), 30},
		};

		std::unordered_map<FoliageType, int> walkableAllowedTypes = {
            {fd.ToFI("NoFoliage"), 40},
			{fd.ToFI("TreeFoliage"), 10},
			{fd.ToFI("TreeSmallFoliage"), 2},
			{fd.ToFI("FlowerClusterFoliage"), 2},
			{fd.ToFI("PlantClusterFoliage"), 1},
		};
		#ifdef RESOURCE_PATH_TEST
		auto relationsPath = std::filesystem::path (RESOURCE_PATH_TEST) / "test_relations_map.txt";
		#else
        auto relationsPath = std::filesystem::path (RESOURCE_PATH) / "GrassRelationsMap.txt";
		#endif
		Setup(allowedTypes, walkableAllowedTypes, fd, relationsPath);
		m_startFoliagePriority[(fd.ToFI("NoFoliage"))] =
			m_foliagePriority[(fd.ToFI("NoFoliage"))];

        const size_t foliageCount = fd.GetFoliageCount();
        auto tempList = std::vector<std::vector<std::pair<FoliageType, int>>>(foliageCount);
		tempList[(fd.ToFI("NoFoliage"))] = {
			std::pair<int, int>(fd.ToFI("NoFoliage"), 120),
			std::pair<int, int>(fd.ToFI("TreeFoliage"), 40),
		};

        neighbourBonusList = DefaultFoliageSquashedList(tempList);
    }
};
