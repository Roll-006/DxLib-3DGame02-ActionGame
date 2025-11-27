#include "humanoid_foot_ik_solver.hpp"

std::unordered_map<std::string, ModelFrameAngleLimitData> HumanoidFootIKSolver::angle_limits = {};

HumanoidFootIKSolver::HumanoidFootIKSolver(
	const std::shared_ptr<AnimatorBase>& animator, const std::shared_ptr<Modeler>& modeler, 
	std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& colliders, HumanoidLegRayData& ray_data) :
	m_animator				(animator),
	m_modeler				(modeler),
	m_colliders				(colliders),
	m_ray_data				(ray_data),
	m_can_left_foot_ik		{ {TimeKind::kCurrent, true}, {TimeKind::kPrev, true} },
	m_can_right_foot_ik		{ {TimeKind::kCurrent, true}, {TimeKind::kPrev, true} },
	m_left_toe_base_offset	(0.0f),
	m_right_toe_base_offset	(0.0f),
	m_origin_leg_matrices	(),
	m_armature_blend_timer	(0.0f),
	m_left_leg_blend_timer	(0.0f),
	m_right_leg_blend_timer	(0.0f)
{
	if (angle_limits.empty())
	{
		nlohmann::json data;
		if (json_loader::Load("Data/JSON/model_foot_angle_limits.json", data))
		{
			for (const auto& [frame_name, angle_limit] : data.at("humanoid").items())
			{
				angle_limits[frame_name] = angle_limit.get<ModelFrameAngleLimitData>();
			}
		}
	}
}

HumanoidFootIKSolver::~HumanoidFootIKSolver()
{

}

void HumanoidFootIKSolver::Init(const std::shared_ptr<IHumanoid>& humanoid)
{
	ChagneArmatureOriginMatrix	(humanoid);
	ChangeLeftLegOriginMatrix	(humanoid);
	ChangeRightLegOriginMatrix	(humanoid);
}

void HumanoidFootIKSolver::Update(const std::shared_ptr<IHumanoid>& humanoid)
{
	// Armatureの位置を初期化
	const auto model_handle			= m_modeler->GetModelHandle();
	const auto armature_index		= humanoid->GetHumanoidFrame()->GetArmatureIndex(model_handle);
	auto	   armature_m			= MV1GetFrameLocalMatrix(model_handle, armature_index);
	matrix::SetPos(armature_m, v3d::GetZeroV());
	MV1SetFrameUserLocalMatrix(model_handle, armature_index, armature_m);

	// 左足の光線位置を更新
	const auto left_foot_segment		= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kLeftFootRay)->GetShape());
	auto	   left_foot_m				= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetLeftFootIndex(model_handle));
	const auto left_foot_pos			= MGetTranslateElem(left_foot_m);
	left_foot_segment->SetBeginPos(left_foot_pos + VGet(0.0f, m_ray_data.left_foot_ray_offset, 0.0f), false);

	// 右足の光線位置を更新
	const auto right_foot_segment		= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kRightFootRay)->GetShape());
	auto	   right_foot_m				= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetRightFootIndex(model_handle));
	const auto right_foot_pos			= MGetTranslateElem(right_foot_m);
	right_foot_segment->SetBeginPos(right_foot_pos + VGet(0.0f, m_ray_data.right_foot_ray_offset, 0.0f), false);

	// 左足のつま先のベースの光線位置を更新
	const auto left_toe_base_segment	= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kLeftToeBaseRay)->GetShape());
	auto	   left_toe_base_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetLeftToeBaseIndex(model_handle));
	const auto left_toe_base_pos		= MGetTranslateElem(left_toe_base_m);
	left_toe_base_segment->SetBeginPos(left_toe_base_pos + VGet(0.0f, m_ray_data.left_toe_base_ray_offset, 0.0f), false);

	// 右足のつま先のベースの光線位置を更新
	const auto right_toe_base_segment	= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kRightToeBaseRay)->GetShape());
	auto	   right_toe_base_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetRightToeBaseIndex(model_handle));
	const auto right_toe_base_pos		= MGetTranslateElem(right_toe_base_m);
	right_toe_base_segment->SetBeginPos(right_toe_base_pos + VGet(0.0f, m_ray_data.right_toe_base_ray_offset, 0.0f), false);
}


