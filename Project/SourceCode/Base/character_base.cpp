#include "character_base.hpp"

#pragma region ïêäÌ
CharacterBase::CharacterBase(const std::string& name, const std::string& tag, const MassKind mass_level_kind) : 
	PhysicalObjBase					(name, tag, mass_level_kind),
	m_modeler						(nullptr),
	m_animator						(nullptr),
	m_collider_creator				(std::make_shared<CharacterColliderCreator>()),
	m_current_held_weapon			(nullptr),
	m_invincible_time				(0.0f),
	m_invincible_timer				(0.0f),
	m_is_invincible					(false)
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

void CharacterBase::OnDamage(const HitPointsPartKind part_kind, const float damage)
{
	if (!m_hit_points.count(part_kind)) { return; }

	m_hit_points.at(part_kind)->OnDamage(damage);
	m_invincible_timer	= m_invincible_time;
	m_is_invincible		= true;
}

void CharacterBase::JudgeInvincible()
{
	math::Decrease(m_invincible_timer, GetDeltaTime(), 0.0f);
	if (m_invincible_timer <= 0.0f)
	{
		m_is_invincible = false;
	}
}
