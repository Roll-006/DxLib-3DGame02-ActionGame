#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kSniperRifle, HolsterKind::kRifle)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ASSAULT_RIFLE, kBasicAngle, kBasicScale);	
	SetColliderModelHandle(m_modeler->GetModelHandle());

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
	m_recoil_data				= RecoilData(1.0f, -0.3f, 0.5f * math::kDegToRad, 0.7f * math::kDegToRad, 0.1f, VGet(0.5f, 0.5f, 0.5f));

	// TODO : âºÇ≈íeêîÇê›íË
	m_max_remaining_bullet_num		= 30;
	m_current_remaining_bullet_num	= m_max_remaining_bullet_num;

	//AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
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
	CalcMuzzleTransform();
	CalcEjectionPortTransform();
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

	DrawSphere3D(GetMuzzleTransform()->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, FALSE);
	DrawSphere3D(GetEjectionPortTransform()->GetPos(CoordinateKind::kWorld), 1, 8, 0xffffff, 0xffffff, FALSE);

	//DrawFormatString(300,  0, 0xffffff, "écíe     : %d", m_current_remaining_bullet_num);
	//DrawFormatString(300, 20, 0xffffff, "ç≈ëÂécíe : %d", m_max_remaining_bullet_num);

	//const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	//segment->Draw(false, 0, 0xffffff);
}

void AssaultRifle::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

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
