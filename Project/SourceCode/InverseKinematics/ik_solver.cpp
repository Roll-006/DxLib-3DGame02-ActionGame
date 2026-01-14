#include "ik_solver.hpp"

const Axis ik_solver::ConvertMixamoAxisToAxis(const Axis& mixamo_axis)
{
	return { -mixamo_axis.x_axis, mixamo_axis.z_axis, mixamo_axis.y_axis };
}

const Axis ik_solver::GetForwardSyncedMixamoAxis(const Axis& origin_mixamo_axis, const VECTOR& forward, const std::optional<AxisData>& aid_axis)
{
	const auto synced_axis = math::GetForwardSyncedAxis(ConvertMixamoAxisToAxis(origin_mixamo_axis), forward, aid_axis);
	return ConvertMixamoAxisToAxis(synced_axis);
}

void ik_solver::OneBoneIK(
	const int						model_handle,
	const VECTOR&					world_destination,
	const int						frame_index,
	const std::optional<AxisData>&	aid_axis)
{
	auto	   child_local_m			= MV1GetFrameLocalMatrix(model_handle, frame_index);
	const auto child_local_pos			= matrix::GetPos(child_local_m);
	auto	   child_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
	const auto child_world_pos			= matrix::GetPos(child_world_m);

	// 変換後のXYZ軸を取得	
	const auto current_axis				= math::ConvertRotMatrixToAxis(child_world_m);
	const auto forward					= v3d::GetNormalizedV(world_destination - child_world_pos);
	const auto target_axis				= GetForwardSyncedMixamoAxis(current_axis, forward, aid_axis);

	// 子のローカル回転行列を取得
	const auto parent_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1GetFrameParent(model_handle, frame_index));
	const auto parent_world_rot_m		= matrix::GetRotMatrix(parent_world_m);
	const auto parent_world_rot_inv_m	= MInverse(parent_world_rot_m);
	auto	   target_world_rot_m		= math::ConvertAxisToRotMatrix(target_axis);
	child_local_m = target_world_rot_m * parent_world_rot_inv_m;

	// 座標を戻し回転結果をフレームに適用
	matrix::SetPos(child_local_m, child_local_pos);
	MV1SetFrameUserLocalMatrix(model_handle, frame_index, child_local_m);
}

void ik_solver::TwoBoneIK(
	const int						model_handle,
	const VECTOR&					world_destination,
	const int						end_frame_index,
	ModelFrameAngleLimitData&		begin_angle_limit,
	ModelFrameAngleLimitData&		middle_angle_limit,
	const RotDirKind				rot_dir_kind,
	const bool						is_rotate_x_axis,
	const std::optional<AxisData>&	aid_axis)
{
	const auto middle_frame_index	= MV1GetFrameParent(model_handle, end_frame_index);
	const auto begin_frame_index	= MV1GetFrameParent(model_handle, middle_frame_index);

	// 起点フレームを目的位置に向ける
	OneBoneIK(model_handle, world_destination, begin_frame_index, aid_axis);

	// 中間フレームの回転を消す
	auto	   middle_local_m = MV1GetFrameLocalMatrix(model_handle, middle_frame_index);
	matrix::SetRot(middle_local_m, MGetIdent());
	MV1SetFrameUserLocalMatrix(model_handle, middle_frame_index, middle_local_m);

	// 各フレームの情報を取得
	auto	   begin_frame		= frame_info::GetFrameInfo(model_handle, begin_frame_index);
	auto	   middle_frame		= frame_info::GetFrameInfo(model_handle, middle_frame_index);
	auto	   end_frame		= frame_info::GetFrameInfo(model_handle, end_frame_index);
	const auto end_world_axis	= math::ConvertRotMatrixToAxis(end_frame.world_m);

	// 中間フレームの回転軸を法線とする三角形を形成
	const auto triangle_edge = TriangleEdgeData(
		VSize(middle_frame.world_pos - begin_frame .world_pos),
		VSize(end_frame   .world_pos - middle_frame.world_pos),
		VSize(begin_frame .world_pos - world_destination));

	// フレームを曲げる必要がない場合は関数を抜ける
	if (triangle_edge.length1 + triangle_edge.length2 < triangle_edge.length3
		|| std::abs(triangle_edge.length1 - triangle_edge.length2) > triangle_edge.length3) {
		return;
	}

	// 回転行列を構築
	CreateTwoBoneIKRotMatrix(begin_frame, middle_frame, begin_angle_limit, middle_angle_limit, triangle_edge, rot_dir_kind, is_rotate_x_axis);

	// 回転を適用
	matrix::SetRot(begin_frame.local_m, begin_frame.local_rot_m);
	MV1SetFrameUserLocalMatrix(model_handle, begin_frame_index, begin_frame.local_m);
	matrix::SetRot(middle_local_m, middle_frame.local_rot_m);
	MV1SetFrameUserLocalMatrix(model_handle, middle_frame_index, middle_local_m);
}

