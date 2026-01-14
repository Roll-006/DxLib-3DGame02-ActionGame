#include "../JSON/json_loader.hpp"
#include "ik_solver.hpp"
#include "frame_info.hpp"
#include "../Interface/i_humanoid.hpp"
#include "../Base/physical_obj_base.hpp"
#include "../Animator/animator.hpp"
#include "../Part/collider.hpp"
#include "../Data/humanoid_arm_ray_data.hpp"
#include "../Data/model_frame_angle_limit_data.hpp"
#include "humanoid_arm_ik_solver.hpp"

std::unordered_map<std::string, ModelFrameAngleLimitData> HumanoidArmIKSolver::angle_limits = {};

HumanoidArmIKSolver::HumanoidArmIKSolver(
	IHumanoid& humanoid,
	const std::shared_ptr<Animator>& animator, 
	const std::shared_ptr<Modeler>& modeler, 
	std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, 
	HumanoidArmRayData& ray_data) :
	m_humanoid				(humanoid),
	m_animator				(animator),
	m_modeler				(modeler),
	m_colliders				(colliders),
	m_ray_data				(ray_data),
	m_origin_arm_matrices	(),
	m_result_arm_matrices	(),
	m_left_arm_blend_timer	(0.0f),
	m_right_arm_blend_timer	(0.0f),
	m_left_ik_kind			{ {TimeKind::kCurrent, IKKind::kNone}, {TimeKind::kPrev, IKKind::kNone} },
	m_right_ik_kind			{ {TimeKind::kCurrent, IKKind::kNone}, {TimeKind::kPrev, IKKind::kNone} }
{
	if (angle_limits.empty())
	{
		nlohmann::json j_data;
		if (json_loader::Load("Data/JSON/model_arm_angle_limits.json", j_data))
		{
			for (const auto& [frame_name, angle_limit] : j_data.at("humanoid").items())
			{
				angle_limits[frame_name] = angle_limit.get<ModelFrameAngleLimitData>();
			}
		}
	}
}

HumanoidArmIKSolver::~HumanoidArmIKSolver()
{

}

void HumanoidArmIKSolver::Init()
{
	ChangeLeftArmOriginMatrix();
	ChangeRightArmOriginMatrix();
}

void HumanoidArmIKSolver::Update()
{
	m_left_ik_kind .at(TimeKind::kPrev) = m_left_ik_kind .at(TimeKind::kCurrent);
	m_right_ik_kind.at(TimeKind::kPrev) = m_right_ik_kind.at(TimeKind::kCurrent);

	m_left_ik_kind .at(TimeKind::kCurrent) = IKKind::kNone;
	m_right_ik_kind.at(TimeKind::kCurrent) = IKKind::kNone;
}


#pragma region コライダー
void HumanoidArmIKSolver::CreateLeftHandRay(PhysicalObjBase* physical_obj) const
{
	const auto model_handle		= m_modeler->GetModelHandle();

	auto	   left_hand_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), m_humanoid.GetHumanoidFrame()->GetLeftHandIndex(model_handle));
	const auto left_hand_pos	= matrix::GetPos(left_hand_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftHandRay, std::make_shared<Segment>(left_hand_pos + VGet(0.0f, m_ray_data.left_hand_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.hand_ray_length), physical_obj));
}

void HumanoidArmIKSolver::CreateRightHandRay(PhysicalObjBase* physical_obj) const
{
	const auto model_handle		= m_modeler->GetModelHandle();

	auto	   right_hand_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), m_humanoid.GetHumanoidFrame()->GetRightHandIndex(model_handle));
	const auto right_hand_pos	= matrix::GetPos(right_hand_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightHandRay, std::make_shared<Segment>(right_hand_pos + VGet(0.0f, m_ray_data.right_hand_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.hand_ray_length), physical_obj));
}

void HumanoidArmIKSolver::DeleteLeftHandRay(PhysicalObjBase* physical_obj) const
{
	physical_obj->RemoveCollider(ColliderKind::kLeftHandRay);
}