#pragma region コライダー
void HumanoidFootIKSolver::CreateRightLegRay(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	// 右足の光線を生成
	auto	   right_leg_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetRightLegIndex(model_handle));
	const auto right_leg_pos		= MGetTranslateElem(right_leg_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightLegRay, std::make_shared<Segment>(right_leg_pos + VGet(0.0f, m_ray_data.right_leg_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.leg_ray_length), physical_obj));
}

void HumanoidFootIKSolver::CreateFootRay(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	// 左足の光線を生成
	auto	   left_foot_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetLeftFootIndex(model_handle));
	const auto left_foot_pos		= MGetTranslateElem(left_foot_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftFootRay, std::make_shared<Segment>(left_foot_pos + VGet(0.0f, m_ray_data.left_foot_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.foot_ray_length), physical_obj));

	// 右足の光線を生成
	auto	   right_foot_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetRightFootIndex(model_handle));
	const auto right_foot_pos		= MGetTranslateElem(right_foot_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightFootRay, std::make_shared<Segment>(right_foot_pos + VGet(0.0f, m_ray_data.right_foot_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.foot_ray_length), physical_obj));
}

void HumanoidFootIKSolver::CreateFoeBaseRay(PhysicalObjBase* physical_obj, const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	// 左足の光線を生成
	auto	   left_foe_base_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetLeftToeBaseIndex(model_handle));
	const auto left_foe_base_pos	= MGetTranslateElem(left_foe_base_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftToeBaseRay, std::make_shared<Segment>(left_foe_base_pos + VGet(0.0f, m_ray_data.left_toe_base_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.toe_base_ray_length), physical_obj));

	// 右足の光線を生成
	auto	   right_foe_base_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetRightToeBaseIndex(model_handle));
	const auto right_foe_base_pos	= MGetTranslateElem(right_foe_base_m);
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightToeBaseRay, std::make_shared<Segment>(right_foe_base_pos + VGet(0.0f, m_ray_data.right_toe_base_ray_offset, 0.0f), -axis::GetWorldYAxis(), m_ray_data.toe_base_ray_length), physical_obj));
}

void HumanoidFootIKSolver::DeleteRightLegRay(PhysicalObjBase* physical_obj)
{
	physical_obj->RemoveCollider(ColliderKind::kRightLegRay);
}

void HumanoidFootIKSolver::CalcRightLegRayPos(const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	const auto right_leg_segment	= std::static_pointer_cast<Segment>(m_colliders.at(ColliderKind::kRightLegRay)->GetShape());
	auto	   right_leg_m			= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), humanoid->GetHumanoidFrame()->GetRightLegIndex(model_handle));
	const auto right_leg_pos		= MGetTranslateElem(right_leg_m);
	right_leg_segment->SetBeginPos(right_leg_pos + VGet(0.0f, m_ray_data.right_leg_ray_offset, 0.0f), false);
}
#pragma endregion


void HumanoidFootIKSolver::OnFootIK(const std::shared_ptr<IHumanoid>& humanoid)
{
	JudgeExecuteIK		(humanoid);
	CalcToeBaseOffset	(humanoid);
	DownArmature		(humanoid);
	OnLeftLegIK			(humanoid);
	OnRightLegIK		(humanoid);

	m_ray_data.left_foot_cast_pos		= std::nullopt;
	m_ray_data.left_toe_base_cast_pos	= std::nullopt;
	m_ray_data.right_foot_cast_pos		= std::nullopt;
	m_ray_data.right_toe_base_cast_pos	= std::nullopt;
}