void ik_solver::CreateTwoBoneIKRotMatrix(
	FrameData&					begin_bone,
	FrameData&					middle_bone,
	ModelFrameAngleLimitData&	begin_angle_limit,
	ModelFrameAngleLimitData&	middle_angle_limit,
	const TriangleEdgeData&		triangle_edge,
	const RotDirKind			rot_dir_kind,
	const bool					is_rotate_x_axis)
{
	// 回転を構築するためのcos, sinを取得
	auto	   cos_b				=  ((std::powf(triangle_edge.length3, 2) + std::powf(triangle_edge.length1, 2) - std::powf(triangle_edge.length2, 2)) / (2 * triangle_edge.length3 * triangle_edge.length1));
	auto	   cos_c				= -((std::powf(triangle_edge.length1, 2) + std::powf(triangle_edge.length2, 2) - std::powf(triangle_edge.length3, 2)) / (2 * triangle_edge.length1 * triangle_edge.length2));
	const auto origin_angle_b		= std::acos(std::clamp(cos_b, -1.0f, 1.0f));
	const auto origin_angle_c		= std::acos(std::clamp(cos_c, -1.0f, 1.0f));
	const auto limited_angle_b		= std::clamp(origin_angle_b, begin_angle_limit .min_angle * math::kDegToRad, begin_angle_limit .max_angle * math::kDegToRad);		// 角度制限
	const auto limited_angle_c		= std::clamp(origin_angle_c, middle_angle_limit.min_angle * math::kDegToRad, middle_angle_limit.max_angle * math::kDegToRad);		// 角度制限
	cos_b							= std::cos(limited_angle_b);																	// 角度制限付きで再計算
	cos_c							= std::cos(limited_angle_c);																	// 角度制限付きで再計算
	const auto sin_b				= rot_dir_kind == RotDirKind::kLeft  ? -std::sin(limited_angle_b) : std::sin(limited_angle_b);
	const auto sin_c				= rot_dir_kind == RotDirKind::kRight ? -std::sin(limited_angle_c) : std::sin(limited_angle_c);	// sinC = sin(π - C)

	// 角度制限が発生したかを格納
	begin_angle_limit.is_limited	= std::abs(limited_angle_b - origin_angle_b) > math::kEpsilonLow;
	middle_angle_limit.is_limited	= std::abs(limited_angle_c - origin_angle_c) > math::kEpsilonLow;

	// 回転行列を構築
	const auto begin_rot			= is_rotate_x_axis ? matrix::CreateXMatrix(cos_b, sin_b) : matrix::CreateZMatrix(cos_b, sin_b);
	const auto middle_rot			= is_rotate_x_axis ? matrix::CreateXMatrix(cos_c, sin_c) : matrix::CreateZMatrix(cos_c, sin_c);
	begin_bone .local_rot_m			= begin_rot  * matrix::GetRotMatrix(begin_bone .local_m);
	middle_bone.local_rot_m			= middle_rot * matrix::GetRotMatrix(middle_bone.local_m);
}
