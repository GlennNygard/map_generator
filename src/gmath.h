#pragma once

#include <string>
#include <format>
#include <cmath>


template <class T>
inline void hash_combine(std::size_t & s, const T & v)
{
    std::hash<T> h;
    s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
}

struct Vector2Int {
    int x;
    int y;

    Vector2Int() {
        x = 0;
        y = 0;
    }

    Vector2Int(int newX, int newY) {
        x = newX;
        y = newY;
    }

    auto operator<=>(const Vector2Int&) const = default;

    Vector2Int operator-(const Vector2Int& other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    std::string to_string() const {
        return std::format("({},{})", std::to_string(x), std::to_string(y));
    }
};

template <class T>
class Vector2IntHash;

template<>
struct std::hash<Vector2Int> {
    std::size_t operator()(const Vector2Int& s) const noexcept {
        std::size_t res = 0;
        hash_combine(res,s.x);
        hash_combine(res,s.y);
        return res;
    }
};

template<size_t size>
static int get_sum(std::array<int, size> arr) {
    int sum = 0;
    for(int val : arr) {
        sum += val;
    }
    return sum;
}

static float square_dist(Vector2Int vec) {
    return (vec.x*vec.x* + vec.y*vec.y);
}

static float dist(Vector2Int vec) {
    return std::sqrt(vec.x*vec.x* + vec.y*vec.y);
}

static float ease_out_cubic(float t) {
    // Fast then slow.
    float tMinus = t-1;
    return tMinus*t*t+1;
}
