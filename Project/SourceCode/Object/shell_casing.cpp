#include "shell_casing.hpp"
#include "../Base/gun_base.hpp"

ShellCasing::ShellCasing() :
	PhysicalObjBase	(ObjName.SHELL_CASING, ObjTag.BULLET, MassKind::kLight),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.SHELL_CASING_556x45, VGet(90.0f * math::kDegreesToRadian, 0.0f, 0.0f))),
	m_move_dir		(v3d::GetZeroV()),
	m_alive_timer	(0.0f),
	m_move_speed	(kInitialVelocity),
	m_is_alive		(true)
{
	SetModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,		  std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), kCapsuleRadius),        this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), kLandingTriggerRadius), this));
}

ShellCasing::~ShellCasing()
{

}

void ShellCasing::Init()
{
	m_velocity		= v3d::GetZeroV();
	m_fall_velocity = v3d::GetZeroV();
	m_alive_timer	= 0.0f;
	m_move_speed	= kInitialVelocity;
	m_is_alive		= true;
}

void ShellCasing::Update()
{
	if (!IsActive()) { return; }

	AddFallVelocity();

	m_is_landing = false;
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }

	Move();
	JudgeAlive();
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//GetCollider(ColliderKind::kCollider)	  ->GetShape()->Draw(true, 255, 0xffffff);
	//GetCollider(ColliderKind::kLandingTrigger)->GetShape()->Draw(true,   0, 0xff0000);

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
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
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

void ShellCasing::Move()
{
	math::Decrease(m_move_speed, kDeceleration * FPS::GetDeltaTime(), 0.0f);
	m_velocity = m_move_dir * m_move_speed;
}

void ShellCasing::JudgeAlive()
{
	// 生存時間を超えたら死亡したものとする
	m_alive_timer += FPS::GetDeltaTime();
	m_is_alive = m_alive_timer > kDisappearTime ? false : true;
}

void ShellCasing::CalcColliderPos()
{
	// 位置設定
	const auto center_pos = m_transform->GetPos(CoordinateKind::kWorld);
	const auto begin_pos  = center_pos - m_transform->GetForward(CoordinateKind::kWorld) * kCapsuleLength * 0.5f;
	const auto end_pos    = center_pos + m_transform->GetForward(CoordinateKind::kWorld) * kCapsuleLength * 0.5f;

	// 押し戻し用コライダー
	auto collider_capsule = std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kCollider)->GetShape());
	collider_capsule->SetSegmentBeginPos(begin_pos, true);
	collider_capsule->SetSegmentEndPos  (end_pos,   true);

	// 着地用トリガー
	auto landing_capsule = std::dynamic_pointer_cast<Capsule>(GetCollider(ColliderKind::kLandingTrigger)->GetShape());
	landing_capsule->SetSegmentBeginPos(begin_pos + kLandingTriggerCorrectPos, true);
	landing_capsule->SetSegmentEndPos  (end_pos   + kLandingTriggerCorrectPos, true);
}
