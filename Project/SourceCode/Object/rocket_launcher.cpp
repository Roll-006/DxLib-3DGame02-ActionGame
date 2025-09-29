#include "rocket_launcher.hpp"

RocketLauncher::RocketLauncher() :
	GunBase					(ObjName.ROCKET_LAUNCHER, GunKind::kRocketLauncher, HolsterKind::kRifle),
	m_exhaust_vent_transform(std::make_shared<Transform>())
{
	m_magazine = std::make_shared<NonCollildeRocketBomb>(m_load_transform);
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->AddToObjManager();

	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_LAUNCHER, kBasicAngle, kBasicScale);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,	kHoldOffsetAngle,	kHoldOffsetScale,
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	m_diffusion_shape		= std::make_shared<Circle>();
	m_scope_scale			= kScopeScale;
	m_range					= kRange;
	m_power					= kPower;
	m_initial_velocity		= kInitialVelocity;
	m_deceleration			= kDeceleration;
	m_shot_interval_time	= kShotIntervalTime;
	m_recoil_data			= RecoilData(0.0f, 0.0f, 0.0f, 10.0f * math::kDegToRad, 0.3f, VGet(1.0f, 1.0f, 1.0f));

	m_max_remaining_bullet_num		= kMaxRemainingBulletNum;
	m_current_remaining_bullet_num	= m_max_remaining_bullet_num;

	//AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
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

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Update();
}

void RocketLauncher::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcTransform(m_muzzle_transform,		kMuzzleOffsetPos);
	CalcTransform(m_load_transform,			kLoadPortOffsetPos);
	CalcTransform(m_exhaust_vent_transform, kExhaustVentOffsetPos);

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->LateUpdate();
}

void RocketLauncher::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->DrawToShadowMap();
}

void RocketLauncher::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Draw();

	if(GetMuzzleTransform())DrawSphere3D(GetMuzzleTransform()->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, FALSE);
	if(GetLoadTransform())  DrawSphere3D(GetLoadTransform()  ->GetPos(CoordinateKind::kWorld), 1, 8, 0xffffff, 0xffffff, FALSE);
	DrawSphere3D(m_exhaust_vent_transform->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, FALSE);
	//DrawSphere3D(GetEjectionPortPos(), 1, 8, 0xffffff, 0xffffff, FALSE);

	//DrawFormatString(300,  0, 0xffffff, "écíe     : %d", m_current_remaining_bullet_num);
	//DrawFormatString(300, 20, 0xffffff, "ç≈ëÂécíe : %d", m_max_remaining_bullet_num);

	//const auto segment = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	//segment->Draw(false, 0, 0xffffff);
}

void RocketLauncher::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

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
