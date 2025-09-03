#include "zombie.hpp"
#include "../Part/zombie_state_controller.hpp"

Zombie::Zombie() :
	CharacterBase	(ObjName.ZOMBIE, ObjTag.ENEMY, MassKind::kMedium),
	m_state			(std::make_shared<ZombieStateController>()),
	m_move_dir		(v3d::GetZeroV()),
	m_look_dir		(v3d::GetZeroV())
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

	m_look_dir = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(140.0f, -73.0f, 96.0f));
	m_modeler->ApplyMatrix();

	// コライダー・トリガーを設定
	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, kCapsuleRadius);
	m_collider_creator->CreateLandingTrigger	(this, kLandingTriggerRadius);
	m_collider_creator->CreateHeadTrigger		(this, m_modeler, kHeadTriggerRadius);
	m_collider_creator->CreateBodyTrigger		(this, m_modeler, kBodyTriggerRadius);
	m_collider_creator->CreateLegTrigger		(this, m_modeler, kUpLegTriggerRadius, kDownLegTriggerRadius);
	m_collider_creator->CreateMeshTrigger		(this, m_modeler);
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

	CalcLookDir();
	CalcMoveVelocity();

	m_collider_creator->CalcCapsuleColliderLength(this, m_modeler);
	m_collider_creator->CalcHeadTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcBodyTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcLegTriggerPos (m_modeler, m_collider);

	ApplyLookDirToRot(m_look_dir);
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }

	m_state->LateUpdate(std::static_pointer_cast<Zombie>(shared_from_this()));
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
}

void Zombie::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kCollider:
		// ロケット弾着弾時の爆発エフェクトとの衝突
		if (hit_collider_pair.target_collider->GetOwnerObj()->GetName() == ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT)
		{
			if (m_is_invincible) { return; }
			
			OnCollideWithExpolsion(std::static_pointer_cast<Sphere>(hit_collider_pair.target_collider->GetShape()));
			OnDamage();
		}
		break;

	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	default:
		break;
	}
}

bool Zombie::IsTargetInSight(const VECTOR& target_pos)
{
	// 頭部を基準に視界を作り出す
	auto	   head_mat		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos		= MGetTranslateElem(head_mat);
	const auto head_axes	= math::ConvertRotMatrixToAxes(head_mat);

	const auto fov			= 60.0f * math::kDegToRad;
	const auto max_distance = 200.0f;
	const auto distance_v	= target_pos - head_pos;
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

}

void Zombie::TrackMove(const VECTOR& pos)
{
	// TODO : 仮で速度を設定。のちに削除
	m_move_dir = v3d::GetNormalizedV(pos - m_transform->GetPos(CoordinateKind::kWorld));
}

void Zombie::CalcMoveSpeed()
{
	m_move_speed = kWalkSpeed;
}

void Zombie::CalcMoveSpeedRun()
{
	m_move_speed = kRunSpeed;
}

void Zombie::OnCollideWithExpolsion(const std::shared_ptr<Sphere> sphere)
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
	m_look_dir = m_move_dir;
}

void Zombie::CalcMoveVelocity()
{
	m_move_velocity = m_move_dir * m_move_speed;
	m_velocity += m_move_velocity;
}
