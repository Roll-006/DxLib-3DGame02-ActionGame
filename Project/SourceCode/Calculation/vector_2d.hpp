#pragma once
#include <cmath>
#include <nlohmann/json.hpp>
#include "../Concept/common_concepts.hpp"

template<common_concepts::ArithmeticT ElemT>
struct Vector2D
{
    ElemT x;
    ElemT y;

    Vector2D  operator+() const { return *this; }
	Vector2D  operator-() const { return { -x, -y }; }

	template<common_concepts::ArithmeticT T>
    Vector2D& operator= (const Vector2D<T>& v) { x  = static_cast<ElemT>(v.x);   y  = static_cast<ElemT>(v.y);   return *this; }

	template<common_concepts::ArithmeticT T>
    Vector2D& operator+=(const Vector2D<T>& v) { x += static_cast<ElemT>(v.x);   y += static_cast<ElemT>(v.y);   return *this; }

	template<common_concepts::ArithmeticT T>
    Vector2D& operator-=(const Vector2D<T>& v) { x -= static_cast<ElemT>(v.x);   y -= static_cast<ElemT>(v.y);   return *this; }

	template<common_concepts::ArithmeticT T>
    Vector2D& operator*=(const Vector2D<T>& v) { x *= static_cast<ElemT>(v.x);   y *= static_cast<ElemT>(v.y);   return *this; }

	template<common_concepts::ArithmeticT ScaleT>
	Vector2D& operator*=(const ScaleT scale)   { x *= static_cast<ElemT>(scale); y *= static_cast<ElemT>(scale); return *this; }
};

template<common_concepts::ArithmeticT T,	common_concepts::ArithmeticT U>
inline const auto operator+ (const Vector2D<T>& v1,		const Vector2D<U>& v2)		{ return Vector2D<decltype(v1.x + v2.x)>{ v1.x + v2.x, v1.y + v2.y }; }

template<common_concepts::ArithmeticT T,	common_concepts::ArithmeticT U>
inline const auto operator- (const Vector2D<T>& v1,		const Vector2D<U>& v2)		{ return Vector2D<decltype(v1.x - v2.x)>{ v1.x - v2.x, v1.y - v2.y }; }

template<common_concepts::ArithmeticT T,	common_concepts::ArithmeticT U>
inline const auto operator* (const Vector2D<T>& v1,		const Vector2D<U>& v2)		{ return Vector2D<decltype(v1.x * v2.x)>{ v1.x * v2.x, v1.y * v2.y }; }

template<common_concepts::ArithmeticT VecT, common_concepts::ArithmeticT ScaleT>
inline const auto operator* (const Vector2D<VecT>& v,	const ScaleT scale)			{ return Vector2D<VecT>{ static_cast<VecT>(v.x * scale), static_cast<VecT>(v.y * scale) }; }

template<common_concepts::ArithmeticT VecT, common_concepts::ArithmeticT ScaleT>
inline const auto operator* (const ScaleT scale,		const Vector2D<VecT>& v)	{ return v * scale; }

template<common_concepts::ArithmeticT T,	common_concepts::ArithmeticT U>
inline const bool operator==(const Vector2D<T>& v1,		const Vector2D<U>& v2)		{ return v1.x == v2.x && v1.y == v2.y; }

template<common_concepts::ArithmeticT T,	common_concepts::ArithmeticT U>
inline const bool operator!=(const Vector2D<T>& v1,		const Vector2D<U>& v2)		{ return !(v1 == v2); }


namespace v2d
{
	/// @brief ベクトルのサイズ(長さ)を取得
	template<common_concepts::ArithmeticT U>
	[[nodiscard]] inline const float GetSize(const Vector2D<U>& v) { return sqrtf(v.x * v.x + v.y * v.y); }

	/// @brief ベクトルのサイズ(長さ)の2乗を取得
	template<common_concepts::ArithmeticT U>
	[[nodiscard]] inline const float GetSquareSize(const Vector2D<U>& v) { return v.x * v.x + v.y * v.y; }

	/// @brief ゼロベクトルを取得
	template<common_concepts::ArithmeticT U>
	[[nodiscard]] inline const Vector2D<U> GetZeroV() { return { 0, 0 }; }

	/// @brief 正規化後のベクトルを取得
	template<common_concepts::ArithmeticT U>
	[[nodiscard]] inline const Vector2D<U> GetNormalizedV(const Vector2D<U>& v)
	{
		auto size = GetSize(v);
		return size != 0 ? Vector2D<U>(v.x / size, v.y / size) : v;
	}
	
	/// @brief ベクトルの内積を取得
	template<common_concepts::ArithmeticT U>
	[[nodiscard]] inline const float GetDot(const Vector2D<U>& v1, const Vector2D<U>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
}


#pragma region from / to JSON
template<common_concepts::ArithmeticT U>
inline void from_json(const nlohmann::json& j_data, Vector2D<U>& v)
{
	j_data.at("x").get_to(v.x);
	j_data.at("y").get_to(v.y);
}

template<common_concepts::ArithmeticT U>
inline void to_json(nlohmann::json& j_data, const Vector2D<U>& v)
{
	j_data = nlohmann::json
	{
		{ "x",	v.x },
		{ "y",	v.y }
	};
}
#pragma endregion
