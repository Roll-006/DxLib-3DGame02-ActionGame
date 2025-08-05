#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kSniperRifle, HolsterKind::kRifle)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ASSAULT_RIFLE, kBasicAngle, kBasicScale);	
	SetModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,   kHoldOffsetAngle,   kHoldOffsetScale, 
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	m_diffusion_shape			= std::make_shared<Circle>();
	m_scope_scale				= kScopeScale;
	m_range						= kRange;
	m_muzzle_offset_pos			= kMuzzleOffsetPos;
	m_ejection_port_offset_pos	= kEjectionPortOffsetPos;
	m_initial_velocity			= kInitialVelocity;
	m_deceleration				= kDeceleration;
	m_shot_interval_time		= kShotIntervalTime;

	// TODO : âºÇ≈íeêîÇê›íË
	m_max_remaining_bullet_num		= 30;
	m_current_remaining_bullet_num	= m_max_remaining_bullet_num;

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

	CalcShotTimer();
}

void AssaultRifle::LateUpdate()
{
	if (!IsActive()) { return; }

	TrackOwnerHand();
}

void AssaultRifle::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void AssaultRifle::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawSphere3D(GetMuzzlePos(),       2, 8, 0xffffff, 0xffffff, FALSE);
	DrawSphere3D(GetEjectionPortPos(), 1, 8, 0xffffff, 0xffffff, FALSE);

	//DrawFormatString(300,  0, 0xffffff, "écíe     : %d", m_current_remaining_bullet_num);
	//DrawFormatString(300, 20, 0xffffff, "ç≈ëÂécíe : %d", m_max_remaining_bullet_num);

	const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	segment->Draw(false, 0, 0xffffff);
}

void AssaultRifle::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:

		DrawFormatString(0, 0, 0xffffff, "åıê¸ÇÃè’ìÀÇåüèoÇµÇ‹ÇµÇΩÅB");

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
	// ägéUîÕàÕÇéwíË
	m_diffusion_shape = std::make_shared<Circle>(m_aim_dir, kDiffusionRadius);
	std::dynamic_pointer_cast<Circle>(m_diffusion_shape)->SetPos(GetFirstShotPos() + m_aim_dir * kDiffusionDistance);
}

void AssaultRifle::CalcTargetPos()
{
	m_target_pos = math::GetRandomPointInCircle(*std::dynamic_pointer_cast<Circle>(m_diffusion_shape));
}
