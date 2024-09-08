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


DiskManager::DiskManager() {

    _relationalMapPath = std::filesystem::path (RESOURCE_PATH);
    _mapsPath = std::filesystem::path (OUTPUT_PATH) / "maps";
    _mapsThumbnailsPath = std::filesystem::path (OUTPUT_PATH) / "thumbnails";

    // Create reverse dict.
    _foliageMapMapping = std::unordered_map<FoliageType, int>();
    _mapFoliageMapping = std::unordered_map<int, FoliageType>();
    auto foliageDefs = FoliageDefinitions::get_foliage_definitions();
    for(int i = 0; i < foliageDefs.size(); i++) {
        auto foliageInfo = foliageDefs[i];
        if(!foliageInfo.containsResourceData) {
            continue;
        }
        _foliageMapMapping[static_cast<FoliageType>(i)] = foliageInfo.mapIndex;
        _mapFoliageMapping[foliageInfo.mapIndex] = static_cast<FoliageType>(i);
    }
}


void DiskManager::save_map(
        MapObject mapObject,
        std::string mapName,
        std::string mapNamePrefix,
        int currentIndex) {
    std::string mapPath = get_map_path(mapNamePrefix);

    std::filesystem::path finalPath(mapPath);

    std::cout << std::format(
        "Saving map {0} to path: {1}",
        mapName, finalPath.generic_string()) << std::endl;

    perform_map_save(mapObject.map, finalPath, mapName);
    std::cout << std::format(
        "Save completed! Map: {0}",
        std::to_string(currentIndex)) << std::endl;
}

