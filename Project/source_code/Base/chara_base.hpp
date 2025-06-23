#pragma once
#include "physical_obj_base.hpp"
#include "gun_base.hpp"

#include "../Part/animator.hpp"
#include "../Part/bone_pos_corrector.hpp"

class CharaBase abstract : public PhysicalObjBase
{
public:
	CharaBase(const std::string& name, const std::string& tag, const std::string& file_path, MassKind mass_level_kind) :
		PhysicalObjBase		(name, tag, mass_level_kind),
		m_animator			(nullptr),
		m_current_attach_gun(nullptr),
		m_capsule_collider	(nullptr),
		m_capsule_length	(0.0f),
		m_capsule_radius	(0.0f)
	{
		m_modeler  = std::make_shared<Modeler>(m_transform, file_path, VGet(0.0f, DX_PI_F, 0.0f));
		m_animator = std::make_shared<Animator>(m_modeler);
	}

	virtual ~CharaBase() = default;

	/// @brief カプセルコライダーを作成
	void MakeCapsuleCollider(const float capsule_radius)
	{
		m_capsule_radius = capsule_radius;

		const auto begin_pos = m_transform->GetPos(CoordinateKind::kWorld) + VGet(0.0f, m_capsule_radius, 0.0f);
		const auto segment_length = m_capsule_length - m_capsule_radius * 2.0f;
		m_capsule_collider = std::make_shared<Capsule>(begin_pos, m_transform->GetUp(CoordinateKind::kWorld), segment_length, m_capsule_radius);

		AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_capsule_collider, this));
	}

	/// @brief 着地トリガーを作成
	void MakeLandingTrigger(const float sphere_radius)
	{
		const auto pos = m_capsule_collider->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);
		AddCollider(std::make_shared<Collider>(ColliderKind::kLandingTrigger, std::make_shared<Sphere>(pos, sphere_radius), this));
	}


	#pragma region 武器
	/// @brief 銃の所持登録 
	void AddGun(const std::shared_ptr<GunBase> gun)
	{
		if (!m_guns.count(gun->GetGunKind())) { m_guns[gun->GetGunKind()] = gun; }
	}
	/// @brief 銃の所持登録を解除
	void RemoveGun(const GunKind gun_kind)
	{
		m_guns.erase(gun_kind);
	}

	/// @brief 銃をアタッチ(装備)する
	void AttachGun(const GunKind gun_kind)
	{
		if (m_guns.count(gun_kind))
		{
			m_current_attach_gun = m_guns.at(gun_kind);
			m_current_attach_gun->AttachOwner(m_modeler);
		}
	}
	/// @brief 銃をデタッチ(装備解除)する
	void DetachGun(const GunKind gun_kind)
	{
		m_current_attach_gun = nullptr;
	}
	#pragma endregion


	[[nodiscard]] const std::shared_ptr<Modeler> GetModeler()const { return m_modeler; }

protected:
	virtual void LoadAnim() abstract;
	virtual void ChangeAnimState() abstract;

	/// @brief カプセルの長さを計算
	void CalcCapsuleLength()
	{
		m_modeler->ApplyMatrix();

		// 頭部ボーンの行列情報を取得
		const int model_handle = m_modeler->GetModelHandle();
		const int frame_num = MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END);
		MATRIX	  frame_mat = MV1GetFrameLocalWorldMatrix(model_handle, frame_num);

		// 始点から頭部までの長さを取得
		m_capsule_length = VSize(m_transform->GetPos(CoordinateKind::kWorld) - MGetTranslateElem(frame_mat));

		const float segment_length = m_capsule_length - m_capsule_radius * 2.0f;
		m_capsule_collider->SetLength(segment_length);
	}

protected:
	std::shared_ptr<Animator> m_animator;
	std::unordered_map<GunKind, std::shared_ptr<GunBase>> m_guns;	// 登録(所持)している銃
	std::shared_ptr<GunBase>  m_current_attach_gun;					// 現在アタッチ(装備)している銃

	std::shared_ptr<Capsule> m_capsule_collider;
	float m_capsule_length;
	float m_capsule_radius;
};
