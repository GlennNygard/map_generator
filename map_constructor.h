#pragma once

#include <cmath>

#include "foliage_processor.h"
#include "biome_foliage_info.h"
#include "grass_foliage_info.h"
#include "foliage_info.h"
#include "foliage_definitions/foliage_definitions.h"


struct LevelValues {
	LevelBiome biome;

	int gridCountX;
	int gridCountY;

	Vector2Int gridBoundsX;
	Vector2Int gridBoundsY;

	LevelValues() {}

	static const LevelValues create_values(Vector2Int sectionCount, LevelBiome biome) {
        auto lv = LevelValues();
        lv.biome = biome;

        // lv.nodeDiameter = NODE_DIAMETER;

        // // Amount of nodes per section.
        // lv.SectionNodeCountX = SECTION_NODE_COUNT;
        // lv.SectionNodeCountY = SECTION_NODE_COUNT;
        // // Width and height of map border (in sections).
        // lv.unplayableAreaBorder = new Vector2Int(SECTION_BORDER,SECTION_BORDER);

        // lv.sectionsX = sectionCount.x;
        // lv.sectionsY = sectionCount.y;

        lv.gridCountX = MapDefinitions::SUBSECTION_SIDE_COUNT_X * sectionCount.x;
        lv.gridCountY = MapDefinitions::SUBSECTION_SIDE_COUNT_Y * sectionCount.y;

        // lv.playableSectionsX = lv.sectionsX - lv.unplayableAreaBorder.x;
        // lv.playableSectionsY = lv.sectionsY - lv.unplayableAreaBorder.y;
        // lv.sectionWorldSize.x = lv.SectionNodeCountX * lv.nodeDiameter;
        // lv.sectionWorldSize.y = lv.SectionNodeCountY * lv.nodeDiameter;
        // lv.worldSize = lv.sectionWorldSize * new Vector2Int(lv.sectionsX, lv.sectionsY);
        // lv.playableWorldSize = lv.sectionWorldSize * new Vector2Int(lv.playableSectionsX, lv.playableSectionsY);

        lv.gridBoundsX = Vector2Int(0,lv.gridCountX);
        lv.gridBoundsY = Vector2Int(0,lv.gridCountY);

        // lv.randomMapFillPercent = new Vector2Int(0,25);
        // lv.smoothIterations = 4;
        // lv.mapBorderInSections = 1;

        return lv;
    }
};


class MapConstructor {

	public:

	// MapConstructor() {}

    MapConstructor(LevelValues levelValues) {
		_levelValues = levelValues;
		BiomeFoliageInfo biomeFoliageInfo;
		if(levelValues.biome == LevelBiome::Grass) {
			// biomeFoliageInfo = dynamic_cast<BiomeFoliageInfo*>(gfi);
			biomeFoliageInfo = GrassFoliageInfo();
		}
		else if(levelValues.biome == LevelBiome::Desert) {
			// We don't have any specific desert foliage info for now.
			biomeFoliageInfo = GrassFoliageInfo();
		}
		// else if(levelValues.biome == LevelBiome::Snow) {
		// 	biomeFoliageInfo = new SnowFoliageInfo();
		// }
		else {
			biomeFoliageInfo = GrassFoliageInfo();
		}
        _foliageProcessor = std::make_unique<FoliageProcessor>(biomeFoliageInfo);
    }

    std::pair<MapObject, bool> construct_random_map(int levelSeed=-1) {

		if(levelSeed == -1) {
			levelSeed = time(0);
		}
		std::cout << std::format("Using level SEED: {0}", levelSeed) << std::endl;
		srand(levelSeed);
		// System.Random pseudoRandom = new System.Random(levelSeed.GetHashCode());

		Matrix<int> fullNodeMap(_levelValues.gridCountX,_levelValues.gridCountY);
		for(int x = 0; x < fullNodeMap.dim_a(); x++) {
			for(int y = 0; y < fullNodeMap.dim_b(); y++) {
				fullNodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
			}
		}

		std::pair<Matrix<FoliageType> , bool> randomFoliageMapPair = create_random_foliage_map(
			_levelValues, fullNodeMap);

		auto fullFoliageMap = randomFoliageMapPair.first;
		bool success = randomFoliageMapPair.second;


		// Sine we are transitioning into making foliage the main source of truth,
		// we copy the foliage node type back into the node map.
		for(int x = 0; x < fullNodeMap.dim_a(); x++) {
			for(int y = 0; y < fullNodeMap.dim_b(); y++) {
				FoliageType foliage = fullFoliageMap[x][y];
				FoliageInfo foliageInfo = FoliageDefinitions::get_foliage_definitions()[(int)foliage];
				fullNodeMap[x][y] = foliageInfo.nodeType;
				// Only the low ground node type needs to be copied like this for now.
				// if(foliageInfo.nodeType == FoliageHelpers::LOW_GROUND_NODE_TYPE) {
				// }
			}
		}

		if(!success) {
			std::cerr << "Random map could not be generated." << std::endl;
			// throw new CouldNotCreateRandomMapException();
		}

		auto mapObject = MapObject(fullNodeMap, fullFoliageMap);
		return std::pair(mapObject, success);
	}