void HumanoidArmIKSolver::DeleteRightHandRay(PhysicalObjBase* physical_obj) const
{
	physical_obj->RemoveCollider(ColliderKind::kRightHandRay);
}

void HumanoidArmIKSolver::CalcLeftHandRayPos() const
{
	const auto model_handle			= m_modeler->GetModelHandle();

	const auto left_hand_segment	= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kLeftHandRay)->GetShape());
	auto	   left_hand_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), m_humanoid.GetHumanoidFrame()->GetLeftHandIndex(model_handle));
	const auto left_hand_pos		= matrix::GetPos(left_hand_m);
	left_hand_segment->SetBeginPos(left_hand_pos + VGet(0.0f, m_ray_data.left_hand_ray_offset, 0.0f), false);
}

void HumanoidArmIKSolver::CalcRightHandRayPos() const
{
	const auto model_handle			= m_modeler->GetModelHandle();

	const auto right_hand_segment	= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kRightHandRay)->GetShape());
	auto	   right_hand_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), m_humanoid.GetHumanoidFrame()->GetRightHandIndex(model_handle));
	const auto right_hand_pos		= matrix::GetPos(right_hand_m);
	right_hand_segment->SetBeginPos(right_hand_pos + VGet(0.0f, m_ray_data.right_hand_ray_offset, 0.0f), false);
}
#pragma endregion


void HumanoidArmIKSolver::ApplyLeftKneelCrouchIK ()
{
	// 左手手IK処理
	const auto left_arm_result = ApplyLeftArmIKOnGround();

	// 左手手が膝に到達しなかった場合、目標地点を太ももに変更して再計算
	switch (left_arm_result)
	{
	case ResultKind::kFailure:
		ApplyLeftHandIKOnThigh();
		break;

	default:
		break;
	}

	// ブレンドの起点行列を設定する
	if (m_left_ik_kind.at(TimeKind::kCurrent) != m_left_ik_kind.at(TimeKind::kPrev))
	{
		ChangeLeftArmOriginMatrix(true);
	}

	// 右手IK処理
	const auto right_arm_result = ApplyRightHandIKOnKnees();

	switch (right_arm_result)
	{
		// 右手が膝に到達しなかった場合、目標地点を太ももに変更して再計算
	case ResultKind::kFailure:
		ApplyRightHandIKOnThigh();
		break;

	default:
		break;
	}

	// ブレンドの起点行列を設定する
	if (m_right_ik_kind.at(TimeKind::kCurrent) != m_right_ik_kind.at(TimeKind::kPrev))
	{
		ChangeRightArmOriginMatrix(true);
	}

	m_ray_data.left_hand_cast_pos		= std::nullopt;
	m_ray_data.right_hand_cast_pos		= std::nullopt;
	m_ray_data.left_hand_hit_triangle	= std::nullopt;
	m_ray_data.right_hand_hit_triangle	= std::nullopt;
}

void HumanoidArmIKSolver::ApplyRightKneelCrouchIK()
{
	// 左手IK処理
	const auto left_arm_result = ApplyLeftHandIKOnKnees();

	switch (left_arm_result)
	{
	// 左手が膝に到達しなかった場合、目標地点を太ももに変更して再計算
	case ResultKind::kFailure:
		ApplyLeftHandIKOnThigh();
		break;

	default:
		break;
	}

	// ブレンドの起点行列を設定する
	if (m_left_ik_kind.at(TimeKind::kCurrent) != m_left_ik_kind.at(TimeKind::kPrev))
	{
		ChangeLeftArmOriginMatrix(true);
	}

	// 右手IK処理
	const auto right_arm_result = ApplyRightArmIKOnGround();

	// 右手が膝に到達しなかった場合、目標地点を太ももに変更して再計算
	switch (right_arm_result)
	{
	case ResultKind::kFailure:
		ApplyRightHandIKOnThigh();
		break;

	default:
		break;
	}

	// ブレンドの起点行列を設定する
	if (m_right_ik_kind.at(TimeKind::kCurrent) != m_right_ik_kind.at(TimeKind::kPrev))
	{
		ChangeRightArmOriginMatrix(true);
	}

	m_ray_data.left_hand_cast_pos		= std::nullopt;
	m_ray_data.right_hand_cast_pos		= std::nullopt;
	m_ray_data.left_hand_hit_triangle	= std::nullopt;
	m_ray_data.right_hand_hit_triangle	= std::nullopt;
}

