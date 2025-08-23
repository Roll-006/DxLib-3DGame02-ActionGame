#include "zombie.hpp"

Zombie::Zombie() :
	EnemyBase(ObjName.ZOMBIE, MassKind::kMedium)
{
	m_hit_points[HitPointsPartKind::kMain]		= std::make_shared<HitPoints>(1684.0f);
	m_hit_points[HitPointsPartKind::kHead]		= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kBody]		= std::make_shared<HitPoints>(500.0f);
	m_hit_points[HitPointsPartKind::kLeftArm]	= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kRightArm]	= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kLeftLeg]	= std::make_shared<HitPoints>(300.0f);
	m_hit_points[HitPointsPartKind::kRightLeg]	= std::make_shared<HitPoints>(300.0f);

	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ZOMBIE_05, kBasicAngle, kBasicScale);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_look_dir = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot  (CoordinateKind::kWorld, m_look_dir);
	m_transform->SetPos  (CoordinateKind::kWorld, VGet(100, 800, 100));
	m_modeler->ApplyMatrix();

	// コライダー・トリガーを設定
	CreateCharaBasisCollider(kCapsuleRadius, kLandingTriggerRadius);
	CreateHeadTrigger		(kHeadTriggerRadius);
	CreateBodyTrigger		(kBodyTriggerRadius);
	CreateLegTrigger		(kUpLegTriggerRadius, kDownLegTriggerRadius);
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
	ApplyLookDirToRot(m_look_dir);

	//m_animator->Update();

	CalcCapsuleColliderLength();
	CalcLegTriggerPos();
	CalcBodyTriggerPos();
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }
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
		const auto shape = collider->GetShape();
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
