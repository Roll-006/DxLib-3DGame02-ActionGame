#include "rocket_launcher.hpp"

RocketLauncher::RocketLauncher() :
	GunBase					(ObjName.ROCKET_LAUNCHER, GunKind::kRocketLauncher, HolsterKind::kRifle),
	m_exhaust_vent_transform(std::make_shared<Transform>())
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_LAUNCHER, kBasicAngle, kBasicScale);
	SetModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,	kHoldOffsetAngle,	kHoldOffsetScale,
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	m_diffusion_shape		= std::make_shared<Circle>();
	m_scope_scale			= kScopeScale;
	m_range					= kRange;
	m_muzzle_offset_pos		= kMuzzleOffsetPos;
	m_initial_velocity		= kInitialVelocity;
	m_deceleration			= kDeceleration;
	m_shot_interval_time	= kShotIntervalTime;

	// TODO : âºÇ≈íeêîÇê›íË
	m_max_remaining_bullet_num		= 30;
	m_current_remaining_bullet_num	= m_max_remaining_bullet_num;

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
}

RocketLauncher::~RocketLauncher()
{

}

void RocketLauncher::Init()
{

}

void RocketLauncher::Update()
{
	if (!IsActive()) { return; }

	CalcShotTimer();
}

void RocketLauncher::LateUpdate()
{
	if (!IsActive()) { return; }

	TrackOwnerHand();
	CalcMuzzleTransform();
	CalcExhaustVentTransform();
}

void RocketLauncher::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void RocketLauncher::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	if(GetMuzzleTransform())DrawSphere3D(GetMuzzleTransform()->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, FALSE);
	DrawSphere3D(m_exhaust_vent_transform->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, FALSE);
	//DrawSphere3D(GetEjectionPortPos(), 1, 8, 0xffffff, 0xffffff, FALSE);

	//DrawFormatString(300,  0, 0xffffff, "écíe     : %d", m_current_remaining_bullet_num);
	//DrawFormatString(300, 20, 0xffffff, "ç≈ëÂécíe : %d", m_max_remaining_bullet_num);

	const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	segment->Draw(false, 0, 0xffffff);
}

void RocketLauncher::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
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

void RocketLauncher::CalcDiffusionRange()
{
	// ägéUîÕàÕÇéwíË
	m_diffusion_shape = std::make_shared<Circle>(m_aim_dir, kDiffusionRadius);
	std::dynamic_pointer_cast<Circle>(m_diffusion_shape)->SetPos(GetFirstShotPos() + m_aim_dir * kDiffusionDistance);
}

void RocketLauncher::CalcTargetPos()
{
	m_target_pos = math::GetRandomPointInCircle(*std::dynamic_pointer_cast<Circle>(m_diffusion_shape));
}

void RocketLauncher::CalcExhaustVentTransform()
{
	m_exhaust_vent_transform->SetMatrix(CoordinateKind::kWorld, m_transform->GetMatrix(CoordinateKind::kWorld));

	const auto world_m		= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos	= m_transform->GetPos	(CoordinateKind::kLocal);

	m_exhaust_vent_transform->SetPos(CoordinateKind::kWorld, local_pos + VTransformSR(kExhaustVentOffsetPos, world_m));
}