void HumanoidFootIKSolver::OnRightKneelCrouchIK(const std::shared_ptr<IHumanoid>& humanoid)
{
	JudgeExecuteIK		(humanoid);
	CalcToeBaseOffset	(humanoid);
	OnLeftLegIK			(humanoid);
	OnRightKneelIK		(humanoid);

	m_ray_data.left_leg_cast_pos		= std::nullopt;
	m_ray_data.left_foot_cast_pos		= std::nullopt;
	m_ray_data.left_toe_base_cast_pos	= std::nullopt;
	m_ray_data.right_leg_cast_pos		= std::nullopt;
	m_ray_data.right_foot_cast_pos		= std::nullopt;
	m_ray_data.right_toe_base_cast_pos	= std::nullopt;
}

void HumanoidFootIKSolver::BlendFrame(const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle	= m_modeler->GetModelHandle();

	// 補間係数tを取得
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kNoneScale);
	math::Increase(m_armature_blend_timer,  delta_time, armature_blend_time,	false);
	math::Increase(m_left_leg_blend_timer,  delta_time, leg_blend_time,			false);
	math::Increase(m_right_leg_blend_timer, delta_time, leg_blend_time,			false);
	const auto armature_t	= math::GetUnitValue<float, float>(0.0f, armature_blend_time,	m_armature_blend_timer);
	const auto left_leg_t	= math::GetUnitValue<float, float>(0.0f, leg_blend_time,		m_left_leg_blend_timer);
	const auto right_leg_t	= math::GetUnitValue<float, float>(0.0f, leg_blend_time,		m_right_leg_blend_timer);

	// ブレンド結果を取得
	const auto result_armature_m		= math::GetLerpMatrix(m_origin_leg_matrices.armature,		MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetArmatureIndex		(model_handle)),armature_t);
	const auto result_hips_m			= math::GetLerpMatrix(m_origin_leg_matrices.hips,			MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetHipsIndex			(model_handle)),armature_t);

	const auto result_left_up_leg_m		= math::GetLerpMatrix(m_origin_leg_matrices.left_up_leg,	MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftUpLegIndex		(model_handle)),left_leg_t);
	const auto result_left_leg_m		= math::GetLerpMatrix(m_origin_leg_matrices.left_leg,		MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftLegIndex		(model_handle)),left_leg_t);
	const auto result_left_foot_m		= math::GetLerpMatrix(m_origin_leg_matrices.left_foot,		MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftFootIndex		(model_handle)),left_leg_t);
	const auto result_left_toe_base_m	= math::GetLerpMatrix(m_origin_leg_matrices.left_toe_base,	MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeBaseIndex	(model_handle)),left_leg_t);
	const auto result_left_toe_end_m	= math::GetLerpMatrix(m_origin_leg_matrices.left_toe_end,	MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeEndIndex	(model_handle)),left_leg_t);

	const auto result_right_up_leg_m	= math::GetLerpMatrix(m_origin_leg_matrices.right_up_leg,	MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightUpLegIndex	(model_handle)),right_leg_t);
	const auto result_right_leg_m		= math::GetLerpMatrix(m_origin_leg_matrices.right_leg,		MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightLegIndex		(model_handle)),right_leg_t);
	const auto result_right_foot_m		= math::GetLerpMatrix(m_origin_leg_matrices.right_foot,		MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightFootIndex		(model_handle)),right_leg_t);
	const auto result_right_toe_base_m	= math::GetLerpMatrix(m_origin_leg_matrices.right_toe_base, MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeBaseIndex	(model_handle)),right_leg_t);
	const auto result_right_toe_end_m	= math::GetLerpMatrix(m_origin_leg_matrices.right_toe_end,	MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeEndIndex	(model_handle)),right_leg_t);

	// ブレンド結果をモデルに設定
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetArmatureIndex		(model_handle),result_armature_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetHipsIndex			(model_handle),result_hips_m);
	
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftUpLegIndex	(model_handle),result_left_up_leg_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftLegIndex		(model_handle),result_left_leg_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftFootIndex		(model_handle),result_left_foot_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeBaseIndex	(model_handle),result_left_toe_base_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeEndIndex	(model_handle),result_left_toe_end_m);
	
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightUpLegIndex	(model_handle),result_right_up_leg_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightLegIndex		(model_handle),result_right_leg_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightFootIndex	(model_handle),result_right_foot_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeBaseIndex	(model_handle),result_right_toe_base_m);
	MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeEndIndex	(model_handle),result_right_toe_end_m);
}

