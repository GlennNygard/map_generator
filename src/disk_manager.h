#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "matrix.h"
#include "foliage.h"
#include "map_definitions.h"
#include "argparse.h"


struct MapNode {
	int nodeType;
	int foliageType;
	LevelBiome nodeBiome;
};

class MapObject {

public:
    Matrix<MapNode> map;

    /// @brief Map array size X.
    int lengthX;
    
    /// @brief Map array size Y.
    int lengthY;

    MapObject(int lengthX, int lengthY) {
        lengthX = lengthX;
        lengthY = lengthY;
        map = Matrix<MapNode>(lengthX, lengthY);
    }

    MapObject(Matrix<MapNode> map) {
        this->map = map;
        lengthX = map.dim_a();
        lengthY = map.dim_b();
    }

    MapObject(Matrix<int> nodeTypeMap, Matrix<int> foliageMap) {
        lengthX = nodeTypeMap.dim_a();
        lengthY = nodeTypeMap.dim_b();
        map = Matrix<MapNode>(lengthX, lengthY);

        for(int x = 0; x < lengthX; x++) {
            for(int y = 0; y < lengthY; y++) {
                auto mn = MapNode();
                mn.nodeType = nodeTypeMap[x][y];
                mn.foliageType = foliageMap[x][y];
                mn.nodeBiome = LevelBiome::None;
                map[x][y] = mn;
            }
        }
    }
};

class DiskManager {

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


    DiskManager();

    static const std::string getAssetPath() {
        std::filesystem::path path ("./");
        path /= "output";
        return path;
    };

    /**
     * These are use for map names when storing/getting maps to/from disk.
     */
    static const std::unordered_map<MapSize, std::string> MAP_SIZE_NAME_DICT;

    static const std::unordered_map<MapSize, std::string> MAP_SIZE_ADDRESSABLE_NAME_DICT;

    static const std::unordered_map<MapSize, Vector2Int> MAP_SIZE_NODE_COUNT_DICT;

    static const std::unordered_map<LevelBiome, std::string> BIOME_MAPNAME_DICT;

    static const std::unordered_map<LevelBiome, std::string> BIOME_ADDRESSABLE_LABEL_DICT;

    static const std::unordered_map<LevelBiome, int> BIOME_MAPCOUNT_DICT;


    /**
     * Mappings from foliage types to map integers.
     * These integers are written to map files and should
     * never be changed.
     */
    std::unordered_map<int, int> mapFoliageMapping;
    static std::string join(const std::vector<std::string> &lst, const std::string &delim);


    Matrix<MapNode> load_map(std::filesystem::path mapPath);
    std::optional<MapObject> load_map_object(std::string resourcePath);

    std::vector<std::string> split(std::string str, std::string delimiter);

    static const bool get_path_exists(std::string loadPath) {
        return std::filesystem::exists(loadPath);
    }

    std::string get_base_map_path();
    std::string get_map_path(std::string mapName);
    std::string get_map_path(LevelBiome biome, MapSize mapSize);
    std::string get_relational_map_path(std::string mapName);

    std::string get_map_name(
        std::string mapNamePrefix, int currentIndex);

    std::string get_map_prefix(
        ArgValues argValues,
        LevelBiome mapBiome);

    /**
     * Save a map to disk. Can only be run in the editor as
     * Android / iOS doesn't support file access to the Asset
     * folder.
     */
    void save_map(
        const MapObject& mapObject,
        const std::string mapName,
        const std::string mapNamePrefix,
        const int currentIndex);
    void save_map_thumbnail(
        const Matrix<MapNode>& fullMap,
        const std::string mapName,
        const std::string mapNamePrefix);

private:

    static const int INITIAL_OFFSET = 2;
    static const std::string SEPARATOR;
    static const std::string CONTENT_SEPARATOR;

    std::filesystem::path _mapsPath;
    std::filesystem::path _mapsThumbnailsPath;

    std::filesystem::path _relationalMapPath;

    std::unordered_map<int, int> m_foliageMapMapping;

    static const std::unordered_map<int, int> m_mapNodeTypeMapping;
    static const std::unordered_map<int, LevelBiome> m_mapNodeBiomeMapping;

    void perform_map_save(
        const Matrix<MapNode>& map,
        const std::filesystem::path directoryPath,
        const std::string mapName);
    void write_to_file(std::string data, std::string path);
    int node_data_to_map_type(int nodeType, LevelBiome nodeBiome);
    std::string get_map_name_prefix(LevelBiome biome, MapSize mapSize);

    Matrix<MapNode> convert_string_to_map(std::string loadText);

    std::unordered_map<int, int> get_foliage_map_mapping() {
        return m_foliageMapMapping;
    }

    std::unordered_map<int, int> get_map_node_type_mapping() {
        return m_mapNodeTypeMapping;
    }
};
