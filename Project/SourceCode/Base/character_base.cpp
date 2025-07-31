#include "character_base.hpp"

#pragma region 武器
CharacterBase::CharacterBase(const std::string& name, const std::string& tag, const std::string& file_path, const MassKind mass_level_kind) : 
	PhysicalObjBase					(name, tag, mass_level_kind),
	m_modeler						(std::make_shared<Modeler>(m_transform, file_path, VGet(0.0f, DX_PI_F, 0.0f))),
	m_animator						(nullptr),
	m_current_equip_weapon			(nullptr),
	m_current_equip_weapon_kind		(WeaponKind::kNone),
	m_capsule_collider				(nullptr),
	m_capsule_length				(0.0f),
	m_capsule_radius				(0.0f),
	m_current_remaining_bullet_num	(0)
{
	SetModelHandle(m_modeler->GetModelHandle());
}

WeaponKind CharacterBase::GetCurrentEquipWeaponKind()
{
	m_current_equip_weapon_kind = m_current_equip_weapon ? m_current_equip_weapon->GetWeaponKind() : WeaponKind::kNone;
	return m_current_equip_weapon_kind;
}

void CharacterBase::RemoveWeapon(const std::string& obj_name)
{
	// 指定武器を削除
	const auto remove_weapon = std::find_if(m_weapons.begin(), m_weapons.end(), [=](const std::shared_ptr<WeaponBase> weapon)
	{
		return weapon->GetName() == obj_name;
	});
	m_weapons.erase(remove_weapon, m_weapons.end());
}

void CharacterBase::RemoveWeapon(const int obj_handle)
{
	// 指定武器を削除
	// MEMO : erase-remove idiom
	const auto remove_weapon = std::find_if(m_weapons.begin(), m_weapons.end(), [=](const std::shared_ptr<WeaponBase> weapon)
	{
		return weapon->GetObjHandle() == obj_handle;
	});

	m_weapons.erase(remove_weapon, m_weapons.end());
}

void CharacterBase::AttachWeapon(const std::string& obj_name)
{
	for (const auto& weapon : m_weapons)
	{
		if (weapon->GetName() == obj_name)
		{
			m_current_equip_weapon = weapon;
			m_current_equip_weapon->AttachOwner(m_modeler);
		}
	}
}

void CharacterBase::AttachWeapon(const int obj_handle)
{
	for (const auto& weapon : m_weapons)
	{
		if (weapon->GetObjHandle() == obj_handle)
		{
			m_current_equip_weapon = weapon;
			m_current_equip_weapon->AttachOwner(m_modeler);
		}
	}
}

void CharacterBase::DetachWeapon()
{
	m_current_equip_weapon = nullptr;
}
#pragma endregion


#pragma region コライダー
void CharacterBase::CreateCharaBasisCollider(const float capsule_radius, const float sphere_radius)
{
	CreateCapsuleCollider(capsule_radius);
	CreateLandingTrigger (sphere_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kMeshTrigger, m_modeler->GetModelHandle(), this));
}

void CharacterBase::CalcCapsuleColliderLength()
{
	m_modeler->ApplyMatrix();

	// 頭部ボーンの行列情報を取得
	const int model_handle = m_modeler->GetModelHandle();
	const int frame_num    = MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END);
	MATRIX	  frame_mat    = MV1GetFrameLocalWorldMatrix(model_handle, frame_num);

	// 始点から頭部までの長さを取得
	m_capsule_length = VSize(m_transform->GetPos(CoordinateKind::kWorld) - MGetTranslateElem(frame_mat));
	m_capsule_collider->SetCapsuleLength(m_capsule_length);
}

void CharacterBase::CreateCapsuleCollider(const float capsule_radius)
{
	m_capsule_radius = capsule_radius;

	const auto begin_pos		= m_transform->GetPos(CoordinateKind::kWorld) + VGet(0.0f, m_capsule_radius, 0.0f);
	const auto segment_length	= m_capsule_length - m_capsule_radius * 2.0f;
	m_capsule_collider = std::make_shared<Capsule>(begin_pos, m_transform->GetUp(CoordinateKind::kWorld), segment_length, m_capsule_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_capsule_collider, this));
}

void CharacterBase::CreateLandingTrigger(const float sphere_radius)
{
	const auto pos = m_capsule_collider->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(pos, sphere_radius), this));
}
#pragma endregion


void CharacterBase::UpdateTransform(const VECTOR& look_dir, const float scale)
{
	m_transform->SetRot  (CoordinateKind::kWorld, look_dir);
	m_transform->SetScale(CoordinateKind::kWorld, scale);
}