void HumanoidFootIKSolver::JudgeExecuteIK(const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto is_current_move_anim = m_animator->GetAnimTag (AnimatorBase::BodyKind::kLowerBody, TimeKind::kCurrent) == AnimTag.MOVE;
	const auto is_prev_move_anim	= m_animator->GetAnimTag (AnimatorBase::BodyKind::kLowerBody, TimeKind::kPrev)	  == AnimTag.MOVE;

	// データのシフト
	m_can_left_foot_ik .at(TimeKind::kPrev) = m_can_left_foot_ik .at(TimeKind::kCurrent);
	m_can_right_foot_ik.at(TimeKind::kPrev) = m_can_right_foot_ik.at(TimeKind::kCurrent);

	if (!is_current_move_anim)
	{
		m_can_left_foot_ik .at(TimeKind::kCurrent)	= true;
		m_can_right_foot_ik.at(TimeKind::kCurrent)	= true;

		// ブレンドの起点行列を設定
		if(!m_can_left_foot_ik .at(TimeKind::kPrev)) { ChangeLeftLegOriginMatrix (humanoid); }
		if(!m_can_right_foot_ik.at(TimeKind::kPrev)) { ChangeRightLegOriginMatrix(humanoid); }

		return;
	}

	const auto play_rate			= m_animator->GetPlayRate		(AnimatorBase::BodyKind::kLowerBody);
	const auto landing_play_rate	= m_animator->GetLandingPlayRate(AnimatorBase::BodyKind::kLowerBody) * 0.25f;
	
	m_can_left_foot_ik .at(TimeKind::kCurrent) = play_rate >= 0.5f - landing_play_rate && play_rate < 0.5f + landing_play_rate;
	m_can_right_foot_ik.at(TimeKind::kCurrent) = play_rate <= 0.0f + landing_play_rate || play_rate > 1.0f - landing_play_rate;
}

void HumanoidFootIKSolver::CalcToeBaseOffset(const std::shared_ptr<IHumanoid>& humanoid)
{
	if (!m_can_left_foot_ik.at(TimeKind::kCurrent) && !m_can_right_foot_ik.at(TimeKind::kCurrent)) { return; }

	const auto model_handle = m_modeler->GetModelHandle();

	// 左足のオフセット値を設定
	auto	   left_foot_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftFootIndex(model_handle));
	const auto left_foot_pos		= MGetTranslateElem(left_foot_m);
	auto	   left_toe_base_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeBaseIndex(model_handle));
	const auto left_toe_base_pos	= MGetTranslateElem(left_toe_base_m);
	m_left_toe_base_offset			= std::abs(left_toe_base_pos.y - left_foot_pos.y) + m_ray_data.left_heels_offset;

	// 右足のオフセット値を設定
	auto	   right_foot_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightFootIndex(model_handle));
	const auto right_foot_pos		= MGetTranslateElem(right_foot_m);
	auto	   right_toe_base_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeBaseIndex(model_handle));
	const auto right_toe_base_pos	= MGetTranslateElem(right_toe_base_m);
	m_right_toe_base_offset			= std::abs(right_toe_base_pos.y - right_foot_pos.y) + m_ray_data.right_heels_offset;
}

