#include "weapon_base.hpp"

WeaponBase::WeaponBase(const std::string& name, const WeaponKind weapon_kind, const HolsterKind holster_kind) :
	PhysicalObjBase			(name, ObjTag.WEAPON/*, MassKind::kLight*/),
	m_modeler				(nullptr),
	m_owner_modeler			(nullptr),
	m_owner_name			(""),
	m_power					(0.0f),
	m_item_effect_transform	(nullptr),
	m_hold_offset_pos		(v3d::GetZeroV()),
	m_hold_offset_angle		(v3d::GetZeroV()),
	m_hold_offset_scale		(v3d::GetZeroV()),
	m_attach_offset_pos		(v3d::GetZeroV()),
	m_attach_offset_angle	(v3d::GetZeroV()),
	m_attach_offset_scale	(v3d::GetZeroV()),
	m_weapon_kind			(weapon_kind),
	m_holster_kind			(holster_kind)
{
	
}

void WeaponBase::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void WeaponBase::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		 (obj_handle);
}

void WeaponBase::OnProjectPos()
{

}

/// @brief 装備する持ち主をアタッチする
void WeaponBase::AttachOwner(const std::shared_ptr<Modeler>& owner_modeler, const std::string& owner_name)
{
	if (!m_owner_modeler)
	{
		m_owner_modeler = owner_modeler;
		m_owner_name	= owner_name;
	}
}
/// @brief 装備していた持ち主をデタッチする
void WeaponBase::DetachOwner()
{
	if (m_owner_modeler)
	{
		m_owner_modeler = nullptr;
		m_owner_name	= "";
	}
}

float WeaponBase::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

void WeaponBase::SetOffset(
	const VECTOR& hold_pos,
	const VECTOR& hold_angle,
	const VECTOR& hold_scale,
	const VECTOR& attach_pos,
	const VECTOR& attach_angle,
	const VECTOR& attach_scale)
{
	m_hold_offset_pos		= hold_pos;
	m_hold_offset_angle		= hold_angle;
	m_hold_offset_scale		= hold_scale;
	m_attach_offset_pos		= attach_pos;
	m_attach_offset_angle	= attach_angle;
	m_attach_offset_scale	= attach_scale;
}

void WeaponBase::SetOffset(
	const VECTOR& hold_pos,
	const VECTOR& hold_angle,
	const float   hold_scale,
	const VECTOR& attach_pos,
	const VECTOR& attach_angle,
	const float   attach_scale)
{
	m_hold_offset_pos		= hold_pos;
	m_hold_offset_angle		= hold_angle;
	m_hold_offset_scale		= VGet(hold_scale, hold_scale, hold_scale);
	m_attach_offset_pos		= attach_pos;
	m_attach_offset_angle	= attach_angle;
	m_attach_offset_scale	= VGet(attach_scale, attach_scale, attach_scale);
}
