#include "shell_casing.hpp"

ShellCasing::ShellCasing() :
	PhysicalObjBase(ObjName.BULLET, ObjTag.BULLET, MassKind::kLight)
{

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

	AddFallVelocity();
}

void ShellCasing::LateUpdate()
{
	if (!IsActive()) { return; }
}

void ShellCasing::Draw() const
{
	if (!IsActive()) { return; }
}

void ShellCasing::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
