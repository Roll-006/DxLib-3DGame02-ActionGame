#include "bullet.hpp"

Bullet::Bullet() : 
	PhysicalObjBase(ObjName.BULLET, ObjTag.BULLET, MassKind::kLight)
{

}

Bullet::~Bullet()
{

}

void Bullet::Init()
{

}

void Bullet::Update()
{
	if (!IsActive()) { return; }
}

void Bullet::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Bullet::Draw() const
{
	if (!IsActive()) { return; }
}

void Bullet::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
