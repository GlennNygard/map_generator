#pragma once

#include <vector>
#include "foliage/foliage.h"

class FoliageInfo {

    public:

    FoliageType foliageType;
    int nodeType;
	bool walkable;
	bool buildable;
	bool randomRotation;
	int mapIndex;
    LevelBiome nodeBiome;
	FoliageFloor FloorType;

    const std::vector<FoliageType> *directionSet;

    Direction direction;

	Colour nodeColour;



	bool containsResourceData;


	// std::vector<ResourceType> resources;
	// std::vector<ResourceType> biomeResources;
	std::vector<float> possibleRotations;
	float height;

    ThrottenAmount ThrottenValue;

	FoliageInfo() {
		this->containsResourceData = false;
		this->foliageType = FoliageType::Foliage_NoSelection;
		// this->resources = new ResourceType[0];
		// this->possibleRotations = new float[0];
		this->randomRotation = false;
		this->height = 0;
		// this->biomeResources = new ResourceType[Enum.GetValues(typeof(LevelBiome)).Length][];
		this->FloorType = FoliageFloor::Normal;
		this->nodeType = FoliageHelpers::FLOOR_NODE_TYPE;
		this->walkable = true;
		this->buildable = true;

		this->nodeBiome = LevelBiome::None;
        this->ThrottenValue = ThrottenAmount::None;
	}

    FoliageInfo(
			FoliageType foliageType,
			int nodeType,
			bool walkable,
			bool buildable,
			int mapIndex,
			LevelBiome nodeBiome=LevelBiome::None,
			Direction direction=Direction::DirectionNone
			// std::vector<FoliageType> directionSet={}
			// Color nodeColour=default(Color)
            ) {
		this->containsResourceData = true;
        this->foliageType = foliageType;
        this->nodeType = nodeType;
		this->walkable = walkable;
		this->buildable = buildable;
		this->mapIndex = mapIndex;
        this->nodeBiome = nodeBiome;
		// directionSet = directionSet;
        this->direction = direction;

		this->nodeColour = Colour();
		this->FloorType = FoliageFloor::Normal;

		// possibleRotations = new float[0];
		this->randomRotation = false;
		this->height = 0;
    }

	FoliageInfo& SetPossibleRotations(std::vector<float> values) {
		this->possibleRotations = values;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetRandomRotation(bool value) {
		this->randomRotation = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetFoliageType(FoliageType value) {
		this->foliageType = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetNodeType(int value) {
		this->nodeType = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetNodeBiome(LevelBiome value) {
		this->nodeBiome = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetFloorType(FoliageFloor value) {
		this->FloorType = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetDirection(Direction value) {
		this->direction = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetDirectionSet(const std::vector<FoliageType> &value) {
		this->directionSet = &value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetMapIndex(int value) {
		this->mapIndex = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetWalkable(bool value) {
		this->walkable = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetBuildable(bool value) {
		this->buildable = value;
		containsResourceData = true;
		return *this;
	}

    FoliageInfo& SetThrotten(ThrottenAmount value) {
		this->ThrottenValue = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetHeight(float value) {
		this->height = value;
		containsResourceData = true;
		return *this;
	}

	FoliageInfo& SetColour(Colour value) {
		this->nodeColour = value;
		containsResourceData = true;
		return *this;
	}
};