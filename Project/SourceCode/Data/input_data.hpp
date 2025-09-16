#pragma once
#include <functional>
#include <unordered_map>

#include "../IncludeList/vector.hpp"
#include "../Kind/input_kind.hpp"

struct InputCode
{
	InputKind	kind = InputKind::kKey;
	int			code = 0;
};

struct InputData
{
	bool  is_input		= false;
	float input_time	= 0.0f;
};

struct MouseData
{
	Vector2D<int>   pos				= v2d::GetZeroV<Vector2D<int>>();
	Vector2D<float> dir				= v2d::GetZeroV<Vector2D<float>>();
	Vector2D<float> velocity		= v2d::GetZeroV<Vector2D<float>>();
	int				wheel_rotation	= 0;
};


inline bool operator==(const InputCode& lhs, const InputCode& rhs)
{
    return lhs.kind == rhs.kind && lhs.code == rhs.code;
}

namespace std
{
    template <>
    struct hash<InputCode>
    {
        std::size_t operator()(const InputCode& code) const noexcept
        {
            // 適当なハッシュ結合
            std::size_t h1 = std::hash<InputKind>{}(code.kind);
            std::size_t h2 = std::hash<int>{}(code.code);

            // 結合方法：XORとシフト
            return h1 ^ (h2 << 1);
        }
    };
}
