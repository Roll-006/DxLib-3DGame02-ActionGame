#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, ModelPath.ASSAULT_RIFLE_01)
{
	SetCorrectValue(kCorrectPos, kCorrectAngle, kCorrectScale);

	m_scope_scale		 = kScopeScale;
	m_range				 = kRange;
	m_muzzle_correct_pos = kMuzzleCorrectPos;

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
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
	CalcRayPos();
}

void AssaultRifle::Draw() const
{
	m_modeler->Draw();

	DrawSphere3D(m_muzzle_pos, 2, 8, 0xffffff, 0xffffff, FALSE);

	const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	segment->Draw(false, 0, 0xffffff);
}

void AssaultRifle::OnCollide(const ColliderPairData& hit_collider_pair)
{

}
