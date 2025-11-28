#include "ik_solver.hpp"

Axis ik_solver::GetRotatedMixamoAxis(
	const Axis&							origin_axis,
	const VECTOR&						target_forward,
	const std::optional<AidAxisData>&	aid_axis)
{
	// MixamoのモデルはY軸がボーンの進行方向であるため、それを前提に作成する
	// ヨー軸回転 : Z, ピッチ軸回転 : X, ロール軸回転 : Y

	Axis mixamo_axis;
	mixamo_axis.y_axis = target_forward;

	// 補助軸なし
	if(!aid_axis || aid_axis->kind == AxisKind::kForward)
	{
		mixamo_axis.x_axis = math::GetNormalVector(origin_axis.z_axis, mixamo_axis.y_axis);		// 一時的なピッチ軸回転を計算
		mixamo_axis.z_axis = math::GetNormalVector(mixamo_axis.y_axis, mixamo_axis.x_axis);
		mixamo_axis.x_axis = math::GetNormalVector(mixamo_axis.z_axis, mixamo_axis.y_axis);		// ピッチ軸回転を再計算
	}
	// 補助軸 : Left
	else if (aid_axis->kind == AxisKind::kLeft)
	{
		mixamo_axis.x_axis = -aid_axis->axis;													// 一時的なピッチ軸回転を計算
		mixamo_axis.z_axis =  math::GetNormalVector(mixamo_axis.y_axis, mixamo_axis.x_axis);
		mixamo_axis.x_axis = -math::GetNormalVector(mixamo_axis.z_axis, mixamo_axis.y_axis);	// ピッチ軸回転を再計算
	}
	// 補助軸 : Up
	else if (aid_axis->kind == AxisKind::kUp)
	{
		mixamo_axis.z_axis = aid_axis->axis;													// 一時的なヨー軸回転を計算
		mixamo_axis.x_axis = math::GetNormalVector(mixamo_axis.z_axis, mixamo_axis.y_axis);
		mixamo_axis.z_axis = math::GetNormalVector(mixamo_axis.x_axis, mixamo_axis.y_axis);		// ヨー軸回転を再計算
	}
	
	return mixamo_axis;
}

void ik_solver::OneBoneIK(
	const int							model_handle,
	const VECTOR&						world_destination,
	const int							frame_index,
	const std::optional<AidAxisData>&	aid_axis)
{
	auto	   child_local_m			= MV1GetFrameLocalMatrix(model_handle, frame_index);
	const auto child_local_pos			= MGetTranslateElem(child_local_m);
	auto	   child_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
	const auto child_world_pos			= MGetTranslateElem(child_world_m);

	// 変換後のXYZ軸を取得	
	const auto current_axis				= math::ConvertRotMatrixToAxis(child_world_m);
	const auto forward					= v3d::GetNormalizedV(world_destination - child_world_pos);
	const auto target_axis				= GetRotatedMixamoAxis(current_axis, forward, aid_axis);

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
	const int							model_handle,
	const VECTOR&						world_destination,
	const int							end_frame_index,
	ModelFrameAngleLimitData&			begin_angle_limit,
	ModelFrameAngleLimitData&			middle_angle_limit,
	const RotDirKind					rot_dir_kind,
	const std::optional<AidAxisData>&	aid_axis)
{
	const auto end_frame_name		= MV1GetFrameName	(model_handle, end_frame_index);
	const auto middle_frame_index	= MV1GetFrameParent	(model_handle, end_frame_index);
	const auto middle_frame_name	= MV1GetFrameName	(model_handle, middle_frame_index);
	const auto begin_frame_index	= MV1GetFrameParent	(model_handle, middle_frame_index);
	const auto begin_frame_name		= MV1GetFrameName	(model_handle, begin_frame_index);

	// 起点フレームを目的位置に向ける
	OneBoneIK(model_handle, world_destination, begin_frame_index, aid_axis);

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
	auto	   cos_b				=  ((std::powf(length_c, 2) + std::powf(length_a, 2) - std::powf(length_b, 2)) / (2 * length_c * length_a));
	auto	   cos_c				= -((std::powf(length_a, 2) + std::powf(length_b, 2) - std::powf(length_c, 2)) / (2 * length_a * length_b));
	const auto origin_angle_b		= std::acos(std::clamp(cos_b, -1.0f, 1.0f));
	const auto origin_angle_c		= std::acos(std::clamp(cos_c, -1.0f, 1.0f));
	const auto limited_angle_b		= std::clamp(origin_angle_b, begin_angle_limit.min_angle,  begin_angle_limit.max_angle);		// 角度制限
	const auto limited_angle_c		= std::clamp(origin_angle_c, middle_angle_limit.min_angle, middle_angle_limit.max_angle);		// 角度制限
	cos_b							= std::cos(limited_angle_b);																	// 角度制限付きで再計算
	cos_c							= std::cos(limited_angle_c);																	// 角度制限付きで再計算
	const auto sin_b				= rot_dir_kind == RotDirKind::kLeft  ? -std::sin(limited_angle_b) : std::sin(limited_angle_b);
	const auto sin_c				= rot_dir_kind == RotDirKind::kRight ? -std::sin(limited_angle_c) : std::sin(limited_angle_c);	// sinC = sin(π - C)

	// 角度制限が発生したかを格納
	begin_angle_limit.is_limited  = std::fabs(limited_angle_b - origin_angle_b) > math::kEpsilonLow;
	middle_angle_limit.is_limited = std::fabs(limited_angle_c - origin_angle_c) > math::kEpsilonLow;

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
