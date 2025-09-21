#include "zombie.hpp"
#include "../Part/zombie_state_controller.hpp"

Zombie::Zombie() :
	EnemyBase			(ObjName.ZOMBIE, MassKind::kMedium),
	m_state				(std::make_shared<ZombieStateController>()),
	m_can_grab_target	(false)
{
	// TODO : JSON指定
	m_health[HealthPartKind::kMain]		= std::make_shared<Health>(1684.0f, 1684.0f);
	m_health[HealthPartKind::kHead]		= std::make_shared<Health>(80.0f,  80.0f);
	m_health[HealthPartKind::kBody]		= std::make_shared<Health>(500.0f, 500.0f);
	m_health[HealthPartKind::kLeftArm]	= std::make_shared<Health>(100.0f, 100.0f);
	m_health[HealthPartKind::kRightArm]	= std::make_shared<Health>(100.0f, 100.0f);
	m_health[HealthPartKind::kLeftLeg]	= std::make_shared<Health>(100.0f, 100.0f);
	m_health[HealthPartKind::kRightLeg]	= std::make_shared<Health>(100.0f, 100.0f);

	m_modeler  = std::make_shared<Modeler>(m_transform, ModelPath.ZOMBIE_01, kBasicAngle, kBasicScale);
	m_animator = std::make_shared<ZombieAnimator>(m_modeler, m_state);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_invincible_time		= kInvincibleTime;
	m_attack_interval_time	= kAttackIntervalTime;

	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0.0f, -54.0f, 0.0f));
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));
	m_modeler->ApplyMatrix();

	// コライダー・トリガーを設定
	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, kCapsuleRadius);
	m_collider_creator->CreateLandingTrigger	(this, kLandingTriggerRadius);
	m_collider_creator->CreateVisionTrigger		(this, m_modeler, 300, kFOV * math::kDegToRad);
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

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));

	m_collider_creator->CalcCapsuleColliderDirAndLength	(m_modeler, m_collider, m_transform);
	m_collider_creator->CalcVisionTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcHeadTriggerPos	(m_modeler, m_collider);
	m_collider_creator->CalcBodyTriggerPos	(m_modeler, m_collider);
	m_collider_creator->CalcArmTriggerPos	(m_modeler, m_collider);
	m_collider_creator->CalcLegTriggerPos	(m_modeler, m_collider);

	auto pos = m_transform->GetScale(CoordinateKind::kWorld);
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(std::static_pointer_cast<Zombie>(shared_from_this()));

	m_can_grab_target	 = false;
	m_is_target_in_sight = false;
}

void Zombie::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
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

	DrawFormatString(100, 100, 0xffffff, "MainHP     : %f", m_health.at(HealthPartKind::kMain)->GetCurrentHealth());
	DrawFormatString(100, 120, 0xffffff, "HeadHP     : %f", m_health.at(HealthPartKind::kHead)->GetCurrentHealth());
	DrawFormatString(100, 140, 0xffffff, "BodyHP	 : %f", m_health.at(HealthPartKind::kBody)->GetCurrentHealth());
	DrawFormatString(100, 160, 0xffffff, "LeftArmHP  : %f", m_health.at(HealthPartKind::kLeftArm)->GetCurrentHealth());
	DrawFormatString(100, 180, 0xffffff, "RightArmHP : %f", m_health.at(HealthPartKind::kRightArm)->GetCurrentHealth());
	DrawFormatString(100, 200, 0xffffff, "LeftLegHP  : %f", m_health.at(HealthPartKind::kLeftLeg)->GetCurrentHealth());
	DrawFormatString(100, 220, 0xffffff, "RightLegHP : %f", m_health.at(HealthPartKind::kRightLeg)->GetCurrentHealth());

	auto pos  = m_transform->GetPos (CoordinateKind::kWorld);
	auto axes = m_transform->GetAxes(CoordinateKind::kWorld);
	DrawLine3D(pos, pos + axes.x_axis * 100, 0xff0000);
	DrawLine3D(pos, pos + axes.y_axis * 100, 0x00ff22);
	DrawLine3D(pos, pos + axes.z_axis * 100, 0x0077ff);
}

void Zombie::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();
	const auto			action_state_kind		= static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	case ColliderKind::kVisionTrigger:
		if (target_collider_kind == ColliderKind::kVisibleTrigger)
		{
			m_is_target_in_sight = true;
		}
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

	case ColliderKind::kHeadTrigger:
		if (target_name == ObjName.BULLET)
		{
			// ダウン中は部位HPは減少させない
			if (action_state_kind != zombie_state::ActionStateKind::kStandStun)
			{
				OnDamage(HealthPartKind::kHead, dynamic_cast<Bullet*>(target_obj)->GetPower());
			}

			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kUpBodyTrigger:
	case ColliderKind::kDownBodyTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HealthPartKind::kBody,		dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kLeftUpperArmTrigger:
	case ColliderKind::kLeftForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HealthPartKind::kLeftArm,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kLeftHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HealthPartKind::kLeftArm,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
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
			OnDamage(HealthPartKind::kRightArm,	dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kRightHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			OnDamage(HealthPartKind::kRightArm, dynamic_cast<Bullet*>(target_obj)->GetPower());
			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
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
			// ダウン中は部位HPは減少させない
			if (   action_state_kind != zombie_state::ActionStateKind::kCrouchLeftStun
				&& action_state_kind != zombie_state::ActionStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kLeftLeg, dynamic_cast<Bullet*>(target_obj)->GetPower());
			}

			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	case ColliderKind::kRightUpLegTrigger:
	case ColliderKind::kRightDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			// ダウン中は部位HPは減少させない
			if (   action_state_kind != zombie_state::ActionStateKind::kCrouchLeftStun
				&& action_state_kind != zombie_state::ActionStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kRightLeg, dynamic_cast<Bullet*>(target_obj)->GetPower());
			}

			OnDamage(HealthPartKind::kMain,		dynamic_cast<Bullet*>(target_obj)->GetPower());
		}
		break;

	default:
		break;
	}
}

void Zombie::OnDamage(const HealthPartKind part_kind, const float damage)
{
	if (!m_health.count(part_kind)) { return; }

	m_health.at(part_kind)->OnDamage(damage);
	m_invincible_timer	= m_invincible_time;
	m_is_invincible		= true;
}

void Zombie::AttachTarget(const std::shared_ptr<CharacterBase> target_character)
{
	m_state->AttachTarget(target_character);
}

void Zombie::DetachTarget()
{
	m_state->DetachTarget();
}

float Zombie::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
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
	// TODO : のちに爆発クラス側に処理内容を委ねる

	// TODO : 座標をモデル依存に変更
	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld) + VGet(0, 50, 0);
	const auto explosion_pos	= sphere->GetPos();
	const auto distance			= pos - explosion_pos;

	if (VSize(distance) > sphere->GetRadius()) { return; }

	const auto dir_xz			= v3d::GetNormalizedV(VGet(distance.x, 0.0f, distance.z));
	const auto dir				= v3d::GetNormalizedV(dir_xz + VGet(0.0f, 0.5f, 0.0f));

	m_knockback_speed			= 400.0f;
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
	distance.y = math::ConnectMinusValueToValue(distance.y, DX_PI_F);

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
