#include "enemy.hpp"

Enemy::Enemy() :
	CharaBase(ObjName.ZOMBIE_POLICE, ObjTag.ENEMY, ModelPath.CHARA_02, MassKind::kMedium),
	m_dir(VGet(0.0f, 0.0f, 1.0f)),
	m_capsule_collider(),
	m_capsule_length()
{
	m_transform->SetRot  (CoordinateKind::kWorld, m_dir);
	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, VGet(0, 0, 0));

	// コライダーを設定
	MakeCapsuleCollider(kCapsuleRadius);

	// トリガーを設定
	MakeLandingTrigger(kLandingTriggerRadius);

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
	ChangeAnimState();
	m_animator->Update();

	CalcCapsuleLength();
}

void Enemy::Draw() const
{
	m_modeler->Draw();

	m_collider->Draw(true, 0, 0xffffff);
	for (auto& trigger : m_trigger)
	{
		trigger.second->Draw(true, 0, 0xffffff);
	}
}

void Enemy::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

void Enemy::OnGravity()
{

}

void Enemy::LoadAnim()
{

}

void Enemy::ChangeAnimState()
{

}
