#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, ModelPath.ASSAULT_RIFLE_01)
{
	SetCorrectValue(kCorrectPos, kCorrectAngle, kCorrectScale);

	m_scope_scale				= kScopeScale;
	m_range						= kRange;
	m_muzzle_correct_pos		= kMuzzleCorrectPos;
	m_ejection_port_correct_pos = kEjectionPortCorrectPos;

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
	CalcRayPos();
}

void AssaultRifle::Draw() const
{
	m_modeler->Draw();

	DrawSphere3D(GetMuzzlePos(),       2, 8, 0xffffff, 0xffffff, FALSE);
	DrawSphere3D(GetEjectionPortPos(), 1, 8, 0xffffff, 0xffffff, FALSE);

	const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	segment->Draw(false, 0, 0xffffff);
	DrawFormatString(0, 20, 0xffffff, "%f, %f, %f", segment->GetBeginPos().x, segment->GetBeginPos().y, segment->GetBeginPos().z);
	DrawFormatString(0, 40, 0xffffff, "%f, %f, %f", segment->GetEndPos().x,   segment->GetEndPos().y,   segment->GetEndPos().z);
}

void AssaultRifle::OnCollide(const ColliderPairData& hit_collider_pair)
{
	if (hit_collider_pair.owner_collider->GetColliderKind() == ColliderKind::kRayCast)
	{
		DrawFormatString(0, 0, 0xffffff, "Œõü‚ÌÕ“Ë‚ðŒŸo‚µ‚Ü‚µ‚½B");

		if (hit_collider_pair.target_collider->GetShape() == nullptr)
		{
			if (hit_collider_pair.intersection)
			{
				DrawSphere3D(*hit_collider_pair.intersection, 5, 16, 0xff0000, 0xff0000, TRUE);
			}
		}
	}
}
