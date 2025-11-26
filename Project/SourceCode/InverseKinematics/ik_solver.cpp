#include "ik_solver.hpp"

Axis ik_solver::GetRotatedMixamoAxis(const Axis& origin_axis, const VECTOR& target_forward, const std::optional<VECTOR>& aid_axis, const AidAxisKind axis_kind)
{
	// MixamoのモデルはY軸がボーンの進行方向であるため、それを前提に作成する
	// ヨー軸回転 : Z, ピッチ軸回転 : X, ロール軸回転 : Y

	Axis mixamo_axis;
	mixamo_axis.y_axis = target_forward;

	if (axis_kind == AidAxisKind::kRight && aid_axis)
	{
		mixamo_axis.x_axis = -*aid_axis;														// 一時的なピッチ軸回転を計算
		mixamo_axis.z_axis = math::GetNormalVector(mixamo_axis.y_axis, mixamo_axis.x_axis);
		mixamo_axis.x_axis = math::GetNormalVector(mixamo_axis.z_axis, mixamo_axis.y_axis);		// ピッチ軸回転を再計算
		mixamo_axis.x_axis *= -1;
	}
	else if (axis_kind == AidAxisKind::kUp && aid_axis)
	{
		mixamo_axis.z_axis = *aid_axis;
		mixamo_axis.x_axis = math::GetNormalVector(mixamo_axis.z_axis, mixamo_axis.y_axis);
		mixamo_axis.z_axis = math::GetNormalVector(mixamo_axis.x_axis, mixamo_axis.y_axis);
	}
	else
	{
		mixamo_axis.x_axis = math::GetNormalVector(origin_axis.z_axis, mixamo_axis.y_axis);		// 一時的なピッチ軸回転を計算
		mixamo_axis.z_axis = math::GetNormalVector(mixamo_axis.y_axis, mixamo_axis.x_axis);
		mixamo_axis.x_axis = math::GetNormalVector(mixamo_axis.z_axis, mixamo_axis.y_axis);		// ピッチ軸回転を再計算
	}
	
	return mixamo_axis;
}

