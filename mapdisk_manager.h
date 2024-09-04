#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "matrix.h"
#include "foliage/foliage.h"
#include "map_definitions.h"


struct MapNode {
	int nodeType;
	FoliageType foliageType;
	LevelBiome nodeBiome;
};

enum class load_error {
    no_such_file,
};

class MapObject {

    public:
    Matrix<MapNode> map;

    /// <summary>
    /// Map array size X.
    /// </summary>
    int LengthX;
    /// <summary>
    /// Map array size Y.
    /// </summary>
    int LengthY;

    int SectionCountX;
    int SectionCountY;

    MapObject(int lengthX, int lengthY) {
        LengthX = lengthX;
        LengthY = lengthY;
        map = Matrix<MapNode>(lengthX, lengthY);

        SectionCountX = LengthX / MapDefinitions::SUBSECTION_SIDE_COUNT_X;
        SectionCountY = LengthY / MapDefinitions::SUBSECTION_SIDE_COUNT_Y;
    }

    MapObject(Matrix<MapNode> map) {
        this->map = map;
        LengthX = map.dim_a();
        LengthY = map.dim_b();

        // LengthX = MapDefinitions::FULL_GRID_COUNT_X;
        // LengthY = MapDefinitions::FULL_GRID_COUNT_Y;

        SectionCountX = LengthX / MapDefinitions::SUBSECTION_SIDE_COUNT_X;
        SectionCountY = LengthY / MapDefinitions::SUBSECTION_SIDE_COUNT_Y;
    }

    /// <summary>
    /// Construct a new map object from a type map and foliage map.
    /// This does not assume any specific node biomes.
    /// </summary>
    /// <param name="nodeTypeMap"></param>
    /// <param name="foliageMap"></param>
    MapObject(Matrix<int> nodeTypeMap, Matrix<FoliageType> foliageMap) {
        LengthX = nodeTypeMap.dim_a();
        LengthY = nodeTypeMap.dim_b();
        map = Matrix<MapNode>(LengthX, LengthY);

        for(int x = 0; x < LengthX; x++) {
            for(int y = 0; y < LengthY; y++) {
                auto mn = MapNode();
                mn.nodeType = nodeTypeMap[x][y];
                mn.foliageType = foliageMap[x][y];
                mn.nodeBiome = LevelBiome::None;
                map[x][y] = mn;
            }
        }
    }
};

class MapDiskManager {
    public:

    static const int FLOOR_GENERIC_TYPE = 0;
    static const int HIGH_GENERIC_TYPE = 1;
    static const int BORDER_GENERIC_TYPE = 2;
    static const int LOW_GENERIC_TYPE = 3;

    static const int FLOOR_GRASS_TYPE = 100;
    static const int HIGH_GRASS_TYPE = 101;
    static const int BORDER_GRASS_TYPE = 102;
    static const int LOW_GRASS_TYPE = 103;

    static const int FLOOR_DESERT_TYPE = 200;
    static const int HIGH_DESERT_TYPE = 201;
    static const int BORDER_DESERT_TYPE = 202;
    static const int LOW_DESERT_TYPE = 203;

    static const int FLOOR_SNOW_TYPE = 300;
    static const int HIGH_SNOW_TYPE = 301;
    static const int BORDER_SNOW_TYPE = 302;
    static const int LOW_SNOW_TYPE = 303;



    static const std::string getAssetPath() {
        std::filesystem::path path ("./");
        path /= "output";
        return path;
    };


    /// <summary>
    /// These are use for map names when storing/getting maps to/from disk.
    /// </summary>
    /// <typeparam name="ChallengeHelpers.MapSize"></typeparam>
    /// <typeparam name="std::string"></typeparam>
    /// <returns></returns>
    const std::unordered_map<MapSize, std::string> MAP_SIZE_NAME_DICT = {
        {MapSize::MapSize_Small, "Small"},
        {MapSize::MapSize_Medium, "Medium"},
        {MapSize::MapSize_Large, "Large"},
        {MapSize::MapSize_VeryLarge, "VeryLarge"},
        {MapSize::MapSize_IncrediblyLarge, "IncrediblyLarge"},
    };

    const std::unordered_map<MapSize, std::string> MAP_SIZE_ADDRESSABLE_NAME_DICT = {
        {MapSize::MapSize_Small, "small"},
        {MapSize::MapSize_Medium, "medium"},
        {MapSize::MapSize_Large, "large"},
        {MapSize::MapSize_VeryLarge, "very_large"},
        {MapSize::MapSize_IncrediblyLarge, "incredibly_large"},
    };

