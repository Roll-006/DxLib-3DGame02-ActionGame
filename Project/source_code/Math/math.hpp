#pragma once
#include <array>

#include "Vector/vector.hpp"
#include "Quaternion/quaternion.hpp"

//// TODO : 関数今後も増え続けると思われるため分離を検討中
namespace math
{
	static constexpr float kDegreesToRadian = DX_PI_F / 180.0f;		// ディグリーをラジアンに変換(変換対象と掛け算を行う)
	static constexpr float kRadianToDegrees = 180.0f / DX_PI_F;		// ラジアンをディグリーに変換(変換対象と掛け算を行う)

	/// @brief 0～1の間に変換した値を取得
	/// @tparam T 変換対象の型
	/// @tparam RetT 戻り値の型(浮動小数点数型である必要有り)
	/// @param min 値の最小値(この値を0とする)
	/// @param max 値の最大値(この値を1とする)
	/// @param value 変換対象の値
	template<typename T, common_concepts::FloatingPointT RetT>
	[[nodiscard]] RetT GetUnitValue(const T min, const T max, const T value)
	{
		return static_cast<RetT>(value - min) / (max - min);
	}

	/// @brief クォータニオンから回転行列へ変換
	/// @brief TODO : 検証が必要
	MATRIX ConvertQuaternionToMatrix(const Quaternion& q);

	/// @brief 回転行列からクォータニオンへ変換
	/// @brief TODO : 検証が必要
	Quaternion ConvertMatrixToQuaternion(const MATRIX& mat);

	/// @brief 回転後の座標を取得
	VECTOR GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q);
}
