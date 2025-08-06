#include "shell_casing.hpp"
#include "../Base/gun_base.hpp"

ShellCasing::ShellCasing() :
	PhysicalObjBase	(ObjName.SHELL_CASING_556x45, ObjTag.BULLET, MassKind::kLight),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.SHELL_CASING_556x45, kBasicAngle, kBasicScale)),
	m_move_dir		(v3d::GetZeroV()),
	m_alive_timer	(0.0f),
	m_move_speed	(kInitialVelocity)
{	
	SetModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,		  std::make_shared<Sphere>(v3d::GetZeroV(), kColliderRadius),       this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), kLandingTriggerRadius), this));
}

ShellCasing::~ShellCasing()
{

}

void ShellCasing::Init()
{
	m_velocity			= v3d::GetZeroV();
	m_fall_velocity		= v3d::GetZeroV();
	m_alive_timer		= 0.0f;
	m_move_speed		= kInitialVelocity;
}

void ShellCasing::Update()
{
	if (!IsActive()) { return; }
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }

	Move();

	m_alive_timer += FPS::GetDeltaTime();
}

void ShellCasing::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//GetCollider(ColliderKind::kCollider)	  ->GetShape()->Draw(false, 255, 0xffffff);
	//GetCollider(ColliderKind::kLandingTrigger)->GetShape()->Draw(false,   0, 0xff0000);

	//for (auto& collider : m_collider)
	//{
	//	const auto shape = collider->GetShape();
	//	if (shape != nullptr)
	//	{
	//		shape->Draw(true, 0, 0xffffff);
	//	}
	//}
}

void ShellCasing::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kCollider:
		if (hit_collider_pair.target_collider->GetColliderKind() == ColliderKind::kCollider)
		{

		}
		break;

	case ColliderKind::kLandingTrigger:
		// 地形の影響を受けるようにvelocityをdirに保存
		// TODO : 自身との着地判定は避ける。のちに衝突マネージャーで管理
		if (hit_collider_pair.target_collider->GetOwnerObj()->GetName() != ObjName.SHELL_CASING_556x45)
		{
			m_move_dir = v3d::GetNormalizedV(m_velocity);
			m_is_landing = true;
		}
		break;

	default:
		break;
	}
}

void ShellCasing::Eject(GunBase& gun)
{
	m_transform->SetPos(CoordinateKind::kWorld, gun.GetEjectionPortPos());
	m_transform->SetRot(CoordinateKind::kWorld, gun.GetTransform()->GetRotMatrix(CoordinateKind::kWorld));

	//const auto angle = math::GetAngleBetweenTwoVector(gun.GetTransform()->GetUp(CoordinateKind::kWorld), axis::GetWorldYAxis());
	//DrawFormatString(400, 0, 0xffffff, "%f", angle);

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
	math::Decrease(m_move_speed, kDeceleration * FPS::GetDeltaTime(), 0.0f);
	m_velocity = m_move_dir * m_move_speed;
}

void ShellCasing::CalcColliderPos()
{
	const auto pos = m_transform->GetPos(CoordinateKind::kWorld);

	// 押し戻し用コライダー
	auto collider_sphere = std::dynamic_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollider)->GetShape());
	collider_sphere->SetPos(pos);

	// 着地用トリガー
	auto landing_sphere = std::dynamic_pointer_cast<Sphere>(GetCollider(ColliderKind::kLandingTrigger)->GetShape());
	landing_sphere->SetPos(pos + kLandingTriggerOffsetPos);
}
