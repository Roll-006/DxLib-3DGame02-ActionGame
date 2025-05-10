#pragma once
#include <concepts>
#include <DxLib.h>

// å^êßå¿
namespace v3d_concepts
{
	template<typename T>
	concept ValidT = std::same_as<T, VECTOR>
				  || std::same_as<T, VECTOR_D>;
}

inline auto operator+ (const v3d_concepts::ValidT auto& v)										{ return v; }
template<v3d_concepts::ValidT VecT>
inline auto operator- (const VecT& v)															{ return VecT(-v.z, -v.y, -v.z); }

template<v3d_concepts::ValidT VecT>
inline auto operator+ (const VecT& v1, const VecT& v2)											{ return VecT(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
template<v3d_concepts::ValidT VecT>
inline auto operator- (const VecT& v1, const VecT& v2)											{ return VecT(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
template<v3d_concepts::ValidT VecT>
inline auto operator* (const VecT& v1, const VecT& v2)											{ return VecT(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

template<v3d_concepts::ValidT VecT, typename ScaleT>
inline auto operator* (const VecT& v, const ScaleT scale)										{ return VecT(v.x * scale, v.y * scale, v.z * scale); }
template<typename ScaleT>
inline auto operator* (const ScaleT scale, const v3d_concepts::ValidT auto& v)					{ return v * scale; }

inline auto operator+=(v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2)		{ v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }
inline auto operator-=(v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2)		{ v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }
inline auto operator*=(v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2)		{ v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }

template<typename ScaleT>
inline auto operator*=(v3d_concepts::ValidT auto& v, const ScaleT scale)						{ v.x *= scale; v.y *= scale; v.z *= scale; return v; }

inline bool operator==(const v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2){ return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
inline bool operator!=(const v3d_concepts::ValidT auto& v1, const v3d_concepts::ValidT auto& v2){ return !(v1 == v2); }

