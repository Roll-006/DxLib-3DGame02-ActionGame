#include "rocket_launcher.hpp"

RocketLauncher::RocketLauncher() :
	GunBase					(ObjName.ROCKET_LAUNCHER, GunKind::kRocketLauncher, HolsterKind::kRifle),
	m_exhaust_vent_transform(std::make_shared<Transform>()),
	m_is_draw_magazine		(false)
{
	m_magazine = std::make_shared<NonCollildeRocketBomb>(m_load_transform);
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->AddToObjManager();

	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/gun_data.json", j_data))
	{
		const auto rocket_launcher	= j_data.at("gun_data").at("rocket_launcher");
		mass_kind					= rocket_launcher.at("mass_kind");
		basic_angle					= rocket_launcher.at("basic_angle").get<VECTOR>() * math::kDegToRad;
		basic_scale					= rocket_launcher.at("basic_scale");
		power						= rocket_launcher.at("power");
		hold_offset_pos				= rocket_launcher.at("hold_offset_pos");
		hold_offset_angle			= rocket_launcher.at("hold_offset_angle").get<VECTOR>() * math::kDegToRad;
		hold_offset_scale			= rocket_launcher.at("hold_offset_scale");
		attach_offset_pos			= rocket_launcher.at("attach_offset_pos");
		attach_offset_angle			= rocket_launcher.at("attach_offset_angle").get<VECTOR>() * math::kDegToRad;
		attach_offset_scale			= rocket_launcher.at("attach_offset_scale");
		weapon_kind					= rocket_launcher.at("weapon_kind");
		holster_kind				= rocket_launcher.at("holster_kind");
		cross_hair_distance			= rocket_launcher.at("cross_hair_distance");
		max_remaining_bullet_num	= rocket_launcher.at("max_remaining_bullet_num");
		range						= rocket_launcher.at("range");
		initial_velocity			= rocket_launcher.at("initial_velocity");
		deceleration				= rocket_launcher.at("deceleration");
		scope_scale					= rocket_launcher.at("scope_scale");
		recoil_data					= rocket_launcher.at("recoil_data").get<RecoilData>();
		recoil_data.yaw		*= math::kDegToRad;
		recoil_data.pitch	*= math::kDegToRad;
		shot_interval_time			= rocket_launcher.at("shot_interval_time");
		gun_kind					= rocket_launcher.at("gun_kind");
		cross_hair_max_radius		= rocket_launcher.at("cross_hair_max_radius");
		muzzle_offset				= rocket_launcher.at("muzzle_offset");
		exhaust_vent_offset			= rocket_launcher.at("exhaust_vent_offset");
		load_port_offset			= rocket_launcher.at("load_port_offset");

		m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_LAUNCHER, basic_angle, basic_scale);
		SetColliderModelHandle(m_modeler->GetModelHandle());
	}

	m_cross_hair_shape = std::make_shared<Circle>();
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
	CalcTransform(m_muzzle_transform,		muzzle_offset);
	CalcTransform(m_load_transform,			load_port_offset);
	CalcTransform(m_exhaust_vent_transform, exhaust_vent_offset);
}
