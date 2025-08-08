#include "bullet.hpp"
#include "../Manager/rifle_cartridge_manager.hpp"
#include "../Base/gun_base.hpp"

Bullet::Bullet() :
	PhysicalObjBase	(ObjName.BULLET, ObjTag.BULLET, MassKind::kLight),
	m_subject		(std::make_shared<Subject<Bullet>>()),
	m_move_dir		(v3d::GetZeroV()),
	m_prev_pos		(v3d::GetZeroV()),
	m_first_pos		(v3d::GetZeroV()),
	m_move_speed	(0.0f),
	m_deceleration	(0.0f),
	m_range			(0.0f)
{
	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));

	EffectManager::GetInstance()->AddToSubject<Bullet>(m_subject);
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
	m_velocity		= v3d::GetZeroV();
	m_fall_velocity = v3d::GetZeroV();
	m_fall_speed	= 0.0f;
}

void Bullet::Update()
{
	if (!IsActive()) { return; }

	// 位置情報をシフト
	m_prev_pos = m_transform->GetPos(CoordinateKind::kWorld);
}

void Bullet::LateUpdate()
{
	if (!IsActive()) { return; }

	Move();
	CalcRayPos();
}

void Bullet::DrawToShadowMap() const
{
	// 処理なし
}

void Bullet::Draw() const
{
	if (!IsActive()) { return; }

	DrawSphere3D(m_transform->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, TRUE);
	std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape())->Draw(false, 0, 0xffffff);
}

void Bullet::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:
		if (hit_collider_pair.intersection)
		{
			RifleCartridgeManager::GetInstance()->DeleteBullet(this->GetObjHandle());
			RifleCartridgeManager::GetInstance()->AddHitPos(*hit_collider_pair.intersection);
		}
		break;

	default:
		break;
	}
}

void Bullet::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void Bullet::OnShot(GunBase& gun)
{
	m_first_pos		= gun.GetFirstShotPos();
	m_transform->SetPos(CoordinateKind::kWorld, m_first_pos);
	m_prev_pos		= m_first_pos;
	m_move_dir		= gun.GetShotDir();
	m_move_speed	= gun.GetInitialVelocity();
	m_deceleration	= gun.GetDeceleration();
	m_range			= gun.GetRange();

	const Event<OnShotBulletData> event = { EventKind::kOnShotBullet, { GetName(), m_transform} };
	m_subject->Notify(event);
}

bool Bullet::IsReturnPool()
{
	float distance = VSize(m_transform->GetPos(CoordinateKind::kWorld) - m_first_pos);

	// 射程範囲を超えた場合は弾丸をプールに返却
	return distance > m_range ? true : false;
}

void Bullet::Move()
{
	math::Decrease(m_move_speed, m_deceleration * FPS::GetDeltaTime(), 0.0f);
	m_velocity = m_move_dir * m_move_speed;
}

void Bullet::CalcRayPos()
{
	// 光線の位置を計算
	auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	ray->SetBeginPos(m_prev_pos, true);
	ray->SetEndPos(m_transform->GetPos(CoordinateKind::kWorld), true);
}
