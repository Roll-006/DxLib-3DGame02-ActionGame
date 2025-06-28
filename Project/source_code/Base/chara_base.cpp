#include "chara_base.hpp"

#pragma region 武器
void CharaBase::AddGun(const std::shared_ptr<GunBase> gun)
{
	if (!m_guns.count(gun->GetGunKind())) { m_guns[gun->GetGunKind()] = gun; }
}

void CharaBase::RemoveGun(const GunKind gun_kind)
{
	m_guns.erase(gun_kind);
}


void CharaBase::AttachGun(const GunKind gun_kind)
{
	if (m_guns.count(gun_kind))
	{
		m_current_attach_gun = m_guns.at(gun_kind);
		m_current_attach_gun->AttachOwner(m_modeler);
	}
}

void CharaBase::DetachGun(const GunKind gun_kind)
{
	m_current_attach_gun = nullptr;
}
#pragma endregion


void CharaBase::MakeCollider(const float capsule_radius, const float sphere_radius)
{
	MakeCapsuleCollider(capsule_radius);
	MakeLandingTrigger (sphere_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kMeshTrigger, m_modeler->GetModelHandle(), this));
}

void CharaBase::CalcCapsuleLength()
{
	m_modeler->ApplyMatrix();

	// 頭部ボーンの行列情報を取得
	const int model_handle = m_modeler->GetModelHandle();
	const int frame_num    = MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END);
	MATRIX	  frame_mat    = MV1GetFrameLocalWorldMatrix(model_handle, frame_num);

	// 始点から頭部までの長さを取得
	m_capsule_length = VSize(m_transform->GetPos(CoordinateKind::kWorld) - MGetTranslateElem(frame_mat));

	const float segment_length = m_capsule_length - m_capsule_radius * 2.0f;
	m_capsule_collider->SetLength(segment_length);
}

void CharaBase::MakeCapsuleCollider(const float capsule_radius)
{
	m_capsule_radius = capsule_radius;

	const auto begin_pos		= m_transform->GetPos(CoordinateKind::kWorld) + VGet(0.0f, m_capsule_radius, 0.0f);
	const auto segment_length	= m_capsule_length - m_capsule_radius * 2.0f;
	m_capsule_collider = std::make_shared<Capsule>(begin_pos, m_transform->GetUp(CoordinateKind::kWorld), segment_length, m_capsule_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_capsule_collider, this));
}

void CharaBase::MakeLandingTrigger(const float sphere_radius)
{
	const auto pos = m_capsule_collider->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);
	AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(pos, sphere_radius), this));
}
