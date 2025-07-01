#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <unordered_map>

#include "../Concept/common_concepts.hpp"

#include "axes.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

#include "../Data/Kind/time_kind.hpp"
#include "../Data/Kind/sort_kind.hpp"
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
	static constexpr float kEpsilonLow		= 1e-4f;				// 0とみなす(低精度)
	static constexpr float kEpsilonHigh		= 1e-6f;				// 0とみなす(高精度)


	#pragma region 変換
	/// @brief 四捨五入
	/// @param value 対象数字
	/// @param digit 概数を取る桁数。負の値は小数点以下を示すものとする。
	/// 
	/// 例1) value : 2.45, digit : -1　➡　return 2.5
	/// 
	/// 例2) value : 1.73, digit : 0   ➡　return 1.73(そのまま返す)
	template<typename T>
	[[nodiscard]] T RoundOff(const T& value, int digit)
	{
		// 0桁目は存在しないためそのまま返す
		if (digit == 0) { return value; }

		if (digit > 0) { --digit; }

		const float adjust = static_cast<float>(std::pow(10, digit));
		return std::round(value / adjust) * adjust;
	}

	template<>
	inline [[nodiscard]] VECTOR RoundOff(const VECTOR& value, int digit)
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

	/// @brief std::vector<std::pair<key, value>>をstd::unorderd_map<key, value>へ変換
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> ConvertPairsToUmap(const std::vector<std::pair<KeyT, ValueT>>& pairs)
	{
		std::unordered_map<KeyT, ValueT> u_map(pairs.begin(), pairs.end());
		return u_map;
	}

	/// @brief std::unorderd_map<key, value>をstd::vector<std::pair<key, value>>へ変換
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::vector<std::pair<KeyT, ValueT>> ConvertUmapToPairs(const std::unordered_map<KeyT, ValueT>& u_map)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs(u_map.begin(), u_map.end());
		return pairs;
	}

	/// @brief 回転行列からクォータニオンへ変換
	/// @brief TODO : 検証が必要
	[[nodiscard]] Quaternion ConvertRotMatrixToQuaternion(const MATRIX& mat);

	/// @brief クォータニオンから回転行列へ変換
	[[nodiscard]] MATRIX ConvertQuaternionToRotMatrix(const MATRIX& mat, const Quaternion& q);

	/// @brief XYZ軸から回転行列へ変換
	/// @param axes 行列に変換するXYZ軸
	/// @param parent_axes 基準とする親XYZ軸
	[[nodiscard]] MATRIX ConvertAxesToXYZRotMatrix(const Axes& axes, const Axes& parent_axes);

	/// @brief XYZ軸からオイラー角へ変換
	/// @brief FIXME : 不具合ありな可能性あり。要検証
	[[nodiscard]] VECTOR ConvertAxesToEulerAngles(const Axes& axes, const Axes& parent_axes);

	/// @brief 回転行列からオイラー角へ変換
	/// @brief FIXME : 不具合ありな可能性あり。要検証
	[[nodiscard]] VECTOR ConvertRotMatrixToEulerAngles(const MATRIX& mat);
	[[nodiscard]] VECTOR ConvertRotMatrixToEulerAngles(const MATRIX& mat, bool& is_gimbal_lock);

	/// @brief 回転行列からXYZ軸へ変換
	[[nodiscard]] Axes ConvertRotMatrixToAxes(const MATRIX& mat);

	/// @brief オイラー角から回転行列へ変換
	[[nodiscard]] MATRIX ConvertEulerAnglesToRotMatrix(const VECTOR& angle);
	#pragma endregion


	#pragma region ソート
	/// @brief unorderd_mapのvalueをソート
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> Sort(const std::unordered_map<KeyT, ValueT>& u_map, const SortKind sort_kind)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs = ConvertUmapToPairs(u_map);

		std::sort(pairs.begin(), pairs.end(), [=](const auto& a, const auto& b)
		{
			switch (sort_kind)
			{
			case SortKind::kAscending:	return a.second < b.second; break;
			case SortKind::kDescending:	return a.second > b.second;	break;
			}
		});

		return ConvertPairsToUmap(pairs);
	}

	/// @brief unorderd_mapのvalueをソート
	/// @brief main_u_mapとsub_u_mapは同じデータ型、同じ個数である必要あり
	/// @param main_u_map ソートされるunorderd_map
	/// @param sub_u_map main_u_mapに同じvalueがあった場合、sub_u_mapでソートを行う
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> Sort(const std::unordered_map<KeyT, ValueT>& main_u_map, const std::unordered_map<KeyT, ValueT>& sub_u_map, const SortKind sort_kind)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs = ConvertUmapToPairs(main_u_map);

		std::sort(pairs.begin(), pairs.end(), [=](const auto& a, const auto& b)
		{
			switch (sort_kind)
			{
			case SortKind::kAscending:
				if (a.second != b.second) { return a.second < b.second; }

				// main_u_mapが同じ値であった場合はsub_u_mapでソート
				return sub_u_map.at(a.first) < sub_u_map.at(b.first);
				break;

			case SortKind::kDescending:
				if (a.second != b.second) { return a.second > b.second; }

				return sub_u_map.at(a.first) > sub_u_map.at(b.first);
				break;
			}
		});

		return ConvertPairsToUmap(pairs);
	}
	#pragma endregion


	#pragma region 平均値
	/// @brief 値の平均を取得
	/// @brief 呼び出す際は「GetAverageValue<戻り値型>(値1, 値2...);」とする
	template<common_concepts::FloatingPointT ReturnT, common_concepts::ArithmeticT T, common_concepts::ArithmeticT... Args>
	[[nodiscard]] inline ReturnT GetAverageValue(T first, Args... args)
	{
		const int length = 1 + sizeof...(args);
		const ReturnT sum = static_cast<ReturnT>(first) + (static_cast<ReturnT>(args) + ...);

		return sum / length;
	}
	#pragma endregion


	#pragma region 値の変化
	/// @brief 値を増加させる
	/// @param value 増加させる値
	/// @param increase_value 増加量
	/// @param max_value 最大値
	template<common_concepts::ArithmeticT T>
	void Increase(T& value, const T& increase_value, const T& max_value)
	{
		if (value == max_value) { return; }

		value += increase_value;
		if (value > max_value)
		{
			value = max_value;
		}
	}

	/// @brief 値を増加させる(ループ用)
	/// @param value 増加させる値
	/// @param increase_value 増加量
	/// @param max_value 最大値
	/// @param is_loop ループ判定
	template<common_concepts::ArithmeticT T>
	void IncreaseLoop(T& value, const T& increase_value, const T& max_value, const bool is_loop)
	{
		value += increase_value;
		if (value > max_value)
		{
			value = is_loop ? 0 : max_value;
		}
	}

	/// @brief 値を減少させる
	/// @param value 減少させる値
	/// @param decrease_value 減少量
	/// @param min_value 最小値
	template<common_concepts::ArithmeticT T>
	void Decrease(T& value, const T& decrease_value, const T& min_value)
	{
		if (value == min_value) { return; }

		value -= decrease_value;
		if (value < min_value)
		{
			value = min_value;
		}
	}
	
	/// @brief 値を減少させる(ループ用)
	/// @param value 減少させる値
	/// @param decrease_value 減少量
	/// @param min_value 最小値
	/// @param is_loop ループ判定
	template<common_concepts::ArithmeticT T>
	void DecreaseLoop(T& value, const T& decrease_value, const T& min_value, const bool is_loop)
	{
		value -= decrease_value;
		if (value < min_value)
		{
			value = is_loop ? 0 : min_value;
		}
	}
	#pragma endregion


	#pragma region 値の修正
	/// @brief 角度が-π～πの値をループするように繋ぎ合わせる
	float ConnectMinusPiToPi(const float angle);
	#pragma endregion


	#pragma region 三平方の定理
	/// @brief 直角二等辺三角形の底辺(又は高さ)の長さを取得
	/// @param hypotenuse_length 斜辺の長さ
	[[nodiscard]] float GetAdjacentLengthIsoscelesRightTriangle(const float hypotenuse_length);

	/// @brief 直角二等辺三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺(又は高さ)の長さ
	[[nodiscard]] float GetHypotenuseLengthIsoscelesRightTriangle(const float adjacent_length);

	/// @brief 直角三角形の底辺(又は高さ)の長さを取得
	/// @param opposite_length 高さ(又は底辺の長さ)
	/// @param hypotenuse_length 斜辺の長さ
	[[nodiscard]] float GetAdjacentLengthRightTriangle(const float opposite_length, const float hypotenuse_length);

	/// @brief 直角三角形の斜辺の長さを取得
	/// @param adjacent_length 底辺の長さ
	/// @param opposite_length 高さ
	[[nodiscard]] float GetHypotenuseLengthRightTriangle(const float adjacent_length, const float opposite_length);
	#pragma endregion


	#pragma region ベクトル
	/// @brief ベクトル同士が平行かを判定
	[[nodiscard]] bool IsHorizontal(const VECTOR& v1, const VECTOR& v2);

	/// @brief ベクトル同士が垂直かを判定
	[[nodiscard]] bool IsVertical(const VECTOR& v1, const VECTOR& v2);

	/// @brief 基準とするベクトルから法線ベクトルを求め、その内の一つを取得
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& v);
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& v1, const VECTOR& v2);

	/// @brief 正射影ベクトル
	/// @param projected_v 投影元ベクトル
	/// @param base_v 投影先ベクトル
	/// @return 投影されたベクトル
	[[nodiscard]] VECTOR GetProjectionVector(const VECTOR& projected_v, const VECTOR& base_v);
	#pragma endregion


	#pragma region 角度
	/// @brief 鋭角かを判定
	[[nodiscard]] bool IsAcuteAngle(const VECTOR& v1, const VECTOR& v2);
	[[nodiscard]] bool IsAcuteAngle(const float radian);

	/// @brief 二つのベクトルのなす角を取得
	[[nodiscard]] float GetAngleBetweenTwoVector(const VECTOR& v1, const VECTOR& v2);
	#pragma endregion


	#pragma region 回転
	/// @brief 回転後の座標を取得
	[[nodiscard]] VECTOR GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q);

	/// @brief ヨー角(Y軸回転)を取得
	[[nodiscard]] float GetYaw  (const VECTOR& v);

	/// @brief ヨー角(Y軸)回転ベクトルを取得
	[[nodiscard]] VECTOR GetYawRotVector  (const VECTOR& v);

	/// @brief XYZ軸を取得
	/// @param dir 向きベクトル(この値をZ軸とする)
	/// @param parent_axes 親とするXYZ軸
	[[nodiscard]] Axes GetAxes(const VECTOR& dir, const Axes& parent_axes);
	#pragma endregion


	#pragma region 重心
	/// @brief 三角形の物理的、および幾何学的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfATriangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3);
	[[nodiscard]] VECTOR GetCentroidOfATriangle(const Triangle& triangle);

	/// @brief 四角形の物理的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfAQuadrilateral(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4);
	[[nodiscard]] VECTOR GetCentroidOfAQuadrilateral(const Square& square);
	#pragma endregion


	#pragma region 図形同士の関係
	/// @brief TODO : 衝突判定との合成を検討
	/// @brief 直線と直線の交点を取得
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合、もしくは二直線が同一直線上にある場合はnullopt(if(戻り値)で判定する必要あり)
	[[nodiscard]] std::optional<VECTOR> GetIntersectionLineAndLine(const Line& line1, const Line& line2);

	/// @brief TODO : 衝突判定との合成を検討
	/// @brief 線分と平面(無限に広がる面)の交点を取得
	/// @brief 受け取り側で、戻り値がnulloptである場合と不定値である場合を考慮する必要あり
	/// @return 一点で交差する場合は交点
	/// @return 該当する値がない場合はnullopt(if(戻り値){}で判定する必要あり)
	/// @return 線分と平面が同一平面上にある場合は不定値(受け取り側でstd::isfinite()を使用する必要あり)
	[[nodiscard]] std::optional<VECTOR> GetIntersectionSegmentAndPlane(const Segment& segment, const Plane& plane);

	/// @brief 二直線が同一直線上にあるかを判定
	[[nodiscard]] bool IsSameLine(const Line& line1, const Line& line2);

	/// @brief 線分に最も近い平面上の座標を取得
	[[nodiscard]] VECTOR GetClosestPosOnPlaneFromSegment(const Plane& plane, const Segment& segment);

	/// @brief 平面に最も近い線分上の座標を取得
	[[nodiscard]] VECTOR GetClosestPosOnSegmentFromPlane(const Segment& segment, const Plane& plane);

	/// @brief 点が平面の前方にあるかを判定
	/// @return 前方 : true, 後方もしくは平面に含まれる : false
	[[nodiscard]] bool IsPointAheadOfPlane(const VECTOR& point, const Plane& plane);

	/// @brief 点が球の表面にあるかを判定
	/// TODO : 正しく機能するか試していないため検証が必要
	[[nodiscard]] bool IsPointOnSphereSurface(const VECTOR& point, const Sphere& sphere);
	#pragma endregion


	#pragma region 最短距離
	/// @brief 点と直線の最短距離を取得
	[[nodiscard]] float GetDistancePointToLine			(const VECTOR&		point,		const Line&		line);
	float               GetDistancePointToLine			(const VECTOR&		point,		const Line&		line,		VECTOR& h, float& t);

	/// @brief 点と線分の最短距離を取得
	[[nodiscard]] float GetDistancePointToSegment		(const VECTOR&		point,		const Segment&	segment);
	float               GetDistancePointToSegment		(const VECTOR&		point,		const Segment&	segment,	VECTOR& h, float& t);

	/// @brief 点と平面(無限に広がる面)の最短距離を取得
	[[nodiscard]] float GetDistancePointToPlane			(const VECTOR&		point,		const Plane&	plane);

	/// @brief 点と三角形の最短距離を取得
	[[nodiscard]] float GetDistancePointToTriangle		(const VECTOR&		point,		const Triangle& triangle);

	/// @brief 点と四角形の最短距離を取得
	[[nodiscard]] float GetDistancePointToSquare		(const VECTOR&		point,		const Square&	square);

	/// @brief 直線と直線の最短距離を取得
	[[nodiscard]] float GetDistanceLineToLine			(const Line&		line1,		const Line&		line2);
	float               GetDistanceLineToLine			(const Line&		line1,		const Line&		line2,		VECTOR& h1, VECTOR& h2, float& t1, float& t2);

	/// @brief 線分と線分の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToSegment		(const Segment&		segment1,	const Segment&	segment2);
	float               GetDistanceSegmentToSegment		(const Segment&		segment1,	const Segment&	segment2,	VECTOR& h1, VECTOR& h2, float& t1, float& t2);

	/// @brief 線分と平面(無限に広がる面)の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToPlane		(const Segment&		segment,	const Plane&	plane);

	/// @brief 線分と三角形の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToTriangle	(const Segment&		segment,	const Triangle& triangle);

	/// @brief 線分と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceSegmentToSquare		(const Segment&		segment,	const Square&	square);

	/// @brief 平面とカプセルの最短距離を取得
	[[nodiscard]] float GetDistancePlaneToCapsule		(const Plane&		plane,		const Capsule&	capsule);

	/// @brief 三角形と三角形の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToTriangle	(const Triangle&	triangle1,	const Triangle& triangle2);

	/// @brief 三角形と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToSquare		(const Triangle&	triangle,	const Square&	square);

	/// @brief 三角形と球の最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToSphere     (const Triangle&    triangle,   const Sphere&   sphere);

	/// @brief 三角形とカプセルの最短距離を取得
	[[nodiscard]] float GetDistanceTriangleToCapsule	(const Triangle&	triangle,	const Capsule&	capsule);

	/// @brief 四角形と四角形の最短距離を取得
	[[nodiscard]] float GetDistanceSquareToSquare		(const Square&		square1,	const Square&	square2);

	/// @brief 四角形とカプセルの最短距離
	[[nodiscard]] float GetDistanceSquareToCapsule		(const Square&		square,		const Capsule&	capsule);
	#pragma endregion
}
