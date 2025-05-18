#pragma once
#include <algorithm>
#include <array>
#include <optional>

#include "../Concept/common_concepts.hpp"

#include "Quaternion/quaternion.hpp"

#include "../Data/IncludeList/vector.hpp"

class Line;
class Segment;
class Plane;
class Triangle;
class Square;
class Sphere;
class Capsule;
class OBB;

namespace math
{
	static constexpr float kDegreesToRadian = DX_PI_F / 180.0f;		// ディグリーをラジアンに変換(変換対象と掛け算を行う)
	static constexpr float kRadianToDegrees = 180.0f / DX_PI_F;		// ラジアンをディグリーに変換(変換対象と掛け算を行う)


	#pragma region 変換
	/// @brief 四捨五入
	/// @param value 対象数字
	/// @param digit 概数を取る桁数。負の値は小数点以下を示すものとする。
	/// 
	/// 例1) value : 2.45, digit : -1　➡　return 2.5
	/// 
	/// 例2) value : 1.73, digit : 0   ➡　return 1.73(そのまま返す)
	template<typename T>
	[[nodiscard]] T RoundOff(const T value, int digit)
	{
		// 0桁目は存在しないためそのまま返す
		if (digit == 0) { return value; }

		if (digit > 0) { --digit; }

		float adjust = static_cast<float>(std::pow(10, digit));
		return std::round(value / adjust) * adjust;
	}

	template<>
	inline [[nodiscard]] VECTOR RoundOff(const VECTOR value, int digit)
	{
		return VGet(RoundOff(value.x, digit), RoundOff(value.y, digit), RoundOff(value.z, digit));
	}

	/// @brief 0～1の間に変換した値を取得
	/// @tparam T 変換対象の型
	/// @tparam ReturnValue 戻り値の型(浮動小数点数型である必要有り)
	/// @param min 値の最小値(この値を0とする)
	/// @param max 値の最大値(この値を1とする)
	/// @param value 変換対象の値
	template<typename T, common_concepts::FloatingPointT ReturnT>
	[[nodiscard]] ReturnT GetUnitValue(const T min, const T max, const T value)
	{
		return static_cast<ReturnT>(value - min) / (max - min);
	}

	/// @brief クォータニオンから回転行列へ変換
	/// @brief TODO : 検証が必要
	MATRIX ConvertQuaternionToMatrix(const Quaternion& q);

	/// @brief 回転行列からクォータニオンへ変換
	/// @brief TODO : 検証が必要
	Quaternion ConvertMatrixToQuaternion(const MATRIX& mat);
	#pragma endregion


	#pragma region 平均値
	/// @brief 値の平均を取得
	/// @brief 呼び出す際は「GetAverageValue<戻り値型>(値...);」とする
	template<common_concepts::FloatingPointT ReturnT, common_concepts::ArithmeticT T, common_concepts::ArithmeticT... Args>
	[[nodiscard]] inline ReturnT GetAverageValue(T first, Args... args)
	{
		constexpr int length = 1 + sizeof...(args);
		ReturnT sum = static_cast<ReturnT>(first) + (static_cast<ReturnT>(args) + ...);

		return sum / length;
	}
	#pragma endregion


	#pragma region 三平方の定理
	/// @brief 直角二等辺三角形の底辺(又は高さ)の長さを取得
	/// @param hypotenuse_length 斜辺の長さ
	[[nodiscard]] float GetAdjacentLengthIsoscelesRightTriangle(float hypotenuse_length);

	/// @brief 直角二等辺三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺(又は高さ)の長さ
	[[nodiscard]] float GetHypotenuseLengthIsoscelesRightTriangle(float adjacent_length);

	/// @brief 直角三角形の底辺(又は高さ)の長さを取得
	/// @param opposite_length 高さ(又は底辺の長さ)
	/// @param hypotenuse_length 斜辺の長さ
	[[nodiscard]] float GetAdjacentLengthRightTriangle(float opposite_length, float hypotenuse_length);

	/// @brief 直角三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺の長さ
	/// @param opposite_length 高さ
	[[nodiscard]] float GetHypotenuseLengthRightTriangle(float adjacent_length, float opposite_length);
	#pragma endregion


	#pragma region ベクトル
	/// @brief ベクトル同士が平行かを判定
	[[nodiscard]] bool IsHorizontal(const VECTOR& vector1, const VECTOR& vector2);

	/// @brief ベクトル同士が垂直かを判定
	[[nodiscard]] bool IsVertical(const VECTOR& vector1, const VECTOR& vector2);

