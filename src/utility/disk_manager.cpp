#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <format>
#include <regex>
#include <expected>

#include "stb_image_write.h"

#include "disk_manager.h"
#include "foliage.h"
#include "foliage_definitions.h"
#include "logger.h"


const std::string DiskManager::SEPARATOR = ",";
const std::string DiskManager::CONTENT_SEPARATOR = "-";


/**
 * These are use for map names when storing/getting maps to/from disk.
 */
const std::unordered_map<MapSize, std::string> DiskManager::MAP_SIZE_NAME_DICT = {
    {MapSize::MapSize_Small, "Small"},
    {MapSize::MapSize_Medium, "Medium"},
    {MapSize::MapSize_Large, "Large"},
    {MapSize::MapSize_VeryLarge, "VeryLarge"},
    {MapSize::MapSize_IncrediblyLarge, "IncrediblyLarge"},
};

const std::unordered_map<MapSize, std::string> DiskManager::MAP_SIZE_ADDRESSABLE_NAME_DICT = {
    {MapSize::MapSize_Small, "small"},
    {MapSize::MapSize_Medium, "medium"},
    {MapSize::MapSize_Large, "large"},
    {MapSize::MapSize_VeryLarge, "very_large"},
    {MapSize::MapSize_IncrediblyLarge, "incredibly_large"},
};

const std::unordered_map<MapSize, Vector2Int> DiskManager::MAP_SIZE_NODE_COUNT_DICT = {
    {MapSize::MapSize_Small, {100,100}},
    {MapSize::MapSize_Medium, {140,140}},
    {MapSize::MapSize_Large, {180,180}},
    {MapSize::MapSize_VeryLarge, {220,220}},
    {MapSize::MapSize_IncrediblyLarge, {260,260}},
};

const std::unordered_map<LevelBiome, std::string> DiskManager::BIOME_MAPNAME_DICT = {
    {LevelBiome::Grass, "RandomGrassMap"},
    {LevelBiome::Snow, "RandomSnowMap"},
};

const std::unordered_map<LevelBiome, std::string> DiskManager::BIOME_ADDRESSABLE_LABEL_DICT = {
    {LevelBiome::Grass, "map_grass"},
    {LevelBiome::Snow, "map_snow"},
};

const std::unordered_map<LevelBiome, int> DiskManager::BIOME_MAPCOUNT_DICT = {
    {LevelBiome::Grass, 10},
    {LevelBiome::Snow, 100},
};


const std::unordered_map<int,int> DiskManager::MAP_NODE_TYPE_MAPPING = std::unordered_map<int,int> {
    {DiskManager::FLOOR_GENERIC_TYPE, FoliageHelpers::FLOOR_NODE_TYPE},
    {DiskManager::HIGH_GENERIC_TYPE, FoliageHelpers::HIGH_GROUND_NODE_TYPE},
    {DiskManager::BORDER_GENERIC_TYPE, FoliageHelpers::BORDER_NODE_TYPE},
    {DiskManager::LOW_GENERIC_TYPE, FoliageHelpers::LOW_GROUND_NODE_TYPE},

    {DiskManager::FLOOR_DESERT_TYPE, FoliageHelpers::FLOOR_NODE_TYPE},
    {DiskManager::HIGH_DESERT_TYPE, FoliageHelpers::HIGH_GROUND_NODE_TYPE},
    {DiskManager::BORDER_DESERT_TYPE, FoliageHelpers::BORDER_NODE_TYPE},
    {DiskManager::LOW_DESERT_TYPE, FoliageHelpers::LOW_GROUND_NODE_TYPE},
};

const std::unordered_map<int, LevelBiome> DiskManager::MAP_NODE_BIOME_MAPPING = std::unordered_map<int,LevelBiome> {
    {DiskManager::FLOOR_GENERIC_TYPE, LevelBiome::None},
    {DiskManager::HIGH_GENERIC_TYPE, LevelBiome::None},
    {DiskManager::BORDER_GENERIC_TYPE, LevelBiome::None},
    {DiskManager::LOW_GENERIC_TYPE, LevelBiome::None},

    {DiskManager::FLOOR_DESERT_TYPE, LevelBiome::Desert},
    {DiskManager::HIGH_DESERT_TYPE, LevelBiome::Desert},
    {DiskManager::BORDER_DESERT_TYPE, LevelBiome::Desert},
    {DiskManager::LOW_DESERT_TYPE, LevelBiome::Desert},
};


