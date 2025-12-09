#include "rocket_launcher.hpp"

RocketLauncher::RocketLauncher() :
	GunBase					(ObjName.ROCKET_LAUNCHER, GunKind::kRocketLauncher, HolsterKind::kRifle),
	m_exhaust_vent_transform(std::make_shared<Transform>()),
	m_is_draw_magazine		(false)
{
	m_magazine = std::make_shared<NonCollildeRocketBomb>(m_load_transform);
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->AddToObjManager();

	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_LAUNCHER, kBasicAngle, kBasicScale);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_cross_hair_shape		= std::make_shared<Circle>();
	scope_scale			= kScopeScale;
	range					= kRange;
	power					= kPower;
	initial_velocity		= kInitialVelocity;
	deceleration			= kDeceleration;
	shot_interval_time	= kShotIntervalTime;
	recoil_data			= RecoilData(0.0f, 0.0f, 0.0f, 10.0f * math::kDegToRad, 0.3f, VGet(1.0f, 1.0f, 1.0f));

	max_remaining_bullet_num = kMaxRemainingBulletNum;

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

	m_is_draw_magazine = m_current_remaining_bullet_num > 0 || m_magazine->IsReloading();
}

void RocketLauncher::LateUpdate()
{
	if (!IsActive()) { return; }
}

void RocketLauncher::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	
	if (m_is_draw_magazine) { std::dynamic_pointer_cast<ObjBase>(m_magazine)->Draw(); }
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

	circle->SetRadius(cross_hair_max_radius);
}

void RocketLauncher::CalcCrossHairRangeShot()
{

}

void RocketLauncher::CalcCrossHairPos()
{
	// ŠgŽU”ÍˆÍ‚ðŽw’è
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	circle->SetPos(GetFirstShotPos() + m_aim_dir * cross_hair_distance);
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