void HumanoidArmIKSolver::BlendFrame()
{
	const auto model_handle		= m_modeler->GetModelHandle();
	const auto humanoid_frame	= m_humanoid.GetHumanoidFrame();

	// 補間係数tを取得
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
	math::Increase(m_left_arm_blend_timer,  delta_time, arm_blend_time,	false);
	math::Increase(m_right_arm_blend_timer, delta_time, arm_blend_time,	false);
	const auto left_arm_t	= math::GetUnitValue<float, float>(0.0f, arm_blend_time, m_left_arm_blend_timer);
	const auto right_arm_t	= math::GetUnitValue<float, float>(0.0f, arm_blend_time, m_right_arm_blend_timer);

	// ブレンド結果を取得
	m_result_arm_matrices.left_arm			= math::GetLerpMatrix(m_origin_arm_matrices.left_arm,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftArmIndex		(model_handle)),left_arm_t);
	m_result_arm_matrices.left_fore_arm		= math::GetLerpMatrix(m_origin_arm_matrices.left_fore_arm,	MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex	(model_handle)),left_arm_t);
	m_result_arm_matrices.left_hand			= math::GetLerpMatrix(m_origin_arm_matrices.left_hand,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex		(model_handle)),left_arm_t);

	m_result_arm_matrices.right_arm			= math::GetLerpMatrix(m_origin_arm_matrices.right_arm,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightArmIndex		(model_handle)),right_arm_t);
	m_result_arm_matrices.right_fore_arm	= math::GetLerpMatrix(m_origin_arm_matrices.right_fore_arm,	MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightForeArmIndex	(model_handle)),right_arm_t);
	m_result_arm_matrices.right_hand		= math::GetLerpMatrix(m_origin_arm_matrices.right_hand,		MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex		(model_handle)),right_arm_t);

	// ブレンド結果をモデルに設定
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftArmIndex		(model_handle), m_result_arm_matrices.left_arm);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex	(model_handle), m_result_arm_matrices.left_fore_arm);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex		(model_handle), m_result_arm_matrices.left_hand);
	
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightArmIndex		(model_handle), m_result_arm_matrices.right_arm);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightForeArmIndex	(model_handle), m_result_arm_matrices.right_fore_arm);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex		(model_handle), m_result_arm_matrices.right_hand);
}


