#include "shell_casing.hpp"
#include "../Base/gun_base.hpp"

ShellCasing::ShellCasing(const std::string& file_path) :
	PhysicalObjBase			(ObjName.SHELL_CASING_556x45, ObjTag.SHELL_CASING),
	m_modeler				(std::make_shared<Modeler>(m_transform, file_path, kBasicAngle, kBasicScale)),
	m_time_scale_owner_name	(""),
	m_move_dir				(v3d::GetZeroV()),
	m_alive_timer			(0.0f),
	m_move_speed			(kInitialVelocity),
	m_prev_on_ground		(false)
{	
	mass_kind = MassKind::kLight;

	SetColliderModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kProjectRay,			std::make_shared<Segment>(v3d::GetZeroV(), -axis::GetWorldYAxis(), kRayLength), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,				std::make_shared<Sphere>(v3d::GetZeroV(), kColliderRadius),						this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger,		std::make_shared<Sphere>(v3d::GetZeroV(), kLandingTriggerRadius),				this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), kCollisionAreaRadius),				this));

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

	if (m_is_on_ground && !m_prev_on_ground)
	{
		const auto time_scale = m_time_scale_owner_name == ObjName.PLAYER ? TimeScaleLayerKind::kPlayer : TimeScaleLayerKind::kWorld;
		EventSystem::GetInstance()->Publish(DropShellCasing(m_transform->GetPos(CoordinateKind::kWorld), time_scale));
	}

	m_prev_on_ground = m_is_on_ground;
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }

	Move();
	CalcProjectRayPos();
	CalcColliderPos();

	m_alive_timer += GetDeltaTime();
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//DrawColliders();
}

void ShellCasing::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kProjectRay:
		if (hit_collider_pair.intersection) { m_current_project_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kLandingTrigger:
		if (hit_collider_pair.target_collider->GetOwnerObj()->GetName() != ObjName.SHELL_CASING_556x45)
		{
			m_move_dir		= v3d::GetNormalizedV(m_move_velocity);
			m_is_on_ground	= true;
		}
		break;

	default:
		break;
	}
}

void ShellCasing::OnProjectPos()
{
	if (!IsActive()) { return; }
	if (IsOnGround()) { return; }

	const auto project_pos = GetCurrentProjectPos();
	if (!project_pos) { return; }

	const auto hit_triangle = GetCollider(ColliderKind::kProjectRay)->GetHitTriangles();
	if (hit_triangle.size() <= 0) { return; }

	// 角度・位置を固定
	const auto current_axis = m_transform->GetAxis(CoordinateKind::kWorld);
	const auto new_axis		= math::GetUpSyncedAxis(current_axis, hit_triangle.front().GetNormalVector(), std::make_optional<AxisData>(AxisData(current_axis.z_axis, AxisKind::kForward)));

	m_transform->SetPos(CoordinateKind::kWorld, *project_pos);
	m_transform->SetRot(CoordinateKind::kWorld, new_axis);

	// コライダーの位置更新
	const auto pos = m_transform->GetPos(CoordinateKind::kWorld);
	CalcColliderPos();
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

void ShellCasing::CalcProjectRayPos()
{
	const auto project_ray = GetCollider(ColliderKind::kProjectRay);
	if (project_ray)
	{
		const auto segment = std::static_pointer_cast<Segment>(project_ray->GetShape());
		segment->SetBeginPos(m_transform->GetPos(CoordinateKind::kWorld), false);
	}
}

float ShellCasing::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_time_scale_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