void HumanoidFootIKSolver::DownArmature(const std::shared_ptr<IHumanoid>& humanoid)
{
	if (!m_can_left_foot_ik.at(TimeKind::kCurrent) && !m_can_right_foot_ik.at(TimeKind::kCurrent))	{ return; }
	if (!m_ray_data.left_foot_cast_pos && !m_ray_data.right_foot_cast_pos)						{ return; }

	const auto left_foot_ray	= m_colliders.at(ColliderKind::kLeftFootRay);
	const auto right_foot_ray	= m_colliders.at(ColliderKind::kRightFootRay);
	if (left_foot_ray && right_foot_ray)
	{
		const auto left_ray_begin_pos	= std::static_pointer_cast<Segment>(left_foot_ray ->GetShape())->GetBeginPos();
		const auto right_ray_begin_pos	= std::static_pointer_cast<Segment>(right_foot_ray->GetShape())->GetBeginPos();

		// 距離が長い方をArmature落とす距離のベースとして使用
		auto left_distance  = 0.0f;
		auto right_distance = 0.0f;
		if(m_can_left_foot_ik.at(TimeKind::kCurrent)  && m_ray_data.left_foot_cast_pos) { left_distance  = left_ray_begin_pos.y  - m_ray_data.left_foot_cast_pos->y;  }
		if(m_can_right_foot_ik.at(TimeKind::kCurrent) && m_ray_data.right_foot_cast_pos){ right_distance = right_ray_begin_pos.y - m_ray_data.right_foot_cast_pos->y; }
		const auto is_left = left_distance >= right_distance;
		auto drop_distance = is_left ? left_distance : right_distance;

		// Armatureを落とす距離を補正
		drop_distance -= is_left ? (m_ray_data.left_foot_ray_offset + m_left_toe_base_offset) : (m_ray_data.right_foot_ray_offset + m_right_toe_base_offset);
		if (drop_distance < 0.0f) { drop_distance = 0.0f; }

		// Armatureを落とす
		const auto model_handle		= m_modeler->GetModelHandle();
		const auto armature_index	= humanoid->GetHumanoidFrame()->GetArmatureIndex(model_handle);
		auto	   armature_m		= MV1GetFrameLocalWorldMatrix(model_handle, armature_index);
		auto	   armature_pos		= MGetTranslateElem(armature_m);
		armature_pos.y -= drop_distance;

		// TODO : リファクタリング必須
		auto transform_m = armature_m;
		matrix::SetPos(transform_m, armature_pos);
		const auto result_m = transform_m * MInverse(armature_m);

		// Armatureのブレンドの起点を設定
		if (!m_can_left_foot_ik.at(TimeKind::kPrev) || !m_can_right_foot_ik.at(TimeKind::kPrev))
		{
			ChagneArmatureOriginMatrix(humanoid);
		}

		MV1SetFrameUserLocalMatrix(model_handle, armature_index, result_m);
	}
}

void HumanoidFootIKSolver::UpHips(const std::shared_ptr<IHumanoid>& humanoid)
{
	if (!m_can_left_foot_ik.at(TimeKind::kCurrent) && !m_can_right_foot_ik.at(TimeKind::kCurrent))	{ return; }
	if (!m_ray_data.left_foot_cast_pos && !m_ray_data.right_foot_cast_pos)						{ return; }
}