#pragma region IK処理
const HumanoidArmIKSolver::ResultKind HumanoidArmIKSolver::ApplyLeftHandIKOnKnees	()
{
	m_left_ik_kind.at(TimeKind::kCurrent) = IKKind::kHandOnKnees;

	const auto model_handle				= m_modeler->GetModelHandle();
	const auto humanoid_frame			= m_humanoid.GetHumanoidFrame();
	auto	   left_arm_limit			= angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetLeftArmIndex		(model_handle)));
	auto	   left_fore_arm_limit		= angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetLeftForeArmIndex	(model_handle)));

	// 行列情報を取得
	auto	   left_leg_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetLeftLegIndex(model_handle));
	const auto left_leg_world_pos		= matrix::GetPos(left_leg_world_m);
	const auto left_leg_world_axis		= math::ConvertRotMatrixToAxis(left_leg_world_m);
	auto	   left_fore_arm_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex(model_handle));
	const auto left_fore_arm_world_axis	= math::ConvertRotMatrixToAxis(left_fore_arm_world_m);
	auto	   left_shoulder_frame		= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetLeftShoulderIndex(model_handle));
	const auto left_shoulder_world_axis = math::ConvertRotMatrixToAxis(left_shoulder_frame.world_rot_m);
	auto	   left_hand_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetLeftHandIndex(model_handle));
	const auto left_hand_world_pos		= matrix::GetPos(left_hand_world_m);

	// 左腕IK処理
	ik_solver::TwoBoneIK(
		model_handle, left_leg_world_pos - left_leg_world_axis.z_axis * 3.0f,
		humanoid_frame->GetLeftHandIndex(model_handle),
		left_arm_limit, left_fore_arm_limit,
		ik_solver::RotDirKind::kLeft, false, std::make_optional<AxisData>(left_shoulder_world_axis.y_axis, AxisKind::kUp));

	// 左手の回転を削除
	auto left_hand_local_m = MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex(model_handle));
	matrix::SetRot(left_hand_local_m, MGetIdent());
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex(model_handle), left_hand_local_m);

	// 腕が伸びきっていた場合、ため失敗とみなす
	auto left_fore_arm_local_m				= MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex(model_handle));
	const auto left_fore_arm_local_rot_m	= matrix::GetRotMatrix(left_fore_arm_local_m);
	if (left_fore_arm_local_rot_m == MGetIdent())
	{
		return ResultKind::kFailure;
	}

	// 到達したが、角度制限がかかった
	if (left_arm_limit.is_limited || left_fore_arm_limit.is_limited) { return ResultKind::kLimitedSuccess; }

	return ResultKind::kSuccess;
}

const HumanoidArmIKSolver::ResultKind HumanoidArmIKSolver::ApplyRightHandIKOnKnees()
{
	m_right_ik_kind.at(TimeKind::kCurrent) = IKKind::kHandOnKnees;

	const auto model_handle					= m_modeler->GetModelHandle();
	const auto humanoid_frame				= m_humanoid.GetHumanoidFrame();
	auto	   right_arm_limit				= angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetRightArmIndex		(model_handle)));
	auto	   right_fore_arm_limit			= angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetRightForeArmIndex	(model_handle)));

	// 行列情報を取得
	auto	   right_leg_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetRightLegIndex(model_handle));
	const auto right_leg_world_pos			= matrix::GetPos(right_leg_world_m);
	const auto right_leg_world_axis			= math::ConvertRotMatrixToAxis(right_leg_world_m);
	auto	   right_fore_arm_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetRightForeArmIndex(model_handle));
	const auto right_fore_arm_world_axis	= math::ConvertRotMatrixToAxis(right_fore_arm_world_m);
	auto	   right_shoulder_frame			= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetRightShoulderIndex(model_handle));
	const auto right_shoulder_world_axis	= math::ConvertRotMatrixToAxis(right_shoulder_frame.world_rot_m);
	auto	   right_hand_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetRightHandIndex(model_handle));
	const auto right_hand_world_pos			= matrix::GetPos(right_hand_world_m);

	// 左腕IK処理
	ik_solver::TwoBoneIK(
		model_handle, right_leg_world_pos - right_leg_world_axis.z_axis * 3.0f,
		humanoid_frame->GetRightHandIndex(model_handle),
		right_arm_limit, right_fore_arm_limit,
		ik_solver::RotDirKind::kRight, false, std::make_optional<AxisData>(right_shoulder_world_axis.y_axis, AxisKind::kUp));

	// 左手の回転を削除
	auto right_hand_local_m = MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex(model_handle));
	matrix::SetRot(right_hand_local_m, MGetIdent());
	MV1SetFrameUserLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex(model_handle), right_hand_local_m);

	// 腕が伸びきっていた場合、ため失敗とみなす
	auto right_fore_arm_local_m = MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightForeArmIndex(model_handle));
	const auto right_fore_arm_local_rot_m = matrix::GetRotMatrix(right_fore_arm_local_m);
	if (right_fore_arm_local_rot_m == MGetIdent())
	{
		return ResultKind::kFailure;
	}

	// 到達したが、角度制限がかかった
	if (right_arm_limit.is_limited || right_fore_arm_limit.is_limited) { return ResultKind::kLimitedSuccess; }

	return ResultKind::kSuccess;
}