DiskManager::DiskManager(FoliageDefinitions& foliageDefinitions) {

    m_foliageDefinitions = std::make_unique<FoliageDefinitions>(foliageDefinitions);

    m_relationalMapPath = std::filesystem::path (RESOURCE_PATH);
    m_mapsPath = std::filesystem::path (OUTPUT_PATH) / "maps";
    m_mapsThumbnailsPath = std::filesystem::path (OUTPUT_PATH) / "thumbnails";

    // Create reverse dict.
    m_foliageMapMapping = std::unordered_map<FoliageType, int>();
    mapFoliageMapping = std::unordered_map<int, FoliageType>();
    auto foliageDefs = foliageDefinitions.foliageInfoElements;
    for(int i = 0; i < foliageDefs.size(); i++) {
        auto foliageInfo = foliageDefs[i];
        if(!foliageInfo.containsResourceData) {
            continue;
        }
        m_foliageMapMapping[foliageInfo.foliageType] = foliageInfo.mapIndex;
        mapFoliageMapping[foliageInfo.mapIndex] = foliageInfo.foliageType;
    }
}



const std::string DiskManager::GetMapName(
        const std::string& mapNamePrefix, const int currentIndex) {
    std::string mapName = std::format(
        "{}_{:03}", mapNamePrefix, currentIndex);
    return mapName;
}

const std::string DiskManager::GetMapPrefix(
        const argparse::ArgValues& argValues,
        const LevelBiome mapBiome) {
    std::string mapNamePrefix = argValues.mapNamePrefix;
    if(mapNamePrefix.empty()) {
        mapNamePrefix = GetMapNamePrefix(
            mapBiome, argValues.mapSize);
    }
    return mapNamePrefix;
}

std::string DiskManager::Join(const std::vector<std::string> &lst, const std::string &delim) {
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret += s;
    }
    return ret;
}

std::vector<std::string> DiskManager::Split(
        const std::string& str, const std::string& delimiter) {
    size_t posStart = 0, possEnd, delimLen = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((possEnd = str.find(delimiter, posStart)) != std::string::npos) {
        token = str.substr (posStart, possEnd - posStart);
        posStart = possEnd + delimLen;
        res.push_back (token);
    }

    res.push_back (str.substr (posStart));
    return res;
}


void DiskManager::SaveMap(
        const MapObject& mapObject,
        const std::string& mapName,
        const std::string& mapNamePrefix,
        const int currentIndex) {
    std::string mapPath = GetMapPath(mapNamePrefix);

    std::filesystem::path finalPath(mapPath);

    std::cout << std::format(
        "Saving map {0} to path: {1}",
        mapName, finalPath.generic_string()) << std::endl;

    PerformMapSave(mapObject.map, finalPath, mapName);
    std::cout << std::format(
        "Save completed! Map index: {0}",
        std::to_string(currentIndex)) << std::endl;
}

void DiskManager::PerformMapSave(
        const Matrix<MapNode>& map,
        const std::filesystem::path& directoryPath,
        const std::string& mapName) const {

    if(!GetPathExists(directoryPath)) {
        std::filesystem::create_directories(directoryPath);
    }

    std::filesystem::path finalPath = directoryPath / (mapName+".txt");

    std::vector<std::string> content (
        map.dim_a() * map.dim_b() + DiskManager::INITIAL_OFFSET);
    content[0] = std::to_string(map.dim_a());
    content[1] = std::to_string(map.dim_b());

    int i = 0;
    for(int y = 0; y < map.dim_b(); y++) {
        for(int x = 0; x < map.dim_a(); x++) {

            MapNode mapNode = map[x][y];

            std::string nodeData =
                std::to_string(NodeDataToMapType(
                    mapNode.nodeType,
                    mapNode.nodeBiome)) +
                DiskManager::CONTENT_SEPARATOR +
                "f" + std::to_string(m_foliageMapMapping.at(mapNode.foliageType)) +
                std::format(
                    "{0}{1}{2}", DiskManager::CONTENT_SEPARATOR,
                    "b", static_cast<int>(mapNode.nodeBiome));

            if(x == map.dim_a()-1) {
                nodeData += "\n";
            }

            content[i+DiskManager::INITIAL_OFFSET] = nodeData;
            i++;
        }
    }

    std::string saveString = Join(content, DiskManager::SEPARATOR);
    WriteToFile(saveString, finalPath);
}

