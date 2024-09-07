#pragma once

#include <string>

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

    // bool operator==(const vector2Int& other) const {
    // 	return x == other.x && y == other.y;
    // }

    Vector2Int operator-(const Vector2Int& other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    std::string to_string() const {
        return std::to_string(x) + std::to_string(y);
    }

    // size_t hash() const {
    //     return std::hash<std::string>()(toString());
    // }
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