const HumanoidArmIKSolver::ResultKind HumanoidArmIKSolver::ApplyLeftHandIKOnThigh	()
{
	m_left_ik_kind.at(TimeKind::kCurrent) = IKKind::kHandOnThigh;

	const auto model_handle				= m_modeler->GetModelHandle();
	const auto humanoid_frame			= m_humanoid.GetHumanoidFrame();

	// 行列情報を取得
	auto	   left_up_leg_frame		= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetLeftUpLegIndex(model_handle));
	const auto left_up_leg_world_axis	= math::ConvertRotMatrixToAxis(left_up_leg_frame.world_rot_m);
	auto	   left_shoulder_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetLeftShoulderIndex(model_handle));
	const auto left_shoulder_world_axis = math::ConvertRotMatrixToAxis(left_shoulder_world_m);
	auto	   left_leg_frame			= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetLeftLegIndex(model_handle));
	auto	   left_fore_arm_frame		= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetLeftForeArmIndex(model_handle));
	auto	   left_hand_frame			= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetLeftHandIndex(model_handle));

	auto thigh_pos = (left_up_leg_frame.world_pos + left_leg_frame.world_pos) * 0.5f;
	thigh_pos += -left_up_leg_world_axis.z_axis * 4.0f - left_up_leg_world_axis.x_axis * 3.0f;

	// 左腕IK処理
	ik_solver::TwoBoneIK(
		model_handle, thigh_pos,
		humanoid_frame->GetLeftHandIndex(model_handle),
		angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetLeftArmIndex		(model_handle))),
		angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetLeftForeArmIndex	(model_handle))),
		ik_solver::RotDirKind::kLeft, false, std::make_optional<AxisData>(-left_shoulder_world_axis.y_axis, AxisKind::kRight));

	// 左手IK処理
	ik_solver::OneBoneIK(model_handle, left_leg_frame.world_pos - left_up_leg_world_axis.z_axis * 2.0f,
		humanoid_frame->GetLeftHandIndex(model_handle),
		std::make_optional<AxisData>(-left_up_leg_world_axis.z_axis, AxisKind::kUp));

	// 腕が伸びきっていた場合、到達していないとみなす
	auto left_fore_arm_local_m				= MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex(model_handle));
	const auto left_fore_arm_local_rot_m	= matrix::GetRotMatrix(left_fore_arm_local_m);
	if (left_fore_arm_local_rot_m == MGetIdent())
	{
		return ResultKind::kFailure;
	}

	return ResultKind::kSuccess;
}