void DiskManager::WriteToFile(const std::string& data, const std::string& path) const {
    std::ofstream out(path);
    out << data;
    out.close();
}

const int DiskManager::NodeDataToMapType(const int nodeType, const LevelBiome nodeBiome) const {
    if(nodeType == FoliageHelpers::FLOOR_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return FLOOR_GENERIC_TYPE;
    }
    if(nodeType == FoliageHelpers::BORDER_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return BORDER_GENERIC_TYPE;
    }
    if(nodeType == FoliageHelpers::HIGH_GROUND_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return HIGH_GENERIC_TYPE;
    }
    if(nodeType == FoliageHelpers::LOW_GROUND_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return LOW_GENERIC_TYPE;
    }

    if(nodeType == FoliageHelpers::FLOOR_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return FLOOR_DESERT_TYPE;
    }
    if(nodeType == FoliageHelpers::BORDER_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return BORDER_DESERT_TYPE;
    }
    if(nodeType == FoliageHelpers::HIGH_GROUND_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return HIGH_DESERT_TYPE;
    }
    if(nodeType == FoliageHelpers::LOW_GROUND_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return LOW_DESERT_TYPE;
    }

    return DiskManager::FLOOR_GENERIC_TYPE;
}


Matrix<MapNode> DiskManager::LoadMap(const std::filesystem::path& mapPath) {

    if(!GetPathExists(mapPath)) {
        return Matrix<MapNode>();
    }

    // Open the file using ifstream.
    std::ifstream file(mapPath);

    // Confirm file opening.
    if(!file.is_open()) {
        // Print error message and return
        logger::LogError("Failed to open file: " + mapPath.generic_string());
        return Matrix<MapNode>();
    }

    // Read the file line by line into a string.
    std::string loadText = "";
    std::string line;
    while(getline(file, line)) {
        loadText += line;
    }

    // Close the file.
    file.close();
    return ConvertStringToMap(loadText);
}

const std::string DiskManager::GetBaseMapPath() const {
    return m_mapsPath;
}

const std::string DiskManager::GetMapPath(std::string mapName) const {
    std::filesystem::path path (m_mapsPath);
    path /= (mapName);
    return path.generic_string();
}

const std::string DiskManager::GetMapPath(LevelBiome biome, MapSize size) const {
    const std::string mapName = GetMapNamePrefix(biome, size);
    std::filesystem::path path (m_mapsPath);
    path /= (mapName);
    return path.generic_string();
}

const std::string DiskManager::GetRelationalMapPath(const std::string& mapName) {
    std::filesystem::path path (RESOURCE_PATH);
    path /= (mapName);
    return path.generic_string();
}

const bool DiskManager::GetPathExists(const std::string& loadPath) {
    return std::filesystem::exists(loadPath);
}

const std::string DiskManager::GetMapNamePrefix(LevelBiome biome, MapSize size) {
    std::string mapNamePrefix = std::format(
        "{}-{}", BIOME_MAPNAME_DICT.at(biome), MAP_SIZE_NAME_DICT.at(size));
    return mapNamePrefix;
}

