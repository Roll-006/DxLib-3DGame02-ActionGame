#include "shell_casing.hpp"
#include "../Base/gun_base.hpp"

ShellCasing::ShellCasing() :
	PhysicalObjBase	(ObjName.SHELL_CASING, ObjTag.BULLET, MassKind::kLight),
	m_alive_timer	(0.0f),
	m_is_alive		(true),
	m_move_dir		(v3d::GetZeroV())
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.SHELL_CASING_556x45, VGet(90.0f * math::kDegreesToRadian, 0.0f, 0.0f));

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,		  std::shared_ptr<Capsule>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::shared_ptr<Capsule>(), this));
}

ShellCasing::~ShellCasing()
{

}

void ShellCasing::Init()
{
	m_velocity		= v3d::GetZeroV();
	m_fall_velocity = v3d::GetZeroV();
	m_alive_timer	= 0.0f;
	m_is_alive		= true;
}

void ShellCasing::Update()
{
	if (!IsActive()) { return; }

	Move();
	AddFallVelocity();
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }

	JudgeAlive();
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//DrawSphere3D(m_transform->GetPos(CoordinateKind::kWorld), 1, 8, GetColor(0, 0, 255), GetColor(0, 0, 255), TRUE);

	for (auto& collider : m_collider)
	{
		const auto shape = collider->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void ShellCasing::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void ShellCasing::Eject(GunBase& gun)
{
	m_transform->SetPos(CoordinateKind::kWorld, gun.GetEjectionPortPos());
	m_transform->SetRot(CoordinateKind::kWorld, gun.GetTransform()->GetRotMatrix(CoordinateKind::kWorld));

	const auto angle = gun.GetTransform()->GetEulerAngles(CoordinateKind::kWorld).x;
	DrawFormatString(400, 0, 0xffffff, "%f", angle);

	const auto gun_rot = gun.GetTransform()->GetRotMatrix(CoordinateKind::kWorld);
	m_move_dir = VTransform(v3d::GetNormalizedV(kLocalFirstMoveDir), gun_rot);
}

void ShellCasing::Move()
{
	m_velocity = m_move_dir * kMoveSpeed;
}

void ShellCasing::JudgeAlive()
{
	// ¶‘¶ŽžŠÔ‚ð’´‚¦‚½‚çŽ€–S‚µ‚½‚à‚Ì‚Æ‚·‚é
	m_alive_timer += FPS::GetDeltaTime();
	m_is_alive = m_alive_timer > kDisappearTime ? false : true;
}
