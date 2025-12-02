#pragma once
#include "../Calculation/math.hpp"
#include "../Data/model_frame_angle_limit_data.hpp"
#include "../Data/aid_mixamo_axis_data.hpp"
#include "../Data/triangle_edge_data.hpp"
#include "frame_info.hpp"

namespace ik_solver
{
	enum class RotDirKind
	{
		kLeft,
		kRight,
	};

	/// @brief forwardを指定した回転後のXYZ軸を取得する
	/// @brief MEMO : Mixamoモデルの軸に合わせた軸構成
	/// @param origin_axis 回転前のXYZ軸
	/// @param target_forward 回転後のY軸
	/// @param aid_axis 補助軸(初期値 : std::nullopt)
	/// @return 回転後のXYZ軸
	[[nodiscard]] Axis GetRotatedMixamoAxis(
		const Axis&								origin_axis, 
		const VECTOR&							target_forward, 
		const std::optional<AidMixamoAxisData>&	aid_axis = std::nullopt);

	/// @brief 1つのフレーム(ボーン)に対してIK処理を適用する
	/// @param model_handle モデルハンドル
	/// @param world_destination 目的地点(ワールド座標)
	/// @param frame_index フレームのインデックス
	/// @param aid_axis 補助軸(初期値 : std::nullopt)
	void OneBoneIK(
		const int								model_handle, 
		const VECTOR&							world_destination, 
		const int								frame_index, 
		const std::optional<AidMixamoAxisData>&	aid_axis = std::nullopt);

	/// @brief 2つのフレーム(ボーン)に対してIK処理を適用する
	/// @param model_handle モデルハンドル
	/// @param world_destination 目的地点(ワールド座標)
	/// @param end_frame_index 終点フレームのインデックス
	/// @param begin_angle_limit 始点フレームの角度制限
	/// @param middle_angle_limit 中間フレームの角度制限
	/// @param rot_dir_kind 回転方向
	/// @param aid_axis 補助軸(初期値 : std::nullopt)
	void TwoBoneIK(
		const int								model_handle, 
		const VECTOR&							world_destination, 
		const int								end_frame_index, 
		ModelFrameAngleLimitData&				begin_angle_limit, 
		ModelFrameAngleLimitData&				middle_angle_limit,
		const RotDirKind						rot_dir_kind, 
		const std::optional<AidMixamoAxisData>&	aid_axis = std::nullopt);

	/// @brief 2ボーンIKに必要な回転行列を生成する
	/// @param begin_rot_m 始点フレームの回転行列
	/// @param middle_rot_m 中間フレームの回転行列
	/// @param begin_angle_limit 始点フレームの角度制限
	/// @param middle_angle_limit 中間フレームの角度制限
	/// @param triangle_edge 三角形の辺データ
	/// @param rot_dir_kind 回転方向
	void CreateTwoBoneIKRotMatrix(
		FrameData&							begin_frame,
		FrameData&							middle_frame,
		ModelFrameAngleLimitData&			begin_angle_limit,
		ModelFrameAngleLimitData&			middle_angle_limit,
		const TriangleEdgeData				triangle_edge,
		const RotDirKind					rot_dir_kind);
}
