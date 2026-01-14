#include "bullet.hpp"
#include "../Manager/rifle_cartridge_manager.hpp"
#include "../Base/gun_base.hpp"

Bullet::Bullet() :
	PhysicalObjBase		(ObjName.BULLET, ObjTag.BULLET),
	m_shot_owner_name	(""),
	m_move_dir			(v3d::GetZeroV()),
	m_prev_pos			(v3d::GetZeroV()),
	m_first_pos			(v3d::GetZeroV()),
	m_move_speed		(0.0f),
	m_deceleration		(0.0f),
	m_range				(0.0f),
	m_power				(0.0f)
{
	mass_kind = MassKind::kLight;

	AddCollider(std::make_shared<Collider>(ColliderKind::kRay,					std::make_shared<Segment>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), kCollisionAreaRadius), this));
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
	m_shot_owner_name		= "";
	m_move_velocity			= v3d::GetZeroV();
	m_fall_velocity			= v3d::GetZeroV();
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
	CalcRayCastPos();
}

void Bullet::Draw() const
{
	if (!IsActive()) { return; }

	//DrawColliders();
}

void Bullet::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase* target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto		 target_name			= target_obj->GetName();
	const auto		 target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRay:
		if (hit_collider_pair.intersection)
		{
			RifleCartridgeManager::GetInstance()->DeleteBullet(shared_from_this());

			const OnHitBulletEvent event{ GetName(), *hit_collider_pair.intersection, m_move_dir };
			EventSystem::GetInstance()->Publish(event);

		}
		break;

	default:
		break;
	}
}

void Bullet::OnProjectPos()
{

}

void Bullet::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjAccessor		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}

void Bullet::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjAccessor		::GetInstance()->RemoveObj		  (obj_handle);
}

void Bullet::OnShot(GunBase& gun)
{
	m_shot_owner_name	= gun.GetOwnerName();

	m_first_pos			= gun.GetFirstShotPos();
	m_transform->SetPos(CoordinateKind::kWorld, m_first_pos);
	m_prev_pos			= m_first_pos;
	m_move_dir			= gun.GetShotDir();
	m_move_speed		= gun.GetInitialVelocity();
	m_deceleration		= gun.GetDeceleration();
	m_range				= gun.GetRange();
	m_power				= gun.GetPower();

	CalcRayCastPos();

	// 衝突判定を許可するエリアに利用するトリガー
	auto collision_area_sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollisionAreaTrigger)->GetShape());
	collision_area_sphere->SetPos(m_first_pos);

	const OnShotBulletEvent event{ GetName(), gun.GetOwnerName(), GetObjHandle(), m_transform };
	EventSystem::GetInstance()->Publish(event);
}

const float Bullet::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_shot_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

const bool Bullet::IsReturnPool()
{
	float distance = VSize(m_transform->GetPos(CoordinateKind::kWorld) - m_first_pos);

	// 射程範囲を超えた場合は弾丸をプールに返却
	return distance > m_range;
}

void Bullet::Move()
{
	math::Decrease(m_move_speed, m_deceleration, 0.0f);
	m_move_velocity = m_move_dir * m_move_speed;
	m_velocity += m_move_velocity;
}

void Bullet::CalcRayCastPos()
{
	// 光線の位置を計算
	auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRay)->GetShape());
	ray->SetBeginPos(m_prev_pos, true);
	ray->SetEndPos	(m_transform->GetPos(CoordinateKind::kWorld), true);
}
