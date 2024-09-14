#pragma once


using FoliageType = uint16_t;
using FoliageIndex = int;

struct Colour {
    uint8_t r,g,b;
    Colour() {r = 100; g = 0; b = 10;};
    Colour(uint8_t r, uint8_t g, uint8_t b){
        this->r = r; this->g = g; this->b = b;};
    Colour(double r, double g, double b){
        this->r = static_cast<uint8_t>(r*255.0);
        this->g = static_cast<uint8_t>(g*255.0);
        this->b = static_cast<uint8_t>(b*255.0);
    };
} __attribute__((__packed__));


enum class ThrottenAmount {
    None,
    Level01,
    Level02,
    Completely,
};

enum class FoliageFloor {
    Normal,
    High,
    Higher,
    Low,
};

enum class Direction {
    DirectionNone,
    DirectionUp,
    DirectionDown,
    DirectionLeft,
    DirectionRight,
    DirectionUpLeft,
    DirectionUpRight,
    DirectionDownLeft,
    DirectionDownRight,
};

enum class MapSize {
    MapSize_Smaller,
    MapSize_Small,
    MapSize_Medium,
    MapSize_Large,
    MapSize_VeryLarge,
    MapSize_IncrediblyLarge,
    MapSize_Massive,
};

// Must keep order.
enum class LevelBiome {
    None,
    Grass,
    Desert,
    Forrest,
    Snow,

    // <- Insert elements here.

    // Always keep at bottom.
    Debug,
};

class FoliageHelpers {

    public:

    static const size_t MAX_FOLIAGE_COUNT = 144;
    static const FoliageType NO_SELECTION_INDEX = 0;
    static const FoliageType NO_FOLIAGE_INDEX = 1;

    static const int FLOOR_NODE_TYPE = 0;
    static const int HIGH_GROUND_NODE_TYPE = 1;
    static const int LOW_GROUND_NODE_TYPE = 2;
    static const int BORDER_NODE_TYPE = 3;
    enum class FoliageFloor {
        Normal,
        High,
        Higher,
        Low,
    };
};