Matrix<MapNode> DiskManager::ConvertStringToMap(const std::string& loadText) {

    // Example formatting:
    // 512,512,1-10,0-22,1-4

    std::vector<std::string> content = Split(loadText, ",");

    int lengthX = std::stoi(content[0]);
    int lengthY = std::stoi(content[1]);

    logger::Log(std::format(
        "Loading map of size {0}x{1}.", lengthX, lengthY));

    Matrix<MapNode> map (lengthX, lengthY);

    int i = 0;
    for(int y = 0; y < map.dim_b(); y++) {
        for(int x = 0; x < map.dim_a(); x++) {

            auto mapNode = MapNode();

            auto nodeData = Split(
                content[i + DiskManager::INITIAL_OFFSET],
                DiskManager::CONTENT_SEPARATOR);

            if(x == lengthX) {
                nodeData[1] = std::regex_replace(nodeData[1], std::regex("\\n"), "");
            }

            std::string foliageString = std::regex_replace(nodeData[1], std::regex("f"), "");
            const int mapType = std::stoi(nodeData[0]);
            auto& mapNodeTypeMapping = MAP_NODE_TYPE_MAPPING;
            mapNode.nodeType = mapNodeTypeMapping.at(mapType);
            if(nodeData.size() == 2) {
                mapNode.nodeBiome = MAP_NODE_BIOME_MAPPING.at(mapType);
            }

            else if(nodeData.size() == 3) {
                std::string biomeString = std::regex_replace(nodeData[2], std::regex("b"), "");
                mapNode.nodeBiome = (LevelBiome)std::stoi(biomeString);
            }

            try {
                int mapFoliageIndex = std::stoi(foliageString);

                auto iter = mapFoliageMapping.find(mapFoliageIndex);
                if(iter != mapFoliageMapping.end()) {
                    FoliageType ft = iter->second;
                    mapNode.foliageType = ft;
                }
                else {
                    logger::Log(
                        "m_mapFoliageMapping is missing the map key: "+foliageString);
                }
            }
            catch(const std::invalid_argument& ia) {
                logger::LogError("Cannot convert the following value to int: "+foliageString);
                mapNode.foliageType = FoliageHelpers::NO_FOLIAGE_INDEX;
            }

            map[x][y] = mapNode;
            i++;
        }
    }

    return map;
}

std::optional<MapObject> DiskManager::LoadMapObject(const std::string& resourcePath) {
    
    auto map = LoadMap(resourcePath);
    if(map.empty()) {
        return std::nullopt;
    }
    MapObject mapObject = MapObject(map);
    return mapObject;
}

void DiskManager::SaveMapThumbnail(
        const Matrix<MapNode>& fullMap,
        const std::string& mapName,
        const std::string& mapNamePrefix) {

    std::filesystem::path directoryPath = m_mapsThumbnailsPath / mapNamePrefix;

    if(!GetPathExists(directoryPath)) {
        std::filesystem::create_directories(directoryPath);
    }

    const int thumbnailSizeMultiplier = 4;

    const size_t fullSizeX = fullMap.dim_a() * thumbnailSizeMultiplier;
    const size_t fullSizeY = fullMap.dim_b() * thumbnailSizeMultiplier;

    const int channelNum = 3;

    std::vector<unsigned char> mapColours (fullSizeX * fullSizeX * channelNum);

    for(int y = 0; y < fullMap.dim_b(); y++) {
        for(int x = 0; x < fullMap.dim_a(); x++) {

            auto mapNode = fullMap[x][y];

            FoliageInfo foliageInfo;
            // We only filter here for speed.
            if(mapNode.foliageType != FoliageHelpers::NO_FOLIAGE_INDEX && mapNode.foliageType != FoliageHelpers::NO_SELECTION_INDEX) {
                foliageInfo = m_foliageDefinitions->foliageInfoElements[mapNode.foliageType];
            }

            Colour finalCol = foliageInfo.nodeColour;

            for(int pixelY = 0; pixelY < thumbnailSizeMultiplier; pixelY++) {
                for(int pixelX = 0; pixelX < thumbnailSizeMultiplier; pixelX++) {
                    int index = static_cast<int>(
                        ((y*thumbnailSizeMultiplier+pixelY) * fullSizeX +
                            (x*thumbnailSizeMultiplier+pixelX)
                        ) * channelNum);
                    mapColours[index] = static_cast<unsigned char>(finalCol.r);
                    mapColours[index+1] = static_cast<unsigned char>(finalCol.g);
                    mapColours[index+2] = static_cast<unsigned char>(finalCol.b);
                }
            }
        }
    }

    std::filesystem::path finalPath = directoryPath / (mapName+".jpg");

    std::cout << "Saving thumbnail to: "+finalPath.generic_string() << std::endl;

    // Write the image to a file (RGB).
    int result = stbi_write_jpg(
        finalPath.c_str(),
        fullSizeX, fullSizeY, channelNum, &mapColours[0],
        fullSizeX*channelNum);

    if(result == 0) {
        std::cerr << "Thumbnail save failed: "+mapName << std::endl;
    }
    else {
        std::cout << "Thumbnail saved successfully: "+mapName << std::endl;
    }
}
