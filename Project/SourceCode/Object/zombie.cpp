#include "zombie.hpp"
#include "../Part/zombie_state_controller.hpp"

Zombie::Zombie() :
	CharacterBase			(ObjName.ZOMBIE, ObjTag.ENEMY, MassKind::kMedium),
	m_state					(std::make_shared<ZombieStateController>()),
	m_move_speed			(kWalkSpeed),
	m_attack_interval_timer	(0.0f),
	m_can_grab_target		(false)
{
	m_hit_points[HitPointsPartKind::kMain]		= std::make_shared<HitPoints>(1684.0f);
	m_hit_points[HitPointsPartKind::kHead]		= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kBody]		= std::make_shared<HitPoints>(500.0f);
	m_hit_points[HitPointsPartKind::kLeftArm]	= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kRightArm]	= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kLeftLeg]	= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kRightLeg]	= std::make_shared<HitPoints>(300.0f);

	m_modeler  = std::make_shared<Modeler>(m_transform, ModelPath.ZOMBIE_05, kBasicAngle, kBasicScale);
	m_animator = std::make_shared<ZombieAnimator>(m_modeler, m_state);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_invincible_time = kInvincibleTime;

	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0.0f, -54.0f, 0.0f));
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));
	m_modeler->ApplyMatrix();

	// コライダー・トリガーを設定
	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, kCapsuleRadius);
	m_collider_creator->CreateLandingTrigger	(this, kLandingTriggerRadius);
	m_collider_creator->CreateHeadTrigger		(this, m_modeler, kHeadTriggerRadius);
	m_collider_creator->CreateBodyTrigger		(this, m_modeler, kUpBodyTriggerRadius, kDownBodyTriggerRadius);
	m_collider_creator->CreateArmTrigger		(this, m_modeler, kUpperArmTriggerRadius, kForearmTriggerRadius, kHandTriggerRadius);
	m_collider_creator->CreateLegTrigger		(this, m_modeler, kUpLegTriggerRadius, kDownLegTriggerRadius);
}

Zombie::~Zombie()
{

}

void Zombie::Init()
{

}

void Zombie::Update()
{
	if (!IsActive()) { return; }

	JudgeInvincible();

	m_state		->Update(std::static_pointer_cast<Zombie>(shared_from_this()));
	m_animator	->Update();

	CalcMoveDir();
	CalcLookDir();
	CalcMoveVelocity();

	m_collider_creator->CalcCapsuleColliderLength(this, m_modeler);
	m_collider_creator->CalcHeadTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcBodyTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcArmTriggerPos (m_modeler, m_collider);
	m_collider_creator->CalcLegTriggerPos (m_modeler, m_collider);

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));

	auto pos = m_transform->GetScale(CoordinateKind::kWorld);
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(std::static_pointer_cast<Zombie>(shared_from_this()));

	m_can_grab_target = false;
}

void Zombie::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void Zombie::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	for (auto& collider : m_collider)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}

	auto pos  = m_transform->GetPos (CoordinateKind::kWorld);
	auto axes = m_transform->GetAxes(CoordinateKind::kWorld);
	DrawLine3D(pos, pos + axes.x_axis * 100, 0xff0000);
	DrawLine3D(pos, pos + axes.y_axis * 100, 0x00ff22);
	DrawLine3D(pos, pos + axes.z_axis * 100, 0x0077ff);

	auto	   head_mat = MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos = MGetTranslateElem(head_mat);
	auto rot_y = MGetRotY(kFOV * 0.5f * math::kDegToRad);
	auto rot_x1 = math::GetRotatedPos(axes.z_axis, quat::CreateQuaternion(axes.x_axis, kFOV * 0.5f * math::kDegToRad));
	auto rot_x2 = math::GetRotatedPos(axes.z_axis, quat::CreateQuaternion(axes.x_axis, -kFOV * 0.5f * math::kDegToRad));
	auto dir1 = VTransform(axes.z_axis, rot_y);
	auto dir2 = VTransform(axes.z_axis, MInverse(rot_y));

	DrawLine3D(head_pos, head_pos + dir1 * kVisibleDistance, 0xffffff);
	DrawLine3D(head_pos, head_pos + dir2 * kVisibleDistance, 0xffffff);
	DrawLine3D(head_pos, head_pos + rot_x1 * kVisibleDistance, 0xffffff);
	DrawLine3D(head_pos, head_pos + rot_x2 * kVisibleDistance, 0xffffff);

	DrawFormatString(800,  0, 0xffffff, "%f, %f, %f", pos.x, pos.y, pos.z);
	DrawFormatString(800, 20, 0xffffff, "%d", m_hit_collider.size());
	DrawFormatString(800, 40, 0xffffff, "%f", m_move_speed);

	DrawFormatString(800,  80, 0xffffff, "main HP		: %f", m_hit_points.at(HitPointsPartKind::kMain)->GetCurrentHitPoints());
	DrawFormatString(800, 100, 0xffffff, "head HP		: %f", m_hit_points.at(HitPointsPartKind::kHead)->GetCurrentHitPoints());
	DrawFormatString(800, 120, 0xffffff, "body HP		: %f", m_hit_points.at(HitPointsPartKind::kBody)->GetCurrentHitPoints());
	DrawFormatString(800, 140, 0xffffff, "left arm HP	: %f", m_hit_points.at(HitPointsPartKind::kLeftArm)->GetCurrentHitPoints());
	DrawFormatString(800, 160, 0xffffff, "right arm HP  : %f", m_hit_points.at(HitPointsPartKind::kRightArm)->GetCurrentHitPoints());
	DrawFormatString(800, 180, 0xffffff, "left leg HP	: %f", m_hit_points.at(HitPointsPartKind::kLeftLeg)->GetCurrentHitPoints());
	DrawFormatString(800, 200, 0xffffff, "right leg HP	: %f", m_hit_points.at(HitPointsPartKind::kRightLeg)->GetCurrentHitPoints());
	DrawFormatString(800, 220, 0xffffff, "is_invincible	: %d", m_is_invincible);
}

