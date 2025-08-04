#include "zombie.hpp"

Zombie::Zombie() :
	EnemyBase(ObjName.ZOMBIE_POLICE, ModelPath.ZOMBIE_05, MassKind::kMedium)
{
	m_look_dir = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot  (CoordinateKind::kWorld, m_look_dir);
	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, VGet(100, 0, 100));
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
	UpdateTransform(m_look_dir, kModelScale);

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

void Zombie::Move()
{
	m_velocity = v3d::GetZeroV();
}
