#pragma once
#include <cmath>
#include <DxLib.h>
#include "vector_3d.hpp"

struct Quaternion;

namespace quat
{
	enum class AxesKind
	{
		kX,
		kY,
		kZ,
		kW,
	};

	/// @brief クォータニオン型とFLOAT4型の変換
	[[nodiscard]] Quaternion ConvertFloat4ToQuaternion(const FLOAT4& f);
	[[nodiscard]] FLOAT4	 ConvertQuaternionToFloat4(const Quaternion& q);

	[[nodiscard]] Quaternion GetZeroQuaternion();
	[[nodiscard]] Quaternion GetIdentityQuaternion();
	[[nodiscard]] Quaternion CreateQuaternion		(const VECTOR& axis, const float angle);

	[[nodiscard]] float		 GetSize				(const Quaternion& q);
	[[nodiscard]] float		 GetSquareSize			(const Quaternion& q);
	[[nodiscard]] Quaternion GetNormalizedQuaternion(const Quaternion& q);

	/// @brief 共役クォータニオンを取得
	[[nodiscard]] Quaternion GetConjugateQuaternion	(const Quaternion& q);

	/// @brief 逆クォータニオンを取得
	[[nodiscard]] Quaternion GetInverseQuaternion	(const Quaternion& q);

	/// @brief 球面線形補間後のクォータニオンを取得
	/// @param begin_q 開始地点となるクォータニオン
	/// @param end_q 終了地点となるクォータニオン
	/// @param t 補間係数
	/// @return 補間結果クォータニオン
	[[nodiscard]] Quaternion GetSlerpQuaternion(const Quaternion& begin_q, const Quaternion& end_q, const float t);
}

/// @brief クォータニオン
/// @brief DxLibのクォータニオンはFLOAT4型で直感的でないため再定義
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;

	Quaternion& operator*=(const Quaternion& q)
	{
		FLOAT4 f1 = quat::ConvertQuaternionToFloat4(*this);
		FLOAT4 f2 = quat::ConvertQuaternionToFloat4(q);
		*this = quat::ConvertFloat4ToQuaternion(QTCross(f1, f2));
		return *this;
	}

	template<typename ScaleT>
	Quaternion& operator*=(const ScaleT scale) { return Quaternion(x * scale, y * scale, z * scale, w * scale); }
};

inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
	FLOAT4 f1 = quat::ConvertQuaternionToFloat4(q1);
	FLOAT4 f2 = quat::ConvertQuaternionToFloat4(q2);
	return Quaternion(quat::ConvertFloat4ToQuaternion(QTCross(f1, f2)));
}

template<typename ScaleT>
inline auto operator* (const Quaternion& q, const ScaleT scale) { return Quaternion(q.x * scale, q.y * scale, q.z * scale, q.w * scale); }
template<typename ScaleT>
inline auto operator* (const ScaleT scale, const Quaternion& q) { return q * scale; }

inline bool operator==(const Quaternion& q1, const Quaternion& q2) { return q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w; }
inline bool operator!=(const Quaternion& q1, const Quaternion& q2) { return !(q1 == q2); }