void DiskManager::perform_map_save(
        Matrix<MapNode> map,
        std::filesystem::path directoryPath,
        std::string mapName) {

    if(!get_path_exists(directoryPath)) {
        std::filesystem::create_directories(directoryPath);
    }

    std::filesystem::path finalPath = directoryPath /= (mapName+".txt");

    std::vector<std::string> content (
        map.dim_a() * map.dim_b() + DiskManager::INITIAL_OFFSET);
    content[0] = std::to_string(map.dim_a());
    content[1] = std::to_string(map.dim_b());

    int i = 0;
    for(int y = 0; y < map.dim_b(); y++) {
        for(int x = 0; x < map.dim_a(); x++) {

            MapNode mapNode = map[x][y];

            std::string nodeData =
                std::to_string(node_data_to_map_type(
                    mapNode.nodeType,
                    mapNode.nodeBiome)) +
                DiskManager::CONTENT_SEPARATOR +
                "f" + std::to_string(get_foliage_map_mapping()[mapNode.foliageType]) +
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

    std::string saveString = join(content, DiskManager::SEPARATOR);
    write_to_file(saveString, finalPath);
}

void DiskManager::write_to_file(std::string data, std::string path) {
    std::ofstream out(path);
    out << data;
    out.close();
}

int DiskManager::node_data_to_map_type(int nodeType, LevelBiome nodeBiome) {
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

std::string DiskManager::get_map_name(std::string mapNamePrefix, int currentIndex) {
    std::string mapName = std::format(
        "{}_{:03}", mapNamePrefix, currentIndex);
    return mapName;
}

std::string DiskManager::get_map_prefix(
        ArgValues argValues,
        LevelBiome mapBiome) {
    std::string mapNamePrefix = argValues.mapNamePrefix;
    if(mapNamePrefix.empty()) {
        mapNamePrefix = get_map_name_prefix(
            mapBiome, argValues.mapSize);
    }
    return mapNamePrefix;
}

std::string DiskManager::join(const std::vector<std::string> &lst, const std::string &delim) {
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret += s;
    }
    return ret;
}

std::vector<std::string> DiskManager::split(std::string str, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
        token = str.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (str.substr (pos_start));
    return res;
}

Matrix<MapNode> DiskManager::load_map(
        std::filesystem::path mapPath) {

    if(!get_path_exists(mapPath)) {
        return Matrix<MapNode>();
    }

    // Open the file using ifstream.
    std::ifstream file(mapPath);

    // Confirm file opening.
    if(!file.is_open()) {
        // print error message and return
        std::cerr << "Failed to open file: " << mapPath << std::endl;
        return Matrix<MapNode>();
    }

    // Read the file line by line into a string.
    std::string loadText = "";
    std::string line;
    while(getline(file, line)) {
        // std::cout << line << std::endl;
        loadText += line;
    }

    // Close the file.
    file.close();
    return convert_string_to_map(loadText);
}

std::string DiskManager::get_base_map_path() {
    return _mapsPath;
}

std::string DiskManager::get_map_path(std::string mapName) {
    std::filesystem::path path (_mapsPath);
    path /= (mapName);
    return path.generic_string();
}

std::string DiskManager::get_map_path(LevelBiome biome, MapSize size) {
    std::string mapName = get_map_name_prefix(biome, size);
    std::filesystem::path path (_mapsPath);
    path /= (mapName);
    return path.generic_string();
}

std::string DiskManager::get_relational_map_path(std::string mapName) {
    std::filesystem::path path (_relationalMapPath);
    path /= (mapName);
    return path.generic_string();
}

std::string DiskManager::get_map_name_prefix(LevelBiome biome, MapSize size) {
    std::string mapNamePrefix = std::format(
        "{}-{}", BIOME_MAPNAME_DICT.at(biome), MAP_SIZE_NAME_DICT.at(size));
    return mapNamePrefix;
}

Matrix<MapNode> DiskManager::convert_string_to_map(std::string loadText) {

    // Example formatting:
    // 512,512,1-10,0-22,1-4

    std::vector<std::string> content = split(loadText, ",");

    int lengthX = std::stoi(content[0]);
    int lengthY = std::stoi(content[1]);

    logger::log(std::format(
        "Loading map of size {0}x{1}.", lengthX, lengthY));

    Matrix<MapNode> map (lengthX, lengthY);

    int i = 0;
    for(int y = 0; y < map.dim_b(); y++) {
        for(int x = 0; x < map.dim_a(); x++) {

            auto mapNode = MapNode();

            auto nodeData = split(
                content[i + DiskManager::INITIAL_OFFSET],
                DiskManager::CONTENT_SEPARATOR);

            if(x == lengthX) {
                // nodeData[1].Replace("\n", "");
                nodeData[1] = std::regex_replace(nodeData[1], std::regex("\\n"), "");
            }

            // std::string foliageString = nodeData[1].Replace("f", "");
            std::string foliageString = std::regex_replace(nodeData[1], std::regex("f"), "");
            int mapType = std::stoi(nodeData[0]);
            auto _mapNodeTypeMapping = get_map_node_type_mapping();
            mapNode.nodeType = _mapNodeTypeMapping.at(mapType);
            if(nodeData.size() == 2) {
                mapNode.nodeBiome = _mapNodeBiomeMapping[mapType];
            }

            else if(nodeData.size() == 3) {
                // std::string biomeString = nodeData[2].Replace("b", "");
                std::string biomeString = std::regex_replace(nodeData[2], std::regex("b"), "");
                mapNode.nodeBiome = (LevelBiome)std::stoi(biomeString);
            }

            try {
                int foliageIndex = std::stoi(foliageString);

                auto iter = _mapFoliageMapping.find(foliageIndex);
                // if(_mapFoliageMapping.TryGetValue(int.Parse(foliageString), out FoliageType ft)) {
                if(iter != _mapFoliageMapping.end()) {
                    FoliageType ft = iter->second;
                    mapNode.foliageType = ft;
                }
                else {
                    std::cout <<
                        "_mapFoliageMapping is missing the map key: "+foliageString << std::endl;
                }
            }
            catch(const std::invalid_argument& ia) {
                std::cerr << "Cannot convert the following value to int: "+foliageString << std::endl;
                mapNode.foliageType = FoliageType::Foliage_NoFoliage;
            }

            map[x][y] = mapNode;
            i++;
        }
    }

    return map;
}

std::optional<MapObject> DiskManager::load_map_object(std::string resourcePath) {
    
    auto map = load_map(resourcePath);
    if(map.empty()) {
        return std::nullopt;
    }
    MapObject mapObject = MapObject(map);
    return mapObject;
}

void DiskManager::save_map_thumbnail(
        Matrix<MapNode> fullMap, std::string mapName, std::string mapNamePrefix) {

    std::filesystem::path directoryPath = _mapsThumbnailsPath / mapNamePrefix;

    if(!get_path_exists(directoryPath)) {
        std::filesystem::create_directories(directoryPath);
    }

    const int thumbnailSizeMultiplier = 4;

    const size_t fullSizeX = fullMap.dim_a() * thumbnailSizeMultiplier;
    const size_t fullSizeY = fullMap.dim_b() * thumbnailSizeMultiplier;

    const int channelNum = 3;

    // Color32[] mapColours = new Color32[fullSizeX * fullSizeY];

    std::vector<unsigned char> mapColours (fullSizeX * fullSizeX * channelNum);

    for(int y = 0; y < fullMap.dim_b(); y++) {
        for(int x = 0; x < fullMap.dim_a(); x++) {

            auto mapNode = fullMap[x][y];

            FoliageInfo foliageInfo;
            // We only filter here for speed.
            if(mapNode.foliageType != FoliageType::Foliage_NoFoliage && mapNode.foliageType != FoliageType::Foliage_NoSelection) {
                foliageInfo = FoliageDefinitions::get_foliage_definitions()[(int)mapNode.foliageType];
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
