#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <format>
#include <regex>
#include <expected>

#include "external/stb_image_write.h"

#include "mapdisk_manager.h"
#include "foliage/foliage.h"
#include "foliage_definitions/foliage_definitions.h"


MapDiskManager::MapDiskManager() {

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

/// <summary>
/// This is messy and should be improved in the future.
/// </summary>
/// <param name="nodeType"></param>
/// <param name="nodeBiome"></param>
/// <returns></returns>
int NodeDataToMapType(int nodeType, LevelBiome nodeBiome) {
    if(nodeType == FoliageHelpers::FLOOR_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return MapDiskManager::FLOOR_GENERIC_TYPE;
    }
    if(nodeType == FoliageHelpers::BORDER_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return MapDiskManager::BORDER_GENERIC_TYPE;
    }
    if(nodeType == FoliageHelpers::HIGH_GROUND_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return MapDiskManager::HIGH_GENERIC_TYPE;
    }
    if(nodeType == FoliageHelpers::LOW_GROUND_NODE_TYPE && nodeBiome == LevelBiome::None) {
        return MapDiskManager::LOW_GENERIC_TYPE;
    }

    if(nodeType == FoliageHelpers::FLOOR_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return MapDiskManager::FLOOR_DESERT_TYPE;
    }
    if(nodeType == FoliageHelpers::BORDER_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return MapDiskManager::BORDER_DESERT_TYPE;
    }
    if(nodeType == FoliageHelpers::HIGH_GROUND_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return MapDiskManager::HIGH_DESERT_TYPE;
    }
    if(nodeType == FoliageHelpers::LOW_GROUND_NODE_TYPE && nodeBiome == LevelBiome::Desert) {
        return MapDiskManager::LOW_DESERT_TYPE;
    }

    return MapDiskManager::FLOOR_GENERIC_TYPE;
}

const std::string MapDiskManager::SEPARATOR = ",";

const std::string MapDiskManager::CONTENT_SEPARATOR = "-";
// const int MapDiskManager::INITIAL_OFFSET = 2;


const std::string MapDiskManager::MAPS_PATH = getAssetPath() + "/maps";

const std::string MapDiskManager::SURVIVAL_MAPS_PATH = getAssetPath() + "/maps/survival";
const std::string MapDiskManager::SURVIVAL_MAPS_THUMBNAILS_PATH = getAssetPath() + "/maps/thumbnails/survival";
const std::string MapDiskManager::CHALLENGE_RESOURCE_PATH = getAssetPath() + "/maps/challenges/resources";

// Example formatting:
// 512,512,1-10,0-22,1-4

// First two entries are map size.

/// <summary>
/// Save a map to disk. Can only be run in the editor as
/// Android / iOS doesn't support file access to the Asset
/// folder.
/// </summary>
/// <param name="map"></param>
/// <param name="savePath">Full Asset folder path.</param>
void MapDiskManager::save_map(Matrix<MapNode> map, std::string savePath) {

    std::vector<std::filesystem::path> pathsThatMustExist = {
        std::filesystem::path("output") / "maps" / "thumbnails" / "survival",
        std::filesystem::path("output") / "maps" / "RandomGrassMap-Small",
    };

    for(auto path : pathsThatMustExist) {
        if(!get_path_exists(path)) {
            std::filesystem::create_directories(path);
        }
    }

    std::vector<std::string> content (map.dim_a() * map.dim_b() + MapDiskManager::INITIAL_OFFSET);
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
                MapDiskManager::CONTENT_SEPARATOR +
                "f" + std::to_string(get_foliage_map_mapping()[mapNode.foliageType]) +
                std::format(
                    "{0}{1}{2}", MapDiskManager::CONTENT_SEPARATOR,
                    "b", static_cast<int>(mapNode.nodeBiome));

            if(x == map.dim_a()-1) {
                nodeData += "\n";
            }

            content[i+MapDiskManager::INITIAL_OFFSET] = nodeData;
            i++;
        }
    }

    std::string saveString = join(content, MapDiskManager::SEPARATOR);
    write_to_file(saveString, savePath);
}

void MapDiskManager::write_to_file(std::string data, std::string path) {
    // std::string input;
    // std::cin >> input;
    std::ofstream out(path);
    out << data;
    out.close();
}

std::string MapDiskManager::join(const std::vector<std::string> &lst, const std::string &delim) {
    std::string ret;
    for(const auto &s : lst) {
        if(!ret.empty())
            ret += delim;
        ret += s;
    }
    return ret;
}

