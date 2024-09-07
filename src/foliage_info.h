#pragma once

#include <vector>

#include "foliage.h"


class FoliageInfo {

    public:

    FoliageType foliageType;
    int nodeType;
	bool walkable;
	bool buildable;
	bool randomRotation;
	int mapIndex;
    LevelBiome nodeBiome;
	FoliageFloor floorType;

    const std::vector<FoliageType> *directionSet;

    Direction direction;

	Colour nodeColour;
	bool containsResourceData;

	std::vector<float> possibleRotations;
	float height;

    ThrottenAmount throttenValue;

	FoliageInfo() {
		containsResourceData = false;
		foliageType = FoliageType::Foliage_NoSelection;
		randomRotation = false;
		height = 0;
		floorType = FoliageFloor::Normal;
		nodeType = FoliageHelpers::FLOOR_NODE_TYPE;
		walkable = true;
		buildable = true;

		nodeBiome = LevelBiome::None;
        throttenValue = ThrottenAmount::None;
	}

    FoliageInfo(
			FoliageType foliageType,
			int nodeType,
			bool walkable,
			bool buildable,
			int mapIndex,
			LevelBiome nodeBiome=LevelBiome::None,
			Direction direction=Direction::DirectionNone) {

		this->containsResourceData = true;
        this->foliageType = foliageType;
        this->nodeType = nodeType;
		this->walkable = walkable;
		this->buildable = buildable;
		this->mapIndex = mapIndex;
        this->nodeBiome = nodeBiome;
        this->direction = direction;

		this->nodeColour = Colour();
		this->floorType = FoliageFloor::Normal;

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
		this->floorType = value;
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
		this->throttenValue = value;
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
