#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, ModelPath.ASSAULT_RIFLE_01)
{
	SetCorrectValue(kCorrectPos, kCorrectAngle, kCorrectScale);

	m_diffusion_shape			= std::make_shared<Circle>();
	m_scope_scale				= kScopeScale;
	m_range						= kRange;
	m_muzzle_correct_pos		= kMuzzleCorrectPos;
	m_ejection_port_correct_pos = kEjectionPortCorrectPos;
	m_initial_velocity			= kInitialVelocity;
	m_deceleration				= kDeceleration;
	m_shot_interval_time		= kShotIntervalTime;

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
	if (!IsActive()) { return; }

	AddFallVelocity();
}

void AssaultRifle::LateUpdate()
{
	if (!IsActive()) { return; }

	TrackOwner();
	CalcDiffusionRange();
	CalcTargetPos();
	Shot();
}

void AssaultRifle::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawSphere3D(GetMuzzlePos(),       2, 8, 0xffffff, 0xffffff, FALSE);
	DrawSphere3D(GetEjectionPortPos(), 1, 8, 0xffffff, 0xffffff, FALSE);

	const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	segment->Draw(false, 0, 0xffffff);
}

void AssaultRifle::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:

		DrawFormatString(0, 0, 0xffffff, "Œõü‚ÌÕ“Ë‚ðŒŸo‚µ‚Ü‚µ‚½B");

		if (hit_collider_pair.target_collider->GetShape() == nullptr)
		{
			if (hit_collider_pair.intersection)
			{
				DrawSphere3D(*hit_collider_pair.intersection, 5, 16, 0xff0000, 0xff0000, TRUE);
			}
		}
		break;

	default:
		break;
	}
}

void AssaultRifle::CalcDiffusionRange()
{
	if (!m_is_aiming) { return; }

	// ŠgŽU”ÍˆÍ‚ðŽw’è
	m_diffusion_shape = std::make_shared<Circle>(m_aim_dir, kDiffusionRadius);
	std::dynamic_pointer_cast<Circle>(m_diffusion_shape)->SetPos(GetFirstShotPos() + m_aim_dir * kDiffusionDistance);
}

void AssaultRifle::CalcTargetPos()
{
	if (!m_is_aiming) { return; }

	m_target_pos = math::GetRandomPointInCircle(*std::dynamic_pointer_cast<Circle>(m_diffusion_shape));
}