void ik_solver::OneFrameIK(const int model_handle, const VECTOR& world_destination, const int frame_index, const std::optional<VECTOR>& aid_axis, const AidAxisKind axis_kind)
{
	auto	   child_local_m			= MV1GetFrameLocalMatrix(model_handle, frame_index);
	const auto child_local_pos			= MGetTranslateElem(child_local_m);
	auto	   child_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
	const auto child_world_pos			= MGetTranslateElem(child_world_m);

	// 変換後のXYZ軸を取得	
	const auto current_axis				= math::ConvertRotMatrixToAxis(child_world_m);
	const auto forward					= v3d::GetNormalizedV(world_destination - child_world_pos);
	const auto target_axis				= GetRotatedMixamoAxis(current_axis, forward, aid_axis, axis_kind);

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

void ik_solver::TwoFrameIK(const int model_handle, const VECTOR& world_destination, const int end_frame_index, 
	const ModelFrameAngleLimitData& begin_angle_limit, const ModelFrameAngleLimitData& middle_angle_limit,
	const RotDirKind rot_dir_kind, const std::optional<VECTOR>& aid_axis, const AidAxisKind axis_kind)
{
	const auto end_frame_name		= MV1GetFrameName	(model_handle, end_frame_index);
	const auto middle_frame_index	= MV1GetFrameParent	(model_handle, end_frame_index);
	const auto middle_frame_name	= MV1GetFrameName	(model_handle, middle_frame_index);
	const auto begin_frame_index	= MV1GetFrameParent	(model_handle, middle_frame_index);
	const auto begin_frame_name		= MV1GetFrameName	(model_handle, begin_frame_index);

	// 起点フレームを目的位置に向ける
	OneFrameIK(model_handle, world_destination, begin_frame_index, aid_axis, axis_kind);

	// 中間フレームの回転を消す
	auto	   middle_local_m		= MV1GetFrameLocalMatrix(model_handle, middle_frame_index);
	matrix::SetRot(middle_local_m, MGetIdent());
	MV1SetFrameUserLocalMatrix(model_handle, middle_frame_index, middle_local_m);

	// 各フレームの情報を取得
	auto	   begin_local_m		= MV1GetFrameLocalMatrix(model_handle, begin_frame_index);
	auto	   begin_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, begin_frame_index);
	const auto begin_world_pos		= MGetTranslateElem(begin_world_m);
			   middle_local_m		= MV1GetFrameLocalMatrix(model_handle, middle_frame_index);
	auto	   middle_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, middle_frame_index);
	const auto middle_world_pos		= MGetTranslateElem(middle_world_m);
	auto	   end_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, end_frame_index);
	const auto end_world_pos		= MGetTranslateElem(end_world_m);
	const auto end_world_axis		= math::ConvertRotMatrixToAxis(end_world_m);

	// 中間フレームの回転軸を法線とする三角形を形成
	const auto length_a				= VSize(middle_world_pos - begin_world_pos);		// 起点フレームから中間フレームまでの長さ
	const auto length_b				= VSize(end_world_pos	 - middle_world_pos);		// 中間フレームから終点フレームまでの長さ
	const auto length_c				= VSize(begin_world_pos  - world_destination);		// 目標地点から起点フレームまでの長さ
	
	// フレームを曲げる必要がない場合は関数を抜ける
	if (length_a + length_b < length_c || std::abs(length_a - length_b) > length_c) { return; }
	
	// 回転を構築するためのcos, sinを取得
	auto	   cos_b				=  ((std::powf(length_c, 2) + std::powf(length_a, 2) - std::powf(length_b, 2)) / (2 * length_c * length_a));			// cos(π − C) = −cosCであるため符号を反転
	auto	   cos_c				= -((std::powf(length_a, 2) + std::powf(length_b, 2) - std::powf(length_c, 2)) / (2 * length_a * length_b));
	const auto angle_b				= std::clamp(std::acos(std::clamp(cos_b, -1.0f, 1.0f)), begin_angle_limit.min_angle,  begin_angle_limit.max_angle);		// 角度制限
	const auto angle_c				= std::clamp(std::acos(std::clamp(cos_c, -1.0f, 1.0f)), middle_angle_limit.min_angle, middle_angle_limit.max_angle);	// 角度制限
	cos_b							= std::cos(angle_b);		// 角度制限付きで再計算
	cos_c							= std::cos(angle_c);		// 角度制限付きで再計算
	const auto sin_b				= rot_dir_kind == RotDirKind::kLeft  ? -std::sin(angle_b) : std::sin(angle_b);
	const auto sin_c				= rot_dir_kind == RotDirKind::kRight ? -std::sin(angle_c) : std::sin(angle_c);		// 実際にはsin(π - C)だが値は変わらないため操作は行わない

	// 回転行列を構築
	const auto begin_rot_m			= matrix::CreateXMatrix(cos_b, sin_b);
	const auto middle_rot_m			= matrix::CreateXMatrix(cos_c, sin_c);
	const auto begin_result_rot_m	= begin_rot_m  * matrix::GetRotMatrix(begin_local_m);
	const auto middle_result_rot_m	= middle_rot_m * matrix::GetRotMatrix(middle_local_m);

	// 回転を適用
	matrix::SetRot(begin_local_m, begin_result_rot_m);
	MV1SetFrameUserLocalMatrix(model_handle, begin_frame_index,  begin_local_m);
	matrix::SetRot(middle_local_m, middle_result_rot_m);
	MV1SetFrameUserLocalMatrix(model_handle, middle_frame_index, middle_local_m);
}

void ik_solver::FrameIK(const int model_handle, const VECTOR& world_destination, const TCHAR* begin_frame_name, const TCHAR* end_frame_name)
{
	const auto begin_frame_index	= MV1SearchFrame(model_handle, begin_frame_name);
	const auto end_frame_index		= MV1SearchFrame(model_handle, end_frame_name);

	// 起点および終点フレームの情報を取得
	auto	   begin_local_m		= MV1GetFrameLocalMatrix(model_handle, begin_frame_index);
	auto	   begin_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, begin_frame_index);
	const auto begin_world_pos		= MGetTranslateElem(begin_world_m);
	auto	   end_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, end_frame_index);
	const auto end_world_pos		= MGetTranslateElem(end_world_m);

	const auto current_axis			= math::ConvertRotMatrixToAxis(begin_world_m);

	// 終点フレームに回転させた軸から補助行列を取得
	const auto aid_forward			= v3d::GetNormalizedV(end_world_pos - begin_world_pos);
	const auto aid_axis				= GetRotatedMixamoAxis(current_axis, aid_forward);
	const auto aid_rot_m			= math::ConvertAxisToRotMatrix(aid_axis);

	// 回転すべき行列を取得
	const auto target_forward		= v3d::GetNormalizedV(world_destination - begin_world_pos);
	const auto target_axis			= GetRotatedMixamoAxis(current_axis, target_forward);
	const auto target_rot_m			= math::ConvertAxisToRotMatrix(target_axis);

	// aid_rot_mがtarget_rot_mに回転するための回転行列を取得
	const auto delta_rot_m			= target_rot_m * MInverse(aid_rot_m);

	// 新たな回転行列を取得
	auto new_local_rot_m			= delta_rot_m * matrix::GetRotMatrix(begin_local_m);

	// 回転を上書きしてフレームに適用
	matrix::SetRot(begin_local_m, new_local_rot_m);
	MV1SetFrameUserLocalMatrix(model_handle, begin_frame_index, begin_local_m);
}