const HumanoidArmIKSolver::ResultKind HumanoidArmIKSolver::ApplyRightHandIKOnThigh()
{
	m_right_ik_kind.at(TimeKind::kCurrent)	= IKKind::kHandOnThigh;

	const auto model_handle					= m_modeler->GetModelHandle();
	const auto humanoid_frame				= m_humanoid.GetHumanoidFrame();

	// 行列情報を取得
	auto	   right_up_leg_frame			= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetRightUpLegIndex(model_handle));
	const auto right_up_leg_world_axis		= math::ConvertRotMatrixToAxis(right_up_leg_frame.world_rot_m);
	auto	   right_shoulder_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetRightShoulderIndex(model_handle));
	const auto right_shoulder_world_axis	= math::ConvertRotMatrixToAxis(right_shoulder_world_m);
	auto	   right_leg_frame				= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetRightLegIndex(model_handle));
	auto	   right_fore_arm_frame			= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetRightForeArmIndex(model_handle));
	auto	   right_hand_frame				= frame_info::GetFrameInfo(model_handle, humanoid_frame->GetRightHandIndex(model_handle));

	auto thigh_pos = (right_up_leg_frame.world_pos + right_leg_frame.world_pos) * 0.5f;
	thigh_pos += -right_up_leg_world_axis.z_axis * 4.0f + right_up_leg_world_axis.x_axis * 3.0f;

	// 左腕IK処理
	ik_solver::TwoBoneIK(
		model_handle, thigh_pos,
		humanoid_frame->GetRightHandIndex(model_handle),
		angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetRightArmIndex		(model_handle))),
		angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetRightForeArmIndex	(model_handle))),
		ik_solver::RotDirKind::kRight, false, std::make_optional<AxisData>(right_shoulder_world_axis.y_axis, AxisKind::kRight));

	// 左手IK処理
	ik_solver::OneBoneIK(model_handle, right_leg_frame.world_pos - right_up_leg_world_axis.z_axis * 2.0f,
		humanoid_frame->GetRightHandIndex(model_handle),
		std::make_optional<AxisData>(-right_up_leg_world_axis.z_axis, AxisKind::kUp));

	// 腕が伸びきっていた場合、到達していないとみなす
	auto right_fore_arm_local_m				= MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightForeArmIndex(model_handle));
	const auto right_fore_arm_local_rot_m	= matrix::GetRotMatrix(right_fore_arm_local_m);
	if (right_fore_arm_local_rot_m == MGetIdent())
	{
		return ResultKind::kFailure;
	}

	return ResultKind::kSuccess;
}

const HumanoidArmIKSolver::ResultKind HumanoidArmIKSolver::ApplyLeftArmIKOnGround()
{
	m_left_ik_kind.at(TimeKind::kCurrent) = IKKind::kHandOnGround;

	const auto model_handle				= m_modeler->GetModelHandle();
	const auto humanoid_frame			= m_humanoid.GetHumanoidFrame();

	// 行列情報を取得
	auto	   spine2_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetSpine2Index(model_handle));
	const auto spine2_world_axis		= math::ConvertRotMatrixToAxis(spine2_world_m);
	auto	   left_fore_arm_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetSpine2Index(model_handle));
	const auto left_fore_arm_world_axis = math::ConvertRotMatrixToAxis(left_fore_arm_world_m);
	auto	   left_hand_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetLeftHandIndex(model_handle));
	const auto left_hand_world_pos		= matrix::GetPos(left_hand_world_m);

	if (m_ray_data.left_hand_cast_pos)
	{
		const auto destination = *m_ray_data.left_hand_cast_pos + VGet(0.0f, 6.0f, 0.0f);

		// 右腕IK処理
		ik_solver::TwoBoneIK(
			model_handle, destination,
			humanoid_frame->GetLeftHandIndex(model_handle),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetLeftArmIndex(model_handle))),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetLeftForeArmIndex(model_handle))),
			ik_solver::RotDirKind::kLeft, false, std::make_optional<AxisData>(-spine2_world_axis.x_axis, AxisKind::kRight));

		if (m_ray_data.left_hand_hit_triangle)
		{
			// 右手IK処理
			ik_solver::OneBoneIK(model_handle, destination + left_fore_arm_world_axis.y_axis * 2.0f,
				humanoid_frame->GetLeftHandIndex(model_handle),
				std::make_optional<AxisData>(m_ray_data.left_hand_hit_triangle->GetNormalVector(), AxisKind::kUp));
		}
	}

	// 腕が伸びきっていた場合、到達していないとみなす
	auto	   left_fore_arm_local_m = MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex(model_handle));
	const auto left_fore_arm_local_rot_m = matrix::GetRotMatrix(left_fore_arm_local_m);
	if (left_fore_arm_local_rot_m == MGetIdent())
	{
		return ResultKind::kFailure;
	}

	return ResultKind::kSuccess;
}

