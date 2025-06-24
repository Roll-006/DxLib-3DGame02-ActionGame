#include "enemy.hpp"

Enemy::Enemy() :
	CharaBase(ObjName.ZOMBIE_POLICE, ObjTag.ENEMY, ModelPath.CHARA_02, MassKind::kMedium),
	m_dir(VGet(0.0f, 0.0f, 1.0f))
{
	m_transform->SetRot  (CoordinateKind::kWorld, m_dir);
	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, VGet(20, 0, 0));

	// コライダー・トリガーを設定
	MakeCapsuleCollider(kCapsuleRadius);
	MakeLandingTrigger (kLandingTriggerRadius);

	// 各アニメーション追加
	LoadAnim();
}

Enemy::~Enemy()
{

}

void Enemy::Init()
{

}

void Enemy::Update()
{
	UpdateTransform();

	ChangeAnimState();
	m_animator->Update();

	CalcCapsuleLength();

	m_is_landing = false;
}

void Enemy::LateUpdate()
{

}

void Enemy::Draw() const
{
	m_modeler->Draw();

	for (auto& trigger : m_collider)
	{
		trigger->GetShape()->Draw(true, 0, 0xffffff);
	}

	auto pos  = m_transform->GetPos (CoordinateKind::kWorld);
	auto axes = m_transform->GetAxes(CoordinateKind::kWorld);
	DrawLine3D(pos, pos + axes.x_axis * 100, 0xff0000);
	DrawLine3D(pos, pos + axes.y_axis * 100, 0x00ff22);
	DrawLine3D(pos, pos + axes.z_axis * 100, 0x0077ff);

	DrawFormatString(0,  0, 0xffffff, "%f, %f, %f", m_velocity.x, m_velocity.y, m_velocity.z);
	DrawFormatString(0, 20, 0xffffff, "%f, %f, %f", m_fall_velocity.x, m_fall_velocity.y, m_fall_velocity.z);
}

void Enemy::OnCollide(const ColliderPairData& hit_collider_pair)
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

void Enemy::LoadAnim()
{

}

void Enemy::ChangeAnimState()
{

}

void Enemy::Move()
{
	m_velocity = v3d::GetZeroV();
}

void Enemy::UpdateTransform()
{
	m_transform->SetRot  (CoordinateKind::kWorld, m_dir);
	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
}