    const std::unordered_map<MapSize, std::array<int, 2>> MAP_SIZE_NODE_COUNT_DICT = {
        {MapSize::MapSize_Small, {100,100}},
        {MapSize::MapSize_Medium, {140,140}},
        {MapSize::MapSize_Large, {180,180}},
        {MapSize::MapSize_VeryLarge, {220,220}},
        {MapSize::MapSize_IncrediblyLarge, {260,260}},
    };

    const std::unordered_map<LevelBiome, std::string> BIOME_MAPNAME_DICT = {
        {LevelBiome::Grass, "RandomGrassMap"},
        {LevelBiome::Snow, "RandomSnowMap"},
    };

    const std::unordered_map<LevelBiome, std::string> BIOME_ADDRESSABLE_LABEL_DICT = {
        {LevelBiome::Grass, "map_grass"},
        {LevelBiome::Snow, "map_snow"},
    };

    const std::unordered_map<LevelBiome, int> BIOME_MAPCOUNT_DICT = {
        {LevelBiome::Grass, 10},
        {LevelBiome::Snow, 100},
    };


    /// <summary>
    /// Mappings from foliage types to map integers.
    /// These integers are written to map files and should
    /// never be changed.
    /// </summary>
    std::unordered_map<int, FoliageType> _mapFoliageMapping;
    static std::string join(const std::vector<std::string> &lst, const std::string &delim);


    Matrix<MapNode> load_map(std::string resourceName);
    std::optional<MapObject> load_map_object(std::string resourcePath);

    void save_map(Matrix<MapNode> map, std::string savePath);
    void write_to_file(std::string data, std::string path);

    std::vector<std::string> split(std::string str, std::string delimiter);


    // const std::string get_challenge_path(std::string mapName) {
    //     std::filesystem::path path (_challengeResourcePath);
    //     path /= (mapName+".txt");
    //     return path.generic_string();
    // }

    // const std::string get_challenge_resource_path(std::string mapName) {
    //     return mapName;
    // }

    static const bool get_path_exists(std::string loadPath) {
        return std::filesystem::exists(loadPath);
    }


    MapDiskManager();

    std::string get_base_map_path();
    std::string get_map_path(std::string mapName);
    std::string get_map_path(LevelBiome biome, MapSize mapSize);
    std::string get_relational_map_path(std::string mapName);
    std::string get_map_name_prefix(LevelBiome biome, MapSize mapSize);

    void save_map_thumbnail(Matrix<MapNode> fullMap, std::string mapName);

    std::unordered_map<FoliageType, int> get_foliage_map_mapping() {
        return _foliageMapMapping;
    }

    std::unordered_map<int, int> get_map_node_type_mapping() {
        return _mapNodeTypeMapping;
    }

    private:

    static const int INITIAL_OFFSET = 2;
    static const std::string SEPARATOR;
    static const std::string CONTENT_SEPARATOR;

    std::filesystem::path _mapsPath;
    std::filesystem::path _mapsThumbnailsPath;

    std::filesystem::path _relationalMapPath;

    std::unordered_map<FoliageType, int> _foliageMapMapping;
    std::unordered_map<int, int> _mapNodeTypeMapping = std::unordered_map<int,int> {
        {FLOOR_GENERIC_TYPE, FoliageHelpers::FLOOR_NODE_TYPE},
        {HIGH_GENERIC_TYPE, FoliageHelpers::HIGH_GROUND_NODE_TYPE},
        {BORDER_GENERIC_TYPE, FoliageHelpers::BORDER_NODE_TYPE},
        {LOW_GENERIC_TYPE, FoliageHelpers::LOW_GROUND_NODE_TYPE},

        {FLOOR_DESERT_TYPE, FoliageHelpers::FLOOR_NODE_TYPE},
        {HIGH_DESERT_TYPE, FoliageHelpers::HIGH_GROUND_NODE_TYPE},
        {BORDER_DESERT_TYPE, FoliageHelpers::BORDER_NODE_TYPE},
        {LOW_DESERT_TYPE, FoliageHelpers::LOW_GROUND_NODE_TYPE},
    };

    std::unordered_map<int, LevelBiome> _mapNodeBiomeMapping = std::unordered_map<int,LevelBiome> {
        {FLOOR_GENERIC_TYPE, LevelBiome::None},
        {HIGH_GENERIC_TYPE, LevelBiome::None},
        {BORDER_GENERIC_TYPE, LevelBiome::None},
        {LOW_GENERIC_TYPE, LevelBiome::None},

        {FLOOR_DESERT_TYPE, LevelBiome::Desert},
        {HIGH_DESERT_TYPE, LevelBiome::Desert},
        {BORDER_DESERT_TYPE, LevelBiome::Desert},
        {LOW_DESERT_TYPE, LevelBiome::Desert},
    };

    int node_data_to_map_type(int nodeType, LevelBiome nodeBiome);

    Matrix<MapNode> convert_string_to_map(std::string loadText);
    void create_required_paths();
};
