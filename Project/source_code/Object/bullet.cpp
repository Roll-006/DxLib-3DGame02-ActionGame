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

}

void Bullet::LateUpdate()
{

}

void Bullet::Draw() const
{

}

void Bullet::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