	static Vector2Int node_grid_pos_from_grid_space_point(
            Vector2Int gridSpacePos, int countX, int countY) {

		float valX = (gridSpacePos.x + (((float)countX)/2));
		float valY = (gridSpacePos.y + (((float)countY)/2));

        int x = static_cast<int>(std::floorf(valX));
        int y = static_cast<int>(std::floorf(valY));

		// Logger.Log(x +", "+y);
		return Vector2Int(x, y);
	}

    static Matrix<int> CarveMainRoom(
			Matrix<int> nodeMap, Vector2Int worldPos,
			float nodeDiameter,
			int sectionCountX, int sectionCountY) {

		float radius = 200; // 400

		float shortRadius = radius/nodeDiameter;
		int tileRange = static_cast<int>(std::round(shortRadius));
		Vector2Int nodePos = node_grid_pos_from_grid_space_point(
			worldPos, sectionCountX, sectionCountY);

		for(int x = -tileRange + nodePos.x; x < tileRange + nodePos.x; x++) {
            for(int y = -tileRange + nodePos.y; y < tileRange + nodePos.y; y++) {
				if(x >= 0 &&
						x < sectionCountX &&
						y >= 0 &&
						y < sectionCountY) {

					float dist = square_dist(nodePos - Vector2Int(x,y));
					if(dist>shortRadius*shortRadius) {
						continue;
					}
                    if(nodeMap[x][y] == FoliageHelpers::HIGH_GROUND_NODE_TYPE) {
						nodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
					}
                }
            }
        }

		return nodeMap;
	}

	static float square_dist(Vector2Int vec) {
		return (vec.x*vec.x* + vec.y*vec.y);
	}

	static float dist(Vector2Int vec) {
		return std::sqrt(vec.x*vec.x* + vec.y*vec.y);
	}

	static Matrix<FoliageType> CarveMainRoom(
			Matrix<FoliageType> foliageMap,
			Vector2Int worldPos,
			float nodeDiameter,
			int sectionCountX, int sectionCountY) {

		float radius = 200; // 400

		float shortRadius = radius/nodeDiameter;
		int tileRange = static_cast<int>(std::round(shortRadius));
		Vector2Int nodePos = node_grid_pos_from_grid_space_point(
			worldPos, sectionCountX, sectionCountY);

		for(int x = -tileRange + nodePos.x; x < tileRange + nodePos.x; x++) {
            for(int y = -tileRange + nodePos.y; y < tileRange + nodePos.y; y++) {
				if(x >= 0 &&
						x < sectionCountX &&
						y >= 0 &&
						y < sectionCountY) {

					float dist = square_dist(nodePos - Vector2Int(x,y));
					if(dist > shortRadius*shortRadius) {
						continue;
					}
                    // if(FoliageHelpers::unwalkableFoliageTypes.Contains(foliageMap[x,y])) {
					if(!FoliageDefinitions::get_foliage_definitions()[
							static_cast<int>(foliageMap[x][y])].walkable) {
						foliageMap[x][y] = FoliageType::Foliage_NoFoliage;
					}
                }
            }
        }

		return foliageMap;
	}

    static Matrix<int> generate_section_walkable_representation(
			Matrix<int> nodeMap, int smoothIterations) {

		// Previously 12.
		for(int i = 0; i < smoothIterations; i ++) {
			SmoothSection(nodeMap);
		}

		int borderSize = 1;

		for (int x = 0; x < nodeMap.dim_a(); x ++) {
			for (int y = 0; y < nodeMap.dim_b(); y ++) {
				int nodeType = nodeMap[x][y];
				if(x >= borderSize &&
						x < nodeMap.dim_a() - borderSize &&
						y >= borderSize &&
						y < nodeMap.dim_b() - borderSize) {
				}
				// If the node is a border.
				else {
					nodeType = FoliageHelpers::BORDER_NODE_TYPE;
				}
				nodeMap[x][y] = nodeType;
			}
		}
		return nodeMap;
	}

	static Matrix<int> SmoothSection(Matrix<int> nodeMap) {
		int countX = nodeMap.dim_a();
		int countY = nodeMap.dim_b();
		for(int x = 0; x < countX; x ++) {
			for (int y = 0; y < countY; y ++) {
				int neighbourWallTiles = GetSurroundingWallCount(nodeMap, x,y);

				if (neighbourWallTiles > 4) {
					nodeMap[x][y] = FoliageHelpers::HIGH_GROUND_NODE_TYPE;
				}
				else if (neighbourWallTiles < 4) {
					nodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
				}
			}
		}
		return nodeMap;
	}