	/// @brief 基準とするベクトルから法線ベクトルを求め、その内の一つを取得
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& vector);

	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& vector1, const VECTOR& vector2);
	#pragma endregion


	#pragma region 角度
	/// @brief 鋭角かを判定
	[[nodiscard]] bool IsAcuteAngle(const VECTOR& vector1, const VECTOR& vector2);
	[[nodiscard]] bool IsAcuteAngle(const float radian);

	/// @brief 二つのベクトルのなす角を取得
	[[nodiscard]] float GetAngleBetweenTwoVector(const VECTOR& vector1, const VECTOR& vector2);
	#pragma endregion


	#pragma region 回転
	/// @brief 回転後の座標を取得
	VECTOR GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q);

	/// @brief ヨー角を取得
	[[nodiscard]] float GetYaw(const VECTOR& vector);

	/// @brief ヨー角回転ベクトルを取得
	[[nodiscard]] VECTOR GetYawRotateVector(const VECTOR& vector);
	#pragma endregion


	#pragma region 重心
	/// @brief 三角形の物理的、および幾何学的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfATriangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3);

	/// @brief 四角形の物理的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfAQuadrilateral(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4);
	#pragma endregion


	#pragma region 図形同士の関係
	/// @brief 直線と直線の交点を取得
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合、もしくは二直線が同一直線上にある場合はnullopt(if(戻り値)で判定する必要あり)
	[[nodiscard]] std::optional<VECTOR> GetIntersectionLineAndLine(const Line* line1, const Line* line2);

	/// @brief 線分と平面(無限に広がる面)の交点を取得
	/// @brief 受け取り側で、戻り値がnulloptである場合と不定値である場合を考慮する必要あり
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合はnullopt(if(戻り値){}で判定する必要あり)
	/// @return 線分と平面が同一平面上にある場合は不定値(受け取り側でstd::isfinite()を使用する必要あり)
	[[nodiscard]] std::optional<VECTOR> GetIntersectionSegmentAndPlane(const Segment* segment, const Plane* plane);

	/// @brief 二直線が同一直線上にあるかを判定
	[[nodiscard]] bool IsSameLine(const Line* line1, const Line* line2);

	/// @brief 線分に最も近い平面上の座標を取得
	[[nodiscard]] VECTOR GetClosestPosOnPlaneFromSegment(const Plane* plane, const Segment* segment);

	/// @brief 平面に最も近い線分上の座標を取得
	[[nodiscard]] VECTOR GetClosestPosOnSegmentFromPlane(const Segment* segment, const Plane* plane);

	/// @brief 点が平面の前方にあるかを判定
	/// @return 前方 : true, 後方もしくは平面に含まれる : false
	[[nodiscard]] bool IsPointAheadOfPlane(const VECTOR& point, const Plane* plane);

	/// @brief 点が球の表面にあるかを判定
	/// TODO : 正しく機能するか試していないため検証が必要
	[[nodiscard]] bool IsPointOnSphereSurface(const VECTOR& point, const Sphere* sphere);
	#pragma endregion


	#pragma region 最短距離
	/// @brief 点と直線の最短距離を取得
	[[nodiscard]] float GetDistancePointToLine			(const VECTOR&		point,		const Line*		line);
	float               GetDistancePointToLine			(const VECTOR&		point,		const Line*		line,		VECTOR& h, float& t);

	/// @brief 点と線分の最短距離を取得
	[[nodiscard]] float GetDistancePointToSegment		(const VECTOR&		point,		const Segment*	segment);
	float               GetDistancePointToSegment		(const VECTOR&		point,		const Segment*	segment,	VECTOR& h, float& t);

	/// @brief 点と平面(無限に広がる面)の最短距離を取得
	[[nodiscard]] float GetDistancePointToPlane			(const VECTOR&		point,		const Plane*	plane);

	/// @brief 点と三角形の最短距離を取得
	[[nodiscard]] float GetDistancePointToTriangle		(const VECTOR&		point,		const Triangle* triangle);

	/// @brief 点と四角形の最短距離を取得
	[[nodiscard]] float GetDistancePointToSquare		(const VECTOR&		point,		const Square*	square);

	/// @brief 直線と直線の最短距離を取得
	[[nodiscard]] float GetDistanceLineToLine			(const Line*		line1,		const Line*		line2);
	float               GetDistanceLineToLine			(const Line*		line1,		const Line*		line2,		VECTOR& h1, VECTOR& h2, float& t1, float& t2);

	/// @brief 線分と線分の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToSegment		(const Segment*		segment1,	const Segment*	segment2);
	float               GetDistanceSegmentToSegment		(const Segment*		segment1,	const Segment*	segment2,	VECTOR& h1, VECTOR& h2, float& t1, float& t2);

	/// @brief 線分と平面(無限に広がる面)の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToPlane		(const Segment*		segment,	const Plane*	plane);

	/// @brief 線分と三角形の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToTriangle	(const Segment*		segment,	const Triangle* triangle);

	/// @brief 線分と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToSquare		(const Segment*		segment,	const Square*	square);

	/// @brief 平面とカプセルの最短距離を取得
	[[nodiscard]] float GetDistancePlaneToCapsule		(const Plane*		plane,		const Capsule*	capsule);

	/// @brief 三角形と三角形の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToTriangle	(const Triangle*	triangle1,	const Triangle* triangle2);

	/// @brief 三角形と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToSquare		(const Triangle*	triangle,	const Square*	square);

	/// @brief 四角形と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceSquareToSquare		(const Square*		square1,	const Square*	square2);

	/// @brief 四角形とカプセルの最短距離
	[[nodiscard]] float GetDistanceSquareToCapsule		(const Square*		square,		const Capsule*	capsule);
	#pragma endregion
}
