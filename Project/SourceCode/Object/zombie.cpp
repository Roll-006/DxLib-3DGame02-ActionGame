#include "zombie.hpp"
#include "../Part/zombie_state_controller.hpp"

Zombie::Zombie() :
	CharacterBase	(ObjName.ZOMBIE, ObjTag.ENEMY, MassKind::kMedium),
	m_ai			(std::make_shared<ZombieAI>()),
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
	m_animator = std::make_shared<ZombieAnimator>(m_modeler, m_ai->GetStateController());
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_look_dir = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(434.0f, -83.0f, 655.0f));
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

	Move();

	m_ai->Update(std::static_pointer_cast<Zombie>(shared_from_this()));
	m_animator->Update();

	ApplyLookDirToRot(m_look_dir);

	m_collider_creator->CalcCapsuleColliderLength(this, m_modeler);
	m_collider_creator->CalcHeadTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcBodyTriggerPos(m_modeler, m_collider);
	m_collider_creator->CalcLegTriggerPos (m_modeler, m_collider);
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }

	m_ai->LateUpdate(std::static_pointer_cast<Zombie>(shared_from_this()));
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
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	default:
		break;
	}
}

float Zombie::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleController::LayerKind::kWorld);
}

void Zombie::Move()
{
	m_velocity = v3d::GetZeroV();
}