void HumanoidFootIKSolver::OnLeftLegIK(const std::shared_ptr<IHumanoid>& humanoid)
{
	if (!m_can_left_foot_ik.at(TimeKind::kCurrent)) { return; }

	const auto model_handle = m_modeler->GetModelHandle();
	
	// 左足IK処理
	if (m_ray_data.left_foot_cast_pos)
	{
		const auto aid_axis = math::ConvertRotMatrixToAxis(MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetHipsIndex(model_handle)));

		ik_solver::TwoBoneIK(
			model_handle, *m_ray_data.left_foot_cast_pos + VGet(0.0f, m_left_toe_base_offset, 0.0f),
			humanoid->GetHumanoidFrame()->GetLeftFootIndex(model_handle),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid->GetHumanoidFrame()->GetLeftUpLegIndex(model_handle))),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid->GetHumanoidFrame()->GetLeftLegIndex	 (model_handle))),
			ik_solver::RotDirKind::kLeft, std::make_optional<AidAxisData>(AidAxisData{ -aid_axis.x_axis, AxisKind::kLeft }));
	}

	// 左のつま先が地面に沿うように回転
	if (m_ray_data.left_toe_base_cast_pos)
	{
		const auto hit_triangles = m_colliders.at(ColliderKind::kLeftToeBaseRay)->GetHitTriangles();
		if (hit_triangles.size() > 0)
		{
			const auto normal_v		= hit_triangles.front().GetNormalVector();
			const auto destination	= *m_ray_data.left_toe_base_cast_pos + VGet(0.0f, m_ray_data.left_toe_offset, 0.0f);

			ik_solver::OneBoneIK(model_handle, destination, humanoid->GetHumanoidFrame()->GetLeftFootIndex(model_handle), std::make_optional<AidAxisData>(AidAxisData{ normal_v, AxisKind::kUp }));
		}
	}
}

void HumanoidFootIKSolver::OnRightLegIK(const std::shared_ptr<IHumanoid>& humanoid)
{
	if (!m_can_right_foot_ik.at(TimeKind::kCurrent)) { return; }

	const auto model_handle = m_modeler->GetModelHandle();

	// 右足IK処理
	if (m_ray_data.right_foot_cast_pos)
	{
		const auto aid_axis = math::ConvertRotMatrixToAxis(MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetHipsIndex(model_handle)));

		ik_solver::TwoBoneIK(
			model_handle, *m_ray_data.right_foot_cast_pos + VGet(0.0f, m_right_toe_base_offset, 0.0f),
			humanoid->GetHumanoidFrame()->GetRightFootIndex(model_handle),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid->GetHumanoidFrame()->GetRightUpLegIndex	(model_handle))),
			angle_limits.at(MV1GetFrameName(model_handle, humanoid->GetHumanoidFrame()->GetRightLegIndex	(model_handle))),
			ik_solver::RotDirKind::kLeft, std::make_optional<AidAxisData>(AidAxisData{ -aid_axis.x_axis, AxisKind::kLeft }));
	}

	// 右のつま先が地面に沿うように回転
	if (m_ray_data.right_toe_base_cast_pos)
	{
		const auto hit_triangles = m_colliders.at(ColliderKind::kRightToeBaseRay)->GetHitTriangles();
		if (hit_triangles.size() > 0)
		{
			const auto normal_v		= hit_triangles.front().GetNormalVector();
			const auto destination	= *m_ray_data.right_toe_base_cast_pos + VGet(0.0f, m_ray_data.right_toe_offset, 0.0f);

			ik_solver::OneBoneIK(model_handle, destination, humanoid->GetHumanoidFrame()->GetRightFootIndex(model_handle), std::make_optional<AidAxisData>(AidAxisData{ normal_v, AxisKind::kUp }));
		}
	}
}