    static int GetSurroundingWallCount(Matrix<int> nodeMap, int gridX, int gridY) {

		// Returns how many walls are around each node in grid.
		int wallCount = 0;
		for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX ++) {
			for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY ++) {
				if (neighbourX >= 0 &&
						neighbourX < nodeMap.dim_a() &&
						neighbourY >= 0 &&
						neighbourY < nodeMap.dim_b())
					{
					
					// If neighbour is within borders.
					if (neighbourX != gridX || neighbourY != gridY) {
						// Add 1 if wall, 0 otherwise.
						// Node node = grid.GetGridNode(neighbourX,neighbourY);
						if(nodeMap[neighbourX][neighbourY] == FoliageHelpers::HIGH_GROUND_NODE_TYPE) {
							wallCount += 1;
						}
						// else if(FoliageHelpers::type == 2) {
						// 	wallCount -= 2;
						// }
						// wallCount += grid.GetGridNode(neighbourX,neighbourY).type;
					}
				}
				else {
					wallCount ++;
				}
			}
		}
		
		return wallCount;
	}


	static Matrix<FoliageType> SetMapBorders(
			Matrix<FoliageType> foliageMap,
			int borderCount) {


		for(int x = 0; x < foliageMap.dim_a(); x++) {
            for(int y = 0; y < foliageMap.dim_b(); y++) {
				if(x >= borderCount &&
						x <= foliageMap.dim_a() - borderCount &&
						y >= borderCount &&
						y <= foliageMap.dim_b() - borderCount) {
					
					continue;
                }
				// For now we just set them all as no foliage.
				foliageMap[x][y] = FoliageType::Foliage_NoFoliage;
            }
        }

		return foliageMap;
	}


	/// <summary>
	/// Creates random foliage for a map.
	/// </summary>
	/// <param name="levelValues"></param>
	/// <param name="fullNodeMap"></param>
	/// <param name="pseudoRandom"></param>
	/// <returns>Random foliage map if successfull, otherwise null.</returns>
	std::pair<Matrix<FoliageType>, bool> create_random_foliage_map(
			LevelValues levelValues, Matrix<int> fullNodeMap) {
		// Create random terrain.
		// Full map.
		int maxAttempts = 3; // 5.
		int currentAttempt = 0;
		Matrix<FoliageType> fullFoliageGrid = {};
		bool success = true;
		while(fullFoliageGrid.empty() && currentAttempt < maxAttempts) {
			auto resultPair = _foliageProcessor->mark_foliage_nodes(
				fullNodeMap,
				levelValues.gridBoundsX.x, levelValues.gridBoundsY.x,
				levelValues.gridBoundsX.y, levelValues.gridBoundsY.y,
				levelValues.gridCountX, levelValues.gridCountY);
			fullFoliageGrid = resultPair.first;
			success = resultPair.second;
			if(!success) {
				std::cerr << std::format("Random map generation failed. Attempt {0} of {1}. Trying again...", currentAttempt+1, maxAttempts) << std::endl;
			}
			currentAttempt++;
		}

		// Carve main room for unwalkable foliage.
		// fullFoliageGrid = CarveMainRoom(
		// 	fullFoliageGrid, new Vector2Int(0,0),
		// 	levelValues.nodeDiameter,
		// 	levelValues.sectionCountX, levelValues.sectionCountY);

		// Carve borders around the map.
		// SetMapBorders(fullFoliageGrid, levelValues.mapBorderInSections * levelValues.sectionCountX);

		return std::pair(fullFoliageGrid, success);
	}

	static float easeOutCubic(float t) {
		// Fast then slow.
		float tMinus = t-1;
		return tMinus*t*t+1;
	}

    static Matrix<int> RandomFillSection(
			int sectionCountX,
			int sectionCountY,
			int randomFillPercent) {

		Matrix<int> nodeMap(sectionCountX,sectionCountY);
        for(int x = 0; x < sectionCountX; x++) {
            for(int y = 0; y < sectionCountY; y++) {
				float multX = easeOutCubic(std::abs(((float)(x)/sectionCountX)-0.5f)*0.5f);
				float multY = easeOutCubic(std::abs(((float)(y)/sectionCountY)-0.5f)*0.5f);

				// Node node = grid.GetGridNode(x + section.gridStartX, y + section.gridStartY);
				int val = static_cast<int>(std::round(100 * (multX + multY)));
				if((rand() % val) < randomFillPercent) {
					nodeMap[x][y] = FoliageHelpers::HIGH_GROUND_NODE_TYPE;
				}
				else {
					nodeMap[x][y] = FoliageHelpers::FLOOR_NODE_TYPE;
				}
            }
        }
		return nodeMap;
    }

	private:

	std::unique_ptr<FoliageProcessor> _foliageProcessor;
	LevelValues _levelValues;
};
