#pragma once
#include "../Calculation/math.hpp"
#include "../Data/model_frame_angle_limit_data.hpp"

namespace ik_solver
{
	enum class AidAxisKind
	{
		kNone = -1,

		kUp,
		kRight,
	};

	enum class RotDirKind
	{
		kLeft,
		kRight,
	};

	/// @brief 対象のforwardをY軸とした回転後のXYZ軸を取得する
	/// @brief MEMO : Mixamoモデルの軸に合わせた軸構成
	/// @param origin_axis 回転前のXYZ軸
	/// @param target_forward 回転後のY軸
	/// @return 回転後のXYZ軸
	[[nodiscard]] Axis GetRotatedMixamoAxis(const Axis& origin_axis, const VECTOR& target_forward, 
		const std::optional<VECTOR>& aid_axis = std::nullopt, const AidAxisKind axis_kind = AidAxisKind::kNone);

	/// @brief 1つのフレームに対してIK処理を適用する
	void OneFrameIK(const int model_handle, const VECTOR& world_destination, const int frame_index, 
		const std::optional<VECTOR>& aid_axis = std::nullopt, const AidAxisKind axis_kind = AidAxisKind::kNone);

	/// @brief 2つのフレームに対してIK処理を適用する
	/// @param model_handle モデルハンドル
	/// @param world_destination 目的地点
	/// @param end_frame_index 終点フレームのインデックス
	void TwoFrameIK(const int model_handle, const VECTOR& world_destination, const int end_frame_index, 
		const ModelFrameAngleLimitData& begin_angle_limit, const ModelFrameAngleLimitData& middle_angle_limit,
		const RotDirKind rot_dir_kind, const std::optional<VECTOR>& aid_axis = std::nullopt, const AidAxisKind axis_kind = AidAxisKind::kNone);

	/// @brief IK処理を適用する
	/// @param model_handle IK処理が適用されるモデルのハンドル
	/// @param world_destination 目標地点
	/// @param begin_frame_index 起点フレームのインデックス
	/// @param end_frame_index 終点フレームのインデックス
	void FrameIK(const int model_handle, const VECTOR& world_destination, const TCHAR* begin_frame_name, const TCHAR* end_frame_name);
}