void HumanoidFootIKSolver::OnRightKneelIK(const std::shared_ptr<IHumanoid>& humanoid)
{
	if (!m_can_right_foot_ik.at(TimeKind::kCurrent)) { return; }

	const auto model_handle = m_modeler->GetModelHandle();

	// 右膝IK処理
	if (m_ray_data.right_leg_cast_pos)
	{
		const auto destination	= *m_ray_data.right_leg_cast_pos + VGet(0.0f, m_ray_data.right_leg_offset, 0.0f);
		const auto aid_axis		= -math::ConvertRotMatrixToAxis(MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetHipsIndex(model_handle))).x_axis;

		ik_solver::OneBoneIK(model_handle, destination, humanoid->GetHumanoidFrame()->GetRightUpLegIndex(model_handle), std::make_optional<AidAxisData>(AidAxisData{ aid_axis, AxisKind::kLeft }));
	}

	// 右足IK処理
	if (m_ray_data.right_foot_cast_pos)
	{
		auto	   right_foot_m			= MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightFootIndex(model_handle));
		const auto right_foot_pos		= MGetTranslateElem(right_foot_m);
		auto	   right_toe_end_m		= MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeEndIndex(model_handle));
		const auto right_toe_end_pos	= MGetTranslateElem(right_toe_end_m);
		const auto destination			= *m_ray_data.right_foot_cast_pos + VGet(0.0f, std::abs(right_foot_pos.y - right_toe_end_pos.y), 0.0f);
		const auto aid_axis				= math::ConvertRotMatrixToAxis(MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightUpLegIndex(model_handle))).x_axis;

		ik_solver::OneBoneIK(model_handle, destination, humanoid->GetHumanoidFrame()->GetRightLegIndex(model_handle), std::make_optional<AidAxisData>(AidAxisData{ aid_axis, AxisKind::kLeft }));
	}

	//auto	   right_leg_m = MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetRightLegIndex());
	//const auto right_leg_pos = MGetTranslateElem(right_leg_m);
	//const auto destination = m_ray_data.right_leg_cast_pos->y + m_ray_data.right_leg_offset;

	//// 目標地点より低い位置にあった場合、Hipsを上にあげる
	//if (right_leg_pos.y < destination)
	//{
	//	const auto distance = std::abs(destination - right_leg_pos.y);
	//	auto	   hips_world_m = MV1GetFrameLocalWorldMatrix(model_handle, humanoid->GetHipsIndex());
	//	auto	   hips_world_pos = MGetTranslateElem(hips_world_m);
	//	hips_world_pos.y += distance;

	//	const auto parent_local_m = MV1GetFrameLocalMatrix(model_handle, MV1GetFrameParent(model_handle, humanoid->GetHipsIndex()));
	//	const auto parent_world_m = MV1GetFrameLocalWorldMatrix(model_handle, MV1GetFrameParent(model_handle, humanoid->GetHipsIndex()));

	//	matrix::SetPos(hips_world_m, hips_world_pos);
	//	const auto hips_local_m = hips_world_m * MInverse(parent_local_m * parent_world_m) * MGetRotY(180.0f * math::kDegToRad);
	//	//MV1SetFrameUserLocalMatrix(model_handle, humanoid->GetHipsIndex(), hips_local_m);
	//}
}

void HumanoidFootIKSolver::ChagneArmatureOriginMatrix(const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	m_origin_leg_matrices.armature			= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetArmatureIndex(model_handle));
	m_origin_leg_matrices.hips				= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetHipsIndex	(model_handle));

	m_armature_blend_timer = 0.0f;
}

void HumanoidFootIKSolver::ChangeLeftLegOriginMatrix(const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	m_origin_leg_matrices.left_up_leg		= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftUpLegIndex	(model_handle));
	m_origin_leg_matrices.left_leg			= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftLegIndex		(model_handle));
	m_origin_leg_matrices.left_foot			= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftFootIndex	(model_handle));
	m_origin_leg_matrices.left_toe_base		= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeBaseIndex	(model_handle));
	m_origin_leg_matrices.left_toe_end		= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetLeftToeEndIndex	(model_handle));

	m_left_leg_blend_timer = 0.0f;
}

void HumanoidFootIKSolver::ChangeRightLegOriginMatrix(const std::shared_ptr<IHumanoid>& humanoid)
{
	const auto model_handle = m_modeler->GetModelHandle();

	m_origin_leg_matrices.right_up_leg		= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightUpLegIndex	(model_handle));
	m_origin_leg_matrices.right_leg			= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightLegIndex	(model_handle));
	m_origin_leg_matrices.right_foot		= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightFootIndex	(model_handle));
	m_origin_leg_matrices.right_toe_base	= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeBaseIndex(model_handle));
	m_origin_leg_matrices.right_toe_end		= MV1GetFrameLocalMatrix(model_handle, humanoid->GetHumanoidFrame()->GetRightToeEndIndex	(model_handle));

	m_right_leg_blend_timer = 0.0f;
}
