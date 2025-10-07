#include "shell_casing.hpp"
#include "../Base/gun_base.hpp"

ShellCasing::ShellCasing(const std::string& file_path) :
	PhysicalObjBase			(ObjName.SHELL_CASING_556x45, ObjTag.SHELL_CASING, MassKind::kLight),
	m_modeler				(std::make_shared<Modeler>(m_transform, file_path, kBasicAngle, kBasicScale)),
	m_time_scale_owner_name	(""),
	m_move_dir				(v3d::GetZeroV()),
	m_alive_timer			(0.0f),
	m_move_speed			(kInitialVelocity)
{	
	SetColliderModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,		  std::make_shared<Sphere>(v3d::GetZeroV(), kColliderRadius),		this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), kLandingTriggerRadius), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), kCollisionAreaRadius), this));

}

ShellCasing::~ShellCasing()
{

}

void ShellCasing::Init()
{
	m_time_scale_owner_name = "";
	m_move_velocity			= v3d::GetZeroV();
	m_fall_velocity			= v3d::GetZeroV();
	m_alive_timer			= 0.0f;
	m_move_speed			= kInitialVelocity;
}

void ShellCasing::Update()
{
	if (!IsActive()) { return; }
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }

	Move();

	m_alive_timer += GetDeltaTime();
}

void ShellCasing::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//GetCollider(ColliderKind::kCollider)	  ->GetShape()->Draw(false, 255, 0xffffff);
	//GetCollider(ColliderKind::kLandingTrigger)->GetShape()->Draw(false,   0, 0xff0000);

	for (auto& collider : m_colliders)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void ShellCasing::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		// 地形の影響を受けるようにvelocityをdirに保存
		// TODO : 自身との着地判定は避ける。のちに衝突マネージャーで管理
		if (hit_collider_pair.target_collider->GetOwnerObj()->GetName() != ObjName.SHELL_CASING_556x45)
		{
			m_move_dir = v3d::GetNormalizedV(m_move_velocity);
			m_is_landing = true;
		}
		break;

	default:
		break;
	}
}

void ShellCasing::AddToObjManager()
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}

void ShellCasing::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

void ShellCasing::Eject(GunBase& gun)
{
	m_time_scale_owner_name = gun.GetOwnerName();

	m_transform->SetPos(CoordinateKind::kWorld, gun.GetEjectionPortTransform()->GetPos(CoordinateKind::kWorld));
	m_transform->SetRot(CoordinateKind::kWorld, gun.GetTransform()->GetRotMatrix(CoordinateKind::kWorld));

	// 銃を基準に移動方向を設定
	const auto gun_rot = gun.GetTransform()->GetRotMatrix(CoordinateKind::kWorld);
	m_move_dir = VTransform(v3d::GetNormalizedV(kLocalFirstMoveDir), gun_rot);

	// コライダーを設定
	CalcColliderPos();
}

bool ShellCasing::IsReturnPool()
{
	return m_alive_timer > kDisappearTime ? true : false;
}

void ShellCasing::Move()
{
	math::Decrease(m_move_speed, kDeceleration, 0.0f);

	m_move_velocity = m_move_dir * m_move_speed;
	m_velocity += m_move_velocity;
}

void ShellCasing::CalcColliderPos()
{
	const auto pos = m_transform->GetPos(CoordinateKind::kWorld);

	// 押し戻し用コライダー
	auto collider_sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollider)->GetShape());
	collider_sphere->SetPos(pos);

	// 衝突判定を許可するエリアに利用するトリガー
	auto collision_area_sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollisionAreaTrigger)->GetShape());
	collision_area_sphere->SetPos(pos);

	// 着地用トリガー
	auto landing_sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kLandingTrigger)->GetShape());
	landing_sphere->SetPos(pos + kLandingTriggerOffsetPos);
}

float ShellCasing::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_time_scale_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
