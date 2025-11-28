#include "rocket_launcher.hpp"

RocketLauncher::RocketLauncher() :
	GunBase					(ObjName.ROCKET_LAUNCHER, GunKind::kRocketLauncher, HolsterKind::kRifle),
	m_exhaust_vent_transform(std::make_shared<Transform>()),
	m_is_draw_magazine		(true)
{
	m_magazine = std::make_shared<NonCollildeRocketBomb>(m_load_transform);
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->AddToObjManager();

	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_LAUNCHER, kBasicAngle, kBasicScale);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,	kHoldOffsetAngle,	kHoldOffsetScale,
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	m_cross_hair_shape		= std::make_shared<Circle>();
	m_scope_scale			= kScopeScale;
	m_range					= kRange;
	m_power					= kPower;
	m_initial_velocity		= kInitialVelocity;
	m_deceleration			= kDeceleration;
	m_shot_interval_time	= kShotIntervalTime;
	m_recoil_data			= RecoilData(0.0f, 0.0f, 0.0f, 10.0f * math::kDegToRad, 0.3f, VGet(1.0f, 1.0f, 1.0f));

	m_max_remaining_bullet_num = kMaxRemainingBulletNum;

	//AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
}

RocketLauncher::~RocketLauncher()
{
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->RemoveToObjManager();
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
}

void RocketLauncher::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//if (m_is_draw_magazine)
	{
		std::dynamic_pointer_cast<ObjBase>(m_magazine)->Draw();
	}
}

void RocketLauncher::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void RocketLauncher::InitCrossHairRange()
{

}

void RocketLauncher::CalcCrossHairRange(const VECTOR& owner_move_velocity)
{
	// ŠgŽU”ÍˆÍ‚ðŽw’è
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	circle->SetRadius(kCrossHairMaxRadius);
}

void RocketLauncher::CalcCrossHairRangeShot()
{

}

void RocketLauncher::CalcCrossHairPos()
{
	// ŠgŽU”ÍˆÍ‚ðŽw’è
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	circle->SetPos(GetFirstShotPos() + m_aim_dir * kCrossHairDistance);
	circle->SetNormalVector(m_aim_dir);
}

void RocketLauncher::CalcTargetPos()
{
	m_target_pos = math::GetRandomPointInCircle(*std::static_pointer_cast<Circle>(m_cross_hair_shape));
}

void RocketLauncher::CalcTransforms()
{
	CalcTransform(m_muzzle_transform,		kMuzzleOffsetPos);
	CalcTransform(m_load_transform,			kLoadPortOffsetPos);
	CalcTransform(m_exhaust_vent_transform, kExhaustVentOffsetPos);
}
