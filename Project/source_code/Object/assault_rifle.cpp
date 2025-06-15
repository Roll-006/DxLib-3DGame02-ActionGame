#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, ModelPath.ASSAULT_RIFLE_01)
{
	SetCorrectValue(kCorrectPos, kCorrectAngle, kCorrectScale);
	SetScopeScale(kScopeScale);
}

AssaultRifle::~AssaultRifle()
{
}

void AssaultRifle::Init()
{

}

void AssaultRifle::Update()
{
	
}

void AssaultRifle::Draw() const
{
	m_modeler->Draw();
}