void Zombie::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	case ColliderKind::kCollider:
		// ロケット弾着弾時の爆発エフェクトとの衝突
		if (target_name == ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT)
		{
			if (m_hit_collider.count(hit_collider_pair.target_collider)) { return; }
			m_hit_collider[hit_collider_pair.target_collider] = hit_collider_pair.target_collider->IsOneCollision();

			if (m_is_invincible) { return; }
			
			OnCollideWithExplosion(std::static_pointer_cast<Sphere>(hit_collider_pair.target_collider->GetShape()));
			
		}
		break;

		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kHeadTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kHead,		dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kUpBodyTrigger:
	case ColliderKind::kDownBodyTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kBody,		dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kLeftUpperArmTrigger:
	case ColliderKind::kLeftForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kLeftArm, dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain, dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kLeftHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kLeftArm,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}

		if (target_obj == GetStateController()->GetTargetCharacter().get() && target_collider_kind == ColliderKind::kCollider)
		{
			m_can_grab_target = true;
		}
		break;

	case ColliderKind::kRightUpperArmTrigger:
	case ColliderKind::kRightForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kRightArm,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kRightHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kRightArm, dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain, dynamic_cast<Bullet*>(target_obj)->GetPower());
		}

		if (target_obj == GetStateController()->GetTargetCharacter().get() && target_collider_kind == ColliderKind::kCollider)
		{
			m_can_grab_target = true;
		}
		break;

	case ColliderKind::kLeftUpLegTrigger:
	case ColliderKind::kLeftDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kLeftLeg,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kRightUpLegTrigger:
	case ColliderKind::kRightDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HitPointsPartKind::kRightLeg,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HitPointsPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	default:
		break;
	}
}

bool Zombie::IsTargetInSight(const int target_model_handle)
{
	auto	   target_head_mat = MV1GetFrameLocalWorldMatrix(target_model_handle, MV1SearchFrame(target_model_handle, BonePath.HEAD));
	const auto target_head_pos = MGetTranslateElem(target_head_mat);

	// 頭部を基準に円錐状の視界を作り出す
	auto	   head_mat		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos		= MGetTranslateElem(head_mat);
	const auto head_axes	= math::ConvertRotMatrixToAxes(head_mat);

	const auto fov			= kFOV * math::kDegToRad;
	const auto max_distance = kVisibleDistance;
	const auto distance_v	= target_head_pos - head_pos;
	const auto distance		= VSize(distance_v);
	const auto dir			= v3d::GetNormalizedV(distance_v);

	return (VDot(dir, -head_axes.z_axis) > cos(fov * 0.5f)) && (distance < max_distance);
}

float Zombie::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleController::LayerKind::kWorld);
}

void Zombie::Move()
{
	m_move_dir_offset_speed = kMoveDirOffsetSpeed;
}

void Zombie::TrackMove(const VECTOR& pos)
{
	m_move_dir[TimeKind::kNext] = v3d::GetNormalizedV(pos - m_transform->GetPos(CoordinateKind::kWorld));
}

void Zombie::UpdateGrabRun()
{
	m_move_dir_offset_speed = 0.5f;
	m_move_speed = kRunGrabSpeed;
}

void Zombie::CalcMoveSpeed()
{
	m_move_speed = kWalkSpeed;
}

void Zombie::CalcMoveSpeedStop()
{
	m_move_speed = 0.0f;
}

void Zombie::CalcMoveSpeedRun()
{
	m_move_speed = kRunSpeed;
}

void Zombie::OnCollideWithExplosion(const std::shared_ptr<Sphere> sphere)
{
	// TODO : 後にエフェクト側に処理内容を委ねる

	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld) + VGet(0, 50, 0);
	const auto explosion_pos	= sphere->GetPos();
	const auto distance			= pos - explosion_pos;

	if (VSize(distance) > sphere->GetRadius()) { return; }

	const auto dir_xz			= v3d::GetNormalizedV(VGet(distance.x, 0.0f, distance.z));
	const auto dir				= v3d::GetNormalizedV(dir_xz + VGet(0.0f, 0.5f, 0.0f));

	m_knockback_speed			= 100.0f;
	m_knockback_deceleration	= 5.0f;
	m_knockback_velocity		= dir * m_knockback_speed;
}

void Zombie::CalcLookDir()
{
	if (m_move_dir[TimeKind::kCurrent] != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = m_move_dir[TimeKind::kCurrent];
	}

	// ヨー角回転を取得し、-π～πで値を管理する
	const VECTOR current_yaw	= math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const VECTOR next_yaw		= math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	VECTOR distance = next_yaw - current_yaw;
	distance.y = math::ConnectMinusPiToPi(distance.y);

	// TODO : 後に変更
	float angle1	= 1.7f * math::kDegToRad;
	float threshold = 10.0f * math::kDegToRad;

	// カメラを基準にして右側であった場合は反転
	if (distance.y > 0) { angle1 *= -1; }

	// 回転を適用
	const Quaternion rot_q = quat::CreateQuaternion(axis::GetWorldYAxis(), -angle1);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	const float angle = math::GetYawBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	if (angle < threshold)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);
	}
}

void Zombie::CalcMoveVelocity()
{
	m_move_velocity = m_move_dir[TimeKind::kCurrent] * m_move_speed;
	m_velocity += m_move_velocity;
}
