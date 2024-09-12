#include <catch2/catch_test_macros.hpp>

#include "gmath.h"


Vector2Int subtract_from_vec(Vector2Int vec, int x, int y) {
    return vec - Vector2Int(x,y);
}

Vector2Int subtract_equals_vec(Vector2Int vec, int x, int y) {
    vec -= Vector2Int(x,y);
    return vec;
}

TEST_CASE( "Gmath vectors work correctly", "[gmath]" ) {

    Vector2Int vec1 (0,0);
    Vector2Int vec2 (3,5);

    float dist = gmath::dist(vec1 - vec2);
    float sqrDist = gmath::square_dist(vec1 - vec2);

    SECTION("Vectors are correct.") {
        CHECK( vec1.x == 0 );
        CHECK( vec2.x == 3 );
        CHECK( vec2.y == 5 );
    }

    SECTION("Vector subtraction.") {
        CHECK( subtract_from_vec(vec2, 1,1).x == 2 );
        CHECK( subtract_from_vec(vec2, 1,1).y == 4 );
        CHECK( subtract_from_vec(vec2, 5,5).x == -2 );
        CHECK( subtract_from_vec(vec2, 5,5).y == 0 );
    }

    SECTION("Vector subtraction set.") {
        vec1 = subtract_equals_vec(vec1, -1,-4);
        vec2 = subtract_equals_vec(vec2, 1,1);
        CHECK( vec2.x == 2 );
        CHECK( vec2.y == 4 );
        CHECK( vec1.x == 1 );
        CHECK( vec1.y == 4 );
    }

    SECTION("Vector distance.") {
        CHECK( gmath::dist(vec1 - vec2) == 15.0 );
        CHECK( gmath::dist(vec2 - vec1) == 15.0 );
    }

    SECTION("Vector sqr distance.") {
        CHECK( gmath::square_dist(vec1 - vec2) == 225.0 );
        CHECK( gmath::square_dist(vec2 - vec1) == 225.0 );
    }
}
