#include <unordered_map>
#include <unordered_set>
#include <expected>

#include "biome_foliage_info.h"
#include "foliage/foliage.h"
#include "mapdisk_manager.h"



BiomeFoliageInfo::BiomeFoliageInfo() {
	impossibleTypesDict = std::unordered_map<FoliageType, std::unordered_set<FoliageType>>();
	defaultHigherSet = std::unordered_set<FoliageType>();
	relationsDict = std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>>();
}

void BiomeFoliageInfo::Setup(
		std::unordered_map<FoliageType, int> allowedTypes,
		std::unordered_map<FoliageType, int> walkableAllowedTypes,
		std::string relationsFile) {

	// PossibleTypes = new int[FoliageHelpers.FoliageCount];
	// WalkablePossibleTypes = new int[FoliageHelpers.FoliageCount];
	// StartPossibleTypes = new int[Foliage_MAX];

	// NeighbourBonus = new (FoliageType, int)[FoliageHelpers.FoliageCount][];

	// We set up a set for easy access later.
	// defaultSet = new FoliageType[FoliageHelpers.FoliageCount];
	// walkableDefaultSet = new FoliageType[FoliageHelpers.FoliageCount];


	defaultSet = std::vector<FoliageType>(static_cast<size_t>(FoliageType::Foliage_MAX));
	walkableDefaultSet = std::vector<FoliageType>(static_cast<size_t>(FoliageType::Foliage_MAX));

	MapDiskManager mdm = MapDiskManager();
	auto relationsPath = mdm.get_map_path(relationsFile);
	auto mapObjectOptional = mdm.load_map_object(relationsPath);
	if(!mapObjectOptional) {
		std::cerr << "Could not load relations file: "+relationsPath << std::endl;
		return;
	}
	auto mapObject = *mapObjectOptional;




	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> upRelationsDict = {};
	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> downRelationsDict = {};
	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> leftRelationsDict = {};
	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> rightRelationsDict = {};

	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> upLeftRelationsDict = {};
	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> upRightRelationsDict = {};
	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> downLeftRelationsDict = {};
	std::unordered_map<FoliageType, std::unordered_map<FoliageType, int>> downRightRelationsDict = {};


	int defaultIndex = 0;
	int walkableDefaultIndex = 0;


	for(int x = 0; x < mapObject.LengthX; x++) {
		for(int y = 0; y < mapObject.LengthY; y++) {
			
			auto map = mapObject.map;
			auto mapNode = map[x][y];
			FoliageType currentFoliageType = mapNode.foliageType;

			if(currentFoliageType == FoliageType::Foliage_NoSelection) {
				continue;
			}

			for(int nx = -1; nx <= 1; nx++) {
				for(int ny = -1; ny <= 1; ny++) {

					if(nx == 0 && ny == 0) {
						continue;
					}

					int realNX = x + nx;
					int realNY = y + ny;

					if(realNX < 0 || realNX >= mapObject.LengthX ||
							realNY < 0 || realNY >= mapObject.LengthY) {
						
						continue;
					}

					FoliageType neighbourFoliageType =
						mapObject.map[realNX][realNY].foliageType;

					if(neighbourFoliageType == FoliageType::Foliage_NoSelection) {
						continue;
					}


					// Dictionary<FoliageType, Dictionary<FoliageType, int>> relationsDict = null;
					std::unordered_map<
						FoliageType, std::unordered_map<FoliageType, int>>* relationsDict;
					Direction direction = Direction::DirectionNone;
					if(nx == 0 && ny == 1) {
						relationsDict = &upRelationsDict;
						direction = Direction::DirectionUp;
					}

					if(nx == 0 && ny == -1) {
						relationsDict = &downRelationsDict;
						direction = Direction::DirectionDown;
					}

					if(nx == -1 && ny == 0) {
						relationsDict = &leftRelationsDict;
						direction = Direction::DirectionLeft;
					}

					if(nx == 1 && ny == 0) {
						relationsDict = &rightRelationsDict;
						direction = Direction::DirectionRight;
					}

					if(nx == -1 && ny == 1) {
						relationsDict = &upLeftRelationsDict;
						direction = Direction::DirectionUpLeft;
					}

					if(nx == 1 && ny == 1) {
						relationsDict = &upRightRelationsDict;
						direction = Direction::DirectionUpRight;
					}

					if(nx == -1 && ny == -1) {
						relationsDict = &downLeftRelationsDict;
						direction = Direction::DirectionDownLeft;
					}

					if(nx == 1 && ny == -1) {
						relationsDict = &downRightRelationsDict;
						direction = Direction::DirectionDownRight;
					}

					std::unordered_map<FoliageType, int> valueDict = {};
					auto valueDictIter = relationsDict->find(currentFoliageType);
					if(valueDictIter != relationsDict->end()) {
						valueDict = valueDictIter->second;
					}
					bool alreadyContains = valueDict.contains(neighbourFoliageType);
					if(alreadyContains) {
						continue;
					}
					valueDict[neighbourFoliageType] = 1;
					(*relationsDict)[currentFoliageType] = valueDict;

					// if(allowedTypes.TryGetValue(currentFoliageType, out int priority)) {
					auto allowedTypesIter = allowedTypes.find(currentFoliageType);
					if(allowedTypesIter != allowedTypes.end()) {
						int priority = allowedTypesIter->second;
						if(PossibleTypes[static_cast<int>(currentFoliageType)] <= 0) {
							PossibleTypes[static_cast<int>(currentFoliageType)] = priority;
							defaultSet[defaultIndex] = currentFoliageType;
							defaultIndex++;
						}
					}

					// if(walkableAllowedTypes.TryGetValue(currentFoliageType, out int wPriority)) {
					auto walkableAllowedTypesIter = walkableAllowedTypes.find(currentFoliageType);
					if(walkableAllowedTypesIter != walkableAllowedTypes.end()) {
						int priority = walkableAllowedTypesIter->second;
						if(WalkablePossibleTypes[static_cast<int>(currentFoliageType)] <= 0) {
							WalkablePossibleTypes[static_cast<int>(currentFoliageType)] = priority;
							walkableDefaultSet[walkableDefaultIndex] = currentFoliageType;
							walkableDefaultIndex++;
						}
					}
				}
			}
		}
	}

	// Array.Resize(ref defaultSet, defaultIndex);
	// Array.Resize(ref walkableDefaultSet, walkableDefaultIndex);

	defaultSet.resize(defaultIndex);
	walkableDefaultSet.resize(walkableDefaultIndex);

	const int foliageLength = static_cast<int>(FoliageType::Foliage_MAX);
	// UpRelations.clear();
	// DownRelations.clear();
	// LeftRelations.clear();
	// RightRelations.clear();

	// UpLeftRelations.clear();
	// UpRightRelations.clear();
	// DownLeftRelations.clear();
	// DownRightRelations.clear();
	UpRelations = std::array<std::vector<FoliageType>, foliageLength>();
	DownRelations = std::array<std::vector<FoliageType>, foliageLength>();
	LeftRelations = std::array<std::vector<FoliageType>, foliageLength>();
	RightRelations = std::array<std::vector<FoliageType>, foliageLength>();

	UpLeftRelations = std::array<std::vector<FoliageType>, foliageLength>();
	UpRightRelations = std::array<std::vector<FoliageType>, foliageLength>();
	DownLeftRelations = std::array<std::vector<FoliageType>, foliageLength>();
	DownRightRelations = std::array<std::vector<FoliageType>, foliageLength>();


	for(auto kvp : upRelationsDict) {
		UpRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
	for(auto kvp : downRelationsDict) {
		DownRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
	for(auto kvp : leftRelationsDict) {
		LeftRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
	for(auto kvp : rightRelationsDict) {
		RightRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}

	for(auto kvp : upLeftRelationsDict) {
		UpLeftRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
	for(auto kvp : upRightRelationsDict) {
		UpRightRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
	for(auto kvp : downLeftRelationsDict) {
		DownLeftRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
	for(auto kvp : downRightRelationsDict) {
		DownRightRelations[static_cast<int>(kvp.first)] = keysFromMap(kvp.second);
	}
}

template<typename T, typename V>
std::vector<T> BiomeFoliageInfo::keysFromMap(std::unordered_map<T, V> map) {
	std::vector<T> keys;
	keys.reserve(map.size());

	for(auto kv : map) {
		keys.push_back(kv.first);
	}
	return keys;
}

// template<typename T>
// T arr[] resize(T arr[], int newSize) {
//     T newArr[newSize];

//     memcpy(newArr, arr, newSize * sizeof(int));

//     delete [] arr;
//     arr = newArr;
// }

std::array<int,static_cast<size_t>(FoliageType::Foliage_MAX)> BiomeFoliageInfo::GetNewDefaultData() {
	std::array<int,static_cast<size_t>(FoliageType::Foliage_MAX)> remainingPossibleTypes = {};
	for(FoliageType foliageType : defaultSet) {
		remainingPossibleTypes[(int)foliageType] = PossibleTypes[(int)foliageType];
	}

	return remainingPossibleTypes;
}

std::array<int,static_cast<size_t>(FoliageType::Foliage_MAX)> BiomeFoliageInfo::GetNewWalkableData() {
	// int remainingPossibleTypes[FoliageType::Foliage_MAX] = {};
	std::array<int,static_cast<size_t>(FoliageType::Foliage_MAX)> remainingPossibleTypes = {};
	for(FoliageType foliageType : walkableDefaultSet) {
		remainingPossibleTypes[(int)foliageType] = WalkablePossibleTypes[(int)foliageType];
	}

	return remainingPossibleTypes;
}

std::array<int,static_cast<size_t>(FoliageType::Foliage_MAX)> BiomeFoliageInfo::GetNewEmptyData() {
	// int remainingPossibleTypes[FoliageType::Foliage_MAX] = {};
	std::array<int,static_cast<size_t>(FoliageType::Foliage_MAX)> remainingPossibleTypes = {};
	return remainingPossibleTypes;
}
