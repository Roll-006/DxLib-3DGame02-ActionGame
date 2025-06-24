#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, ModelPath.ASSAULT_RIFLE_01)
{
	SetCorrectValue(kCorrectPos, kCorrectAngle, kCorrectScale);

	m_scope_scale		 = kScopeScale;
	m_range				 = kRange;
	m_muzzle_correct_pos = kMuzzleCorrectPos;
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

void AssaultRifle::LateUpdate()
{
	TrackOwner();
	CalcMuzzlePos();
}

void AssaultRifle::Draw() const
{
	m_modeler->Draw();

	DrawSphere3D(m_muzzle_pos, 2, 8, 0xffffff, 0xffffff, FALSE);
}
