#include "character_base.hpp"

#pragma region ïêäÌ
CharacterBase::CharacterBase(const std::string& name, const std::string& tag, const MassKind mass_level_kind) : 
	PhysicalObjBase					(name, tag, mass_level_kind),
	m_modeler						(nullptr),
	m_animator						(nullptr),
	m_collider_creator				(std::make_shared<CharacterColliderCreator>()),
	m_current_held_weapon			(nullptr)
{
		
}

void CharacterBase::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}


#pragma region Getter
WeaponKind CharacterBase::GetCurrentHeldWeaponKind()
{
	return m_current_held_weapon ? m_current_held_weapon->GetWeaponKind() : WeaponKind::kNone;
}

std::shared_ptr<WeaponBase> CharacterBase::GetCurrentAttachWeapon(const HolsterKind holster_kind) const
{
	return m_attach_weapons.count(holster_kind) ? m_attach_weapons.at(holster_kind) : nullptr;
}

WeaponKind CharacterBase::GetCurrentAttachWeaponKind(const HolsterKind holster_kind) const
{
	return m_attach_weapons.count(holster_kind) ? m_attach_weapons.at(holster_kind)->GetWeaponKind() : WeaponKind::kNone;
}
#pragma endregion


void CharacterBase::HoldWeapon(const int obj_handle)
{
	auto weapon = ObjManager::GetInstance()->GetObj<WeaponBase>(obj_handle);

	if (weapon)
	{
		m_current_held_weapon = weapon;
		m_current_held_weapon->AttachOwner(m_modeler, GetName());
	}
}

void CharacterBase::ReleaseWeapon()
{
	m_current_held_weapon->DetachOwner();
	m_current_held_weapon = nullptr;
}

void CharacterBase::AttachWeapon(const int obj_handle)
{
	auto weapon = ObjManager::GetInstance()->GetObj<WeaponBase>(obj_handle);

	if (weapon)
	{
		m_attach_weapons[weapon->GetHolsterKind()] = weapon;
		m_attach_weapons[weapon->GetHolsterKind()]->AttachOwner(m_modeler, GetName());
	}
}

void CharacterBase::DetachWeapon(const HolsterKind holster_kind)
{
	m_attach_weapons[holster_kind]->DetachOwner();
	m_attach_weapons.erase(holster_kind);
}
#pragma endregion


void CharacterBase::ApplyLookDirToRot(const VECTOR& look_dir)
{
	m_transform->SetRot  (CoordinateKind::kWorld, look_dir);
}
