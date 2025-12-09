#include "assault_rifle.hpp"

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kAssaultRifle, HolsterKind::kRifle)
{
	m_magazine = std::make_shared<AssaultRifleMagazine>(m_load_transform);
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->AddToObjManager();

	nlohmann::json data;
	if (json_loader::Load("Data/JSON/gun_data.json", data))
	{
		const auto assault_rifle = data.at("gun_data").at("assault_rifle");
		mass_kind					= assault_rifle.at("mass_kind");
		basic_angle					= assault_rifle.at("basic_angle").get<VECTOR>() * math::kDegToRad;
		basic_scale					= assault_rifle.at("basic_scale");
		power						= assault_rifle.at("power");
		hold_offset_pos				= assault_rifle.at("hold_offset_pos");
		hold_offset_angle			= assault_rifle.at("hold_offset_angle").get<VECTOR>() * math::kDegToRad;
		hold_offset_scale			= assault_rifle.at("hold_offset_scale");
		attach_offset_pos			= assault_rifle.at("attach_offset_pos");
		attach_offset_angle			= assault_rifle.at("attach_offset_angle").get<VECTOR>() * math::kDegToRad;
		attach_offset_scale			= assault_rifle.at("attach_offset_scale");
		weapon_kind					= assault_rifle.at("weapon_kind");
		holster_kind				= assault_rifle.at("holster_kind");
		cross_hair_distance			= assault_rifle.at("cross_hair_distance");
		max_remaining_bullet_num	= assault_rifle.at("max_remaining_bullet_num");
		range						= assault_rifle.at("range");
		initial_velocity			= assault_rifle.at("initial_velocity");
		deceleration				= assault_rifle.at("deceleration");
		scope_scale					= assault_rifle.at("scope_scale");
		recoil_data					= assault_rifle.at("recoil_data").get<RecoilData>();
		recoil_data.yaw   *= math::kDegToRad;
		recoil_data.pitch *= math::kDegToRad;
		shot_interval_time			= assault_rifle.at("shot_interval_time");
		gun_kind					= assault_rifle.at("gun_kind");
		cross_hair_max_radius		= assault_rifle.at("cross_hair_max_radius");
		cross_hair_middle_radius	= assault_rifle.at("cross_hair_middle_radius");
		cross_hair_min_radius		= assault_rifle.at("cross_hair_min_radius");
		muzzle_offset				= assault_rifle.at("muzzle_offset");
		ejection_port_offset		= assault_rifle.at("ejection_port_offset");
		load_port_offset			= assault_rifle.at("load_port_offset");

		m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ASSAULT_RIFLE, basic_angle, basic_scale);
		SetColliderModelHandle(m_modeler->GetModelHandle());
	}

	m_cross_hair_shape = std::make_shared<Circle>();
	m_current_remaining_bullet_num = max_remaining_bullet_num;
}

AssaultRifle::~AssaultRifle()
{
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->RemoveToObjManager();
}

void AssaultRifle::Init()
{

}

void AssaultRifle::Update()
{
	if (!IsActive()) { return; }

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Update();
}

void AssaultRifle::LateUpdate()
{
	if (!IsActive()) { return; }
}

void AssaultRifle::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Draw();
}

void AssaultRifle::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void AssaultRifle::InitCrossHairRange()
{
	m_cross_hair_radius = cross_hair_max_radius;
}

void AssaultRifle::CalcCrossHairRange(const VECTOR& owner_move_velocity)
{
	// ägéUîÕàÕÇéwíË
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	// ÉJÉÅÉâÇÃvelocityÇéÊìæ
	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	const auto camera_controller	= std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	const auto camera_velocity		= camera_controller->GetVelocity();
	
	if (owner_move_velocity != v3d::GetZeroV() || camera_velocity != v3d::GetZeroV())
	{
		if (m_cross_hair_radius < cross_hair_middle_radius)
		{
			math::Increase(m_cross_hair_radius, 600.0f * GetDeltaTime(), cross_hair_middle_radius, false);
		}
		else
		{
			math::Increase(m_cross_hair_radius, 90.0f * GetDeltaTime(), cross_hair_max_radius,	false);
		}
	}
	else
	{
		if (m_cross_hair_radius > cross_hair_middle_radius)
		{
			math::Decrease(m_cross_hair_radius, 20.0f * GetDeltaTime(), cross_hair_middle_radius);
		}
		else
		{
			math::Decrease(m_cross_hair_radius, 300.0f * GetDeltaTime(), cross_hair_min_radius);
		}
	}

	circle->SetRadius(m_cross_hair_radius);
}

void AssaultRifle::CalcCrossHairRangeShot()
{
	// ägéUîÕàÕÇéwíË
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	if (m_cross_hair_radius < cross_hair_middle_radius)
	{
		m_cross_hair_radius = cross_hair_middle_radius;
	}
	else
	{
		math::Increase(m_cross_hair_radius, 100.0f * GetDeltaTime(), cross_hair_max_radius, false);
	}

	circle->SetRadius(m_cross_hair_radius);
}

void AssaultRifle::CalcCrossHairPos()
{
	// ägéUîÕàÕÇéwíË
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	circle->SetPos(GetFirstShotPos() + m_aim_dir * cross_hair_distance);
	circle->SetNormalVector(m_aim_dir);
}

void AssaultRifle::CalcTargetPos()
{
	m_target_pos = math::GetRandomPointInCircle(*std::static_pointer_cast<Circle>(m_cross_hair_shape));
}

void AssaultRifle::CalcTransforms()
{
	CalcTransform(m_muzzle_transform,			muzzle_offset);
	CalcTransform(m_ejection_port_transform,	ejection_port_offset);
	CalcTransform(m_load_transform,				load_port_offset);
}
