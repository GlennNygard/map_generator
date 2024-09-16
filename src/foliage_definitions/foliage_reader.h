#pragma once

#include <string>
#include <filesystem>
#include <format>

#include "yaml-cpp/yaml.h"

#include "foliage.h"
#include "foliage_info.h"
#include "logger.h"


namespace foliagereader {

    FoliageInfo read_node_data(YAML::Node node) {
        FoliageInfo info = FoliageInfo();

        if(node["mapIndex"]) {
            info.SetMapIndex(node["mapIndex"].as<int>());
        }
        if(node["walkable"]) {
            info.SetWalkable(node["walkable"].as<bool>());
        }
        if(node["buildable"]) {
            info.SetBuildable(node["buildable"].as<bool>());
        }
        if(node["colour"]) {
            auto colour = node["colour"];
            info.SetColour(Colour(
                colour[0].as<float>(),
                colour[1].as<float>(),
                colour[2].as<float>()));
        }

        if(node["nodeBiome"]) {
            std::string nodeBiome = node["nodeBiome"].as<std::string>();
            if(nodeBiome == "None") {
                info.SetNodeBiome(LevelBiome::None);
            }
            if(nodeBiome == "Grass") {
                info.SetNodeBiome(LevelBiome::Grass);
            }
            if(nodeBiome == "Desert") {
                info.SetNodeBiome(LevelBiome::Desert);
            }
            if(nodeBiome == "Snow") {
                info.SetNodeBiome(LevelBiome::Snow);
            }
        }

        return info;
    }

    const std::vector<int> read_direction_set(YAML::Node node) {
        std::vector<int> vec = {};
        for(auto itr = node.begin(); itr != node.end(); itr++) {
            int mapIndex = itr->second.as<int>();
            vec.push_back(mapIndex);
        }
        return vec;
    }

    int read(
            std::vector<FoliageInfo> &mainFoliageList,
            const std::filesystem::path& dataPath,
            int mapIndexStart,
            int currentIndex) {

        YAML::Node data = YAML::LoadFile(dataPath);

        if(!data["Foliage"]) {
            return currentIndex;
        }

        std::vector<int> directionSet;
        bool hasDirectionSet = false;
        if(data["DirectionSets"]) {
            directionSet = read_direction_set(data["DirectionSets"].begin()->second);
            hasDirectionSet = true;
        }

        auto foliageList = data["Foliage"];
        for(auto itr = foliageList.begin(); itr != foliageList.end(); ++itr) {

            if(currentIndex >= FoliageHelpers::MAX_FOLIAGE_COUNT) {
                logger::LogError(
                    "Foliage index is higher than max foliage count.");
                break;
            }

            std::string foliageName = itr->first.as<std::string>();
            auto foliageData = itr->second;

            FoliageInfo info = read_node_data(foliageData);
            info.SetFoliageType(currentIndex);
            info.SetFoliageName(foliageName);
            info.SetMapIndex(info.mapIndex + mapIndexStart);
            currentIndex++;

            if(foliageData["directionSet"] && hasDirectionSet) {
                info.SetDirectionSet(directionSet);
            }

            mainFoliageList.push_back(info);
        }

        return currentIndex;
    }
}
