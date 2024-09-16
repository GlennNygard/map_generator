#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "matrix.h"
#include "foliage.h"
#include "map_definitions.h"
#include "foliage_definitions.h"
#include "argparse.h"


struct MapNode {
	int nodeType;
	FoliageType foliageType;
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

    MapObject(Matrix<int> nodeTypeMap, Matrix<FoliageType> foliageMap) {
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


    DiskManager(FoliageDefinitions& foliageDefinitions);

    static const std::string getAssetPath() {
        std::filesystem::path path ("./");
        path /= "output";
        return path;
    };

    /**
     * These are use for names when storing/getting maps to/from disk.
     */
    static const std::unordered_map<MapSize, std::string> MAP_SIZE_NAME_DICT;
    static const std::unordered_map<MapSize, std::string> MAP_SIZE_ADDRESSABLE_NAME_DICT;
    static const std::unordered_map<MapSize, Vector2Int> MAP_SIZE_NODE_COUNT_DICT;
    static const std::unordered_map<LevelBiome, std::string> BIOME_MAPNAME_DICT;
    static const std::unordered_map<LevelBiome, std::string> BIOME_ADDRESSABLE_LABEL_DICT;
    static const std::unordered_map<LevelBiome, int> BIOME_MAPCOUNT_DICT;


    static const std::string GetRelationalMapPath(const std::string& mapName);
    static const bool GetPathExists(const std::string& loadPath);

    static const std::string GetMapName(
        const std::string& mapNamePrefix, const int currentIndex);

    static const std::string GetMapPrefix(
        const ArgValues& argValues,
        const LevelBiome mapBiome);

    static std::string Join(
        const std::vector<std::string> &lst, const std::string &delim);
    static std::vector<std::string> Split(
        const std::string& str, const std::string& delimiter);

    /**
     * Mappings from map integers to foliage types.
     * These integers are written to map files and should
     * never be changed.
     */
    std::unordered_map<int, FoliageType> mapFoliageMapping;

    Matrix<MapNode> LoadMap(const std::filesystem::path& mapPath);
    std::optional<MapObject> LoadMapObject(const std::string& resourcePath);

    const std::string GetBaseMapPath() const;
    const std::string GetMapPath(std::string mapName) const;
    const std::string GetMapPath(LevelBiome biome, MapSize mapSize) const;

    /**
     * Save a map to disk. Can only be run in the editor as
     * Android / iOS doesn't support file access to the Asset
     * folder.
     */
    void SaveMap(
        const MapObject& mapObject,
        const std::string& mapName,
        const std::string& mapNamePrefix,
        const int currentIndex);
    void SaveMapThumbnail(
        const Matrix<MapNode>& fullMap,
        const std::string& mapName,
        const std::string& mapNamePrefix);

private:

    static const int INITIAL_OFFSET = 2;
    static const std::string SEPARATOR;
    static const std::string CONTENT_SEPARATOR;

    static const std::unordered_map<int, int> MAP_NODE_TYPE_MAPPING;
    static const std::unordered_map<int, LevelBiome> MAP_NODE_BIOME_MAPPING;

    std::filesystem::path m_mapsPath;
    std::filesystem::path m_mapsThumbnailsPath;

    std::filesystem::path m_relationalMapPath;
    std::unordered_map<FoliageType, int> m_foliageMapMapping;
    std::unique_ptr<FoliageDefinitions> m_foliageDefinitions;

    static const std::string GetMapNamePrefix(
        const LevelBiome biome, const MapSize mapSize);

    void PerformMapSave(
        const Matrix<MapNode>& map,
        const std::filesystem::path& directoryPath,
        const std::string& mapName) const;
    void WriteToFile(const std::string& data, const std::string& path) const;
    const int NodeDataToMapType(const int nodeType, const LevelBiome nodeBiome) const;

    Matrix<MapNode> ConvertStringToMap(const std::string& loadText);
};
