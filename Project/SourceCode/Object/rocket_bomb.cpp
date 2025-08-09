#include "rocket_bomb.hpp"
#include "../Manager/rifle_cartridge_manager.hpp"
#include "../Base/gun_base.hpp"

RocketBomb::RocketBomb() :
	PhysicalObjBase			(ObjName.ROCKET_BOMB, ObjTag.BULLET, MassKind::kLight),
	m_modeler				(std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_BOMB, kBasicAngle, kBasicScale)),
	m_subject				(std::make_shared<Subject<RocketBomb>>()),
	m_time_scale_owner_name	(""),
	m_move_dir				(v3d::GetZeroV()),
	m_prev_pos				(v3d::GetZeroV()),
	m_first_pos				(v3d::GetZeroV()),
	m_move_speed			(0.0f),
	m_deceleration			(0.0f),
	m_range					(0.0f)
{
	SetModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));

	EffectManager::GetInstance()->AddToSubject<RocketBomb>(m_subject);
}

RocketBomb::~RocketBomb()
{

}

void RocketBomb::Init()
{
	m_time_scale_owner_name = "";
	m_velocity				= v3d::GetZeroV();
	m_fall_velocity			= v3d::GetZeroV();
	m_fall_speed			= 0.0f;
}

void RocketBomb::Update()
{
	if (!IsActive()) { return; }

	// 位置情報をシフト
	m_prev_pos = m_transform->GetPos(CoordinateKind::kWorld);
}

void RocketBomb::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMoveDirToRot();
	Move();
	CalcRayPos();
}

void RocketBomb::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void RocketBomb::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawSphere3D(m_transform->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, TRUE);
	//std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape())->Draw(false, 0, 0xffffff);
}

void RocketBomb::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:
		if (hit_collider_pair.intersection)
		{
			RifleCartridgeManager::GetInstance()->DeleteBullet(shared_from_this());
			RifleCartridgeManager::GetInstance()->AddHitPos(*hit_collider_pair.intersection);

			const Event<OnHitBulletData> event = { EventKind::kOnHitBullet, { GetName(), *hit_collider_pair.intersection, m_move_dir } };
			m_subject->Notify(event);

			EffectManager::GetInstance()->ForciblyReturnPoolEffect(GetObjHandle());
		}
		break;

	default:
		break;
	}
}

void RocketBomb::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}

void RocketBomb::OnShot(GunBase& gun)
{
	m_time_scale_owner_name = gun.GetOwnerName();

	m_transform->SetRot(CoordinateKind::kWorld, MGetIdent());
	m_first_pos		= gun.GetFirstShotPos();
	m_transform->SetPos(CoordinateKind::kWorld, m_first_pos);
	m_prev_pos		= m_first_pos;
	m_move_dir		= gun.GetShotDir();
	m_move_speed	= gun.GetInitialVelocity();
	m_deceleration	= gun.GetDeceleration();
	m_range			= gun.GetRange();

	const Event<OnShotBulletData> event = { EventKind::kOnShotBullet, { GetName(), gun.GetOwnerName(), GetObjHandle(), m_transform}};
	m_subject->Notify(event);
}

bool RocketBomb::IsReturnPool()
{
	float distance = VSize(m_transform->GetPos(CoordinateKind::kWorld) - m_first_pos);

	// 射程範囲を超えた場合は弾丸をプールに返却
	return distance > m_range ? true : false;
}

void RocketBomb::ApplyMoveDirToRot()
{
	if (m_velocity != v3d::GetZeroV())
	{
		const auto forward = v3d::GetNormalizedV(m_velocity);

		if (forward != axis::GetWorldYAxis() && forward != -axis::GetWorldYAxis())
		{
			m_transform->SetRot(CoordinateKind::kWorld, forward);
		}
	}
	else
	{
		m_transform->SetRot(CoordinateKind::kWorld, m_move_dir);
	}
}

void RocketBomb::Move()
{
	const auto time_manager = GameTimeManager::GetInstance();

	const float delta_time = m_time_scale_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScale::LayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScale::LayerKind::kWorld);

	math::Decrease(m_move_speed, m_deceleration * delta_time, 0.0f);
	m_velocity = m_move_dir * m_move_speed;
}

void RocketBomb::CalcRayPos()
{
	// 光線の位置を計算
	auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	ray->SetBeginPos(m_prev_pos, true);
	ray->SetEndPos(m_transform->GetPos(CoordinateKind::kWorld), true);
}