const HumanoidArmIKSolver::ResultKind HumanoidArmIKSolver::ApplyRightArmIKOnGround()
{
	m_right_ik_kind.at(TimeKind::kCurrent)	= IKKind::kHandOnGround;

	const auto model_handle					= m_modeler->GetModelHandle();
	const auto humanoid_frame				= m_humanoid.GetHumanoidFrame();

	// 行列情報を取得
	auto	   spine2_world_m				= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetSpine2Index(model_handle));
	const auto spine2_world_axis			= math::ConvertRotMatrixToAxis(spine2_world_m);
	auto	   right_fore_arm_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetSpine2Index(model_handle));
	const auto right_fore_arm_world_axis	= math::ConvertRotMatrixToAxis(right_fore_arm_world_m);
	auto	   right_hand_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetRightHandIndex(model_handle));
	const auto right_hand_world_pos			= matrix::GetPos(right_hand_world_m);
	//auto	   right_hand_middle4_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, humanoid_frame->GetRightHandMiddle4Index(model_handle));
	//const auto right_hand_middle4_world_pos	= matrix::GetPos(right_hand_middle4_world_m);

	if (m_ray_data.right_hand_cast_pos)
	{
		const auto destination = *m_ray_data.right_hand_cast_pos + VGet(0.0f, 6.0f, 0.0f);

		// 右腕IK処理
		ik_solver::TwoBoneIK(
			model_handle, destination,
			humanoid_frame->GetRightHandIndex(model_handle),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetRightArmIndex		(model_handle))),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid_frame->GetRightForeArmIndex	(model_handle))),
			ik_solver::RotDirKind::kRight, false, std::make_optional<AxisData>(-spine2_world_axis.x_axis, AxisKind::kRight));

		if (m_ray_data.right_hand_hit_triangle)
		{
			// 右手IK処理
			ik_solver::OneBoneIK(model_handle, destination + right_fore_arm_world_axis.y_axis * 2.0f,
				humanoid_frame->GetRightHandIndex(model_handle),
				std::make_optional<AxisData>(m_ray_data.right_hand_hit_triangle->GetNormalVector(), AxisKind::kUp));
		}
	}

	// 腕が伸びきっていた場合、到達していないとみなす
	auto	   right_fore_arm_local_m		= MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightForeArmIndex(model_handle));
	const auto right_fore_arm_local_rot_m	= matrix::GetRotMatrix(right_fore_arm_local_m);
	if (right_fore_arm_local_rot_m == MGetIdent())
	{
		return ResultKind::kFailure;
	}

	return ResultKind::kSuccess;
}
#pragma endregion


#pragma region ブレンドの起点を変更
void HumanoidArmIKSolver::ChangeLeftArmOriginMatrix (const bool is_set_result_m)
{
	const auto model_handle					= m_modeler->GetModelHandle();
	const auto humanoid_frame				= m_humanoid.GetHumanoidFrame();

	m_origin_arm_matrices.left_arm			= is_set_result_m ? m_result_arm_matrices.left_arm			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftArmIndex		(model_handle));
	m_origin_arm_matrices.left_fore_arm		= is_set_result_m ? m_result_arm_matrices.left_fore_arm		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftForeArmIndex	(model_handle));
	m_origin_arm_matrices.left_hand			= is_set_result_m ? m_result_arm_matrices.left_hand			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetLeftHandIndex		(model_handle));

	m_left_arm_blend_timer = 0.0f;
}

void HumanoidArmIKSolver::ChangeRightArmOriginMatrix(const bool is_set_result_m)
{
	const auto model_handle					= m_modeler->GetModelHandle();
	const auto humanoid_frame				= m_humanoid.GetHumanoidFrame();

	m_origin_arm_matrices.right_arm			= is_set_result_m ? m_result_arm_matrices.right_arm			: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightArmIndex		(model_handle));
	m_origin_arm_matrices.right_fore_arm	= is_set_result_m ? m_result_arm_matrices.right_fore_arm	: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightForeArmIndex	(model_handle));
	m_origin_arm_matrices.right_hand		= is_set_result_m ? m_result_arm_matrices.right_hand		: MV1GetFrameLocalMatrix(model_handle, humanoid_frame->GetRightHandIndex	(model_handle));

	m_right_arm_blend_timer = 0.0f;
}
#pragma endregion
