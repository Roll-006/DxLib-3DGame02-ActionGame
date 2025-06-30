#include "shell_casing.hpp"
#include "../Base/gun_base.hpp"

ShellCasing::ShellCasing() :
	PhysicalObjBase	(ObjName.BULLET, ObjTag.BULLET, MassKind::kLight),
	m_alive_timer	(0.0f)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.SHELL_CASING_556x45);

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider,		  std::shared_ptr<Capsule>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::shared_ptr<Capsule>(), this));
}

ShellCasing::~ShellCasing()
{

}

void ShellCasing::Init()
{

}

void ShellCasing::Update()
{
	if (!IsActive()) { return; }

	m_alive_timer += FPS::GetDeltaTime();

	AddFallVelocity();
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

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
}

bool ShellCasing::IsAlive() const
{
	return m_alive_timer > kDisappearTime;
}
