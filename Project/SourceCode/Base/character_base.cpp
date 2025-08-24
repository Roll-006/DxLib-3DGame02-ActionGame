#include "character_base.hpp"

#pragma region 武器
CharacterBase::CharacterBase(const std::string& name, const std::string& tag, const MassKind mass_level_kind) : 
	PhysicalObjBase					(name, tag, mass_level_kind),
	m_modeler						(nullptr),
	m_animator						(nullptr),
	m_current_held_weapon			(nullptr),
	m_capsule_collider				(nullptr),
	m_capsule_length				(0.0f),
	m_capsule_radius				(0.0f)
{
		
}

void CharacterBase::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
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
	// TODO : カプセルのサイズの比率によってずらし量を自動で設定させるように変更
	const auto pos = m_capsule_collider->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(pos, sphere_radius), this));
}
#pragma endregion


void CharacterBase::ApplyLookDirToRot(const VECTOR& look_dir)
{
	m_transform->SetRot  (CoordinateKind::kWorld, look_dir);
}