std::vector<std::string> MapDiskManager::split(std::string str, std::string delimiter) {
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

Matrix<MapNode> MapDiskManager::load_map(std::string filePath) {
    // std::string loadText = Resources.Load<TextAsset>(resourceName).text;

    // Open the file using ifstream.
    std::ifstream file(filePath);

    // confirm file opening.
    if(!file.is_open()) {
        // print error message and return
        std::cerr << "Failed to open file: " << filePath << std::endl;
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

// bool SaveMapToResources(MapNode[,] map, string resourceName) {
//     SaveMap(map, Path.Join(CHALLENGE_RESOURCE_PATH, resourceName+".txt"));
//     Logger.Log("Save completed!");
//     return true;
// }

// std::vector<MapNode> MapDiskManager::LoadMapFromResources(std::string resourceName) {
//     std::string loadText = Resources.Load<TextAsset>(resourceName).text;
//     return convert_string_to_map(loadText);
// }

// MapNode[,] LoadMapFromAddressableLabel(string labelName) {
//     string loadText = AddressablesHelpers.LoadRandomResource<TextAsset>(labelName).text;
//     return convert_string_to_map(loadText);
// }

// void LoadMapFromAddressableLabelAsync(string labelName, Action<MapNode[,]> callback) {
//     AddressablesHelpers.LoadRandomResourceAsync<TextAsset>(labelName, (TextAsset textAsset)=> {
//         string loadText = textAsset.text;
//         var map = convert_string_to_map(loadText);
//         callback(map);
//     });
// }

// MapNode[,] LoadMapFromAddressableLabelAsync(string labelName) {
//     TextAsset textAsset = await AddressablesHelpers.LoadRandomResourceAsync<TextAsset>(labelName);
//     string loadText = textAsset.text;
//     var map = convert_string_to_map(loadText);
//     return map;
// }

std::string MapDiskManager::get_base_map_path() {
    return MAPS_PATH;
}

std::string MapDiskManager::get_map_path(std::string mapName) {
    std::filesystem::path path (MAPS_PATH);
    path /= (mapName);
    return path.generic_string();
}

std::string MapDiskManager::get_map_path(LevelBiome biome, MapSize size) {
    std::string mapName = get_map_name_prefix(biome, size);
    std::filesystem::path path (MAPS_PATH);
    path /= (mapName);
    return path.generic_string();
}

std::string MapDiskManager::get_map_name_prefix(LevelBiome biome, MapSize size) {
    std::string mapNamePrefix = std::format(
        "{}-{}", BIOME_MAPNAME_DICT.at(biome), MAP_SIZE_NAME_DICT.at(size));
    return mapNamePrefix;
}

Matrix<MapNode> MapDiskManager::convert_string_to_map(std::string loadText) {

    // string loadText = File.ReadAllText(loadPath);

    std::vector<std::string> content = split(loadText, ",");

    int lengthX = std::stoi(content[0]);
    int lengthY = std::stoi(content[1]);

    std::cout << std::format(
        "Loading map of size {0}x{1}", lengthX, lengthY) << std::endl;

    Matrix<MapNode> map (lengthX, lengthY);

    int i = 0;
    // for(int y = map.GetLength(1)-1; y >= 0; y--) {
    for(int y = 0; y < map.dim_b(); y++) {
        for(int x = 0; x < map.dim_a(); x++) {

            auto mapNode = MapNode();

            auto nodeData = split(
                content[i + MapDiskManager::INITIAL_OFFSET],
                MapDiskManager::CONTENT_SEPARATOR);

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

std::optional<MapObject> MapDiskManager::load_map_object(std::string resourcePath) {
    
    auto map = load_map(resourcePath);
    if(map.empty()) {
        return std::nullopt;
    }
    MapObject mapObject = MapObject(map);
    return mapObject;
}

void MapDiskManager::save_map_thumbnail(Matrix<MapNode> fullMap, std::string mapName) {

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
                    // mapColours[index] = finalCol;
                }
            }
        }
    }

    

    // Magick::Blob blob(&mapColours[0], mapColours.size());

    // image.size(std::format("{}x{}", fullSizeX, fullSizeY));
    // image.magick("RGB"); 
    // image.read(blob);

    std::filesystem::path path (SURVIVAL_MAPS_THUMBNAILS_PATH);
    path /= (mapName+".jpg");

    std::cout << "Saving thumbnail to: "+path.generic_string() << std::endl;

    // Write the image to a file.
    // Quality is in the range 1-100.
    // Mode 3 = RGB.
    int result = stbi_write_jpg(
        path.c_str(),
        fullSizeX, fullSizeY, channelNum, &mapColours[0],
        fullSizeX*channelNum);

    if(result == 0) {
        std::cerr << "Thumbnail save failed: "+mapName << std::endl;
    }
    else {
        std::cout << "Thumbnail saved successfully: "+mapName << std::endl;
    }


    // Texture2D mapTex = new Texture2D(
    //     fullSizeX,
    //     fullSizeY);
    // mapTex.SetPixels32(mapColours);
    // mapTex.Apply();
    // byte[] itemBGBytes = mapTex.EncodeToPNG();
    // File.WriteAllBytes(
    //     Path.Join(SURVIVAL_MAPS_THUMBNAILS_PATH, mapName+".png"),
    //     itemBGBytes);

    
}