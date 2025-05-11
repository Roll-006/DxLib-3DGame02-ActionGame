#pragma once
#include <concepts>
#include "../Concept/common_concepts.hpp"

template<typename ElemT>
struct Vector2D
{
    ElemT x;
    ElemT y;

    Vector2D  operator+()const              { return *this; }
    Vector2D  operator-()const              { return Vector2D(-x, -y); }

    Vector2D& operator= (const Vector2D& v) { x = v.x;  y = v.y;  return *this; }

    Vector2D& operator+=(const Vector2D& v) { x += v.x; y += v.y; return *this; }
    Vector2D& operator-=(const Vector2D& v) { x -= v.x; y -= v.y; return *this; }
    Vector2D& operator*=(const Vector2D& v) { x *= v.x; y *= v.y; return *this; }

    template<typename ScaleT>
    Vector2D& operator*=(ScaleT scale)      { return Vector2D(x * scale, y * scale); }
};

// å^êßå¿
namespace v2d_concepts
{
    template<typename T>
    concept ValidT = std::same_as<T, Vector2D<int>>
                  || std::same_as<T, Vector2D<long>>
                  || std::same_as<T, Vector2D<long long>>
                  || std::same_as<T, Vector2D<float>>
                  || std::same_as<T, Vector2D<double>>
                  || std::same_as<T, Vector2D<long double>>;
}

inline auto operator+ (const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2)    { return Vector2D(v1.x + v2.x, v1.y + v2.y); }
inline auto operator- (const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2)    { return Vector2D(v1.x - v2.x, v1.y - v2.y); }
inline auto operator* (const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2)    { return Vector2D(v1.x * v2.x, v1.y * v2.y); }

template<typename ScaleT>
inline auto operator* (const v2d_concepts::ValidT auto& v,  const ScaleT scale)                     { return Vector2D(v.x * scale, v.y * scale); }
template<typename ScaleT>
inline auto operator* (const ScaleT scale,                  const v2d_concepts::ValidT auto& v)     { return v * scale; }

inline bool operator==(const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2)    { return v1.x == v2.x && v1.y == v2.y; }
inline bool operator!=(const v2d_concepts::ValidT auto& v1, const v2d_concepts::ValidT auto& v2)    { return !(v1 == v2); }
