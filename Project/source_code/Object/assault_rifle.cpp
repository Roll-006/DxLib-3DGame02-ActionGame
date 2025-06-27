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
	CalcRayPos();
}

void AssaultRifle::Draw() const
{
	m_modeler->Draw();

	DrawSphere3D(GetMuzzlePos(), 2, 8, 0xffffff, 0xffffff, FALSE);

	const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	segment->Draw(false, 0, 0xffffff);
	DrawFormatString(0, 20, 0xffffff, "%f, %f, %f", segment->GetBeginPos().x, segment->GetBeginPos().y, segment->GetBeginPos().z);
	DrawFormatString(0, 40, 0xffffff, "%f, %f, %f", segment->GetEndPos().x,   segment->GetEndPos().y,   segment->GetEndPos().z);
}

void AssaultRifle::OnCollide(const ColliderPairData& hit_collider_pair)
{
	const auto ray = std::dynamic_pointer_cast<Segment>(hit_collider_pair.owner_collider->GetShape());
	const auto target_collider = hit_collider_pair.target_collider;
	const auto target_shape    = target_collider->GetShape();
	std::optional<VECTOR> intersection = std::nullopt;

	if (hit_collider_pair.owner_collider->GetColliderKind() == ColliderKind::kRayCast)
	{
		DrawFormatString(0, 0, 0xffffff, "光線の衝突を検出しました。");

		// TODO : 仮のレイキャスト
		if (target_shape == nullptr)
		{
			const bool is_hit = collision::IsHitSegmentAndModel(*ray, target_collider->GetModelHandle(), intersection);
			DrawSphere3D(*intersection, 5, 16, 0xff0000, 0xff0000, TRUE);
		}
	}
}
