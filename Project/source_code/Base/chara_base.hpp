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

	[[nodiscard]] const std::shared_ptr<Modeler> GetModeler()const { return m_modeler; }

protected:
	virtual void LoadAnim() abstract;
	virtual void ChangeAnimState() abstract;


	#pragma region 武器
	/// @brief 銃の所持登録 
	void AddGun(const std::shared_ptr<GunBase> gun);
	/// @brief 銃の所持登録を解除
	void RemoveGun(const GunKind gun_kind);

	/// @brief 銃をアタッチ(装備)する
	void AttachGun(const GunKind gun_kind);
	/// @brief 銃をデタッチ(装備解除)する
	void DetachGun(const GunKind gun_kind);
	#pragma endregion

	/// @brief キャラクターが標準的に持つコライダーを一括で作成する
	/// @brief カプセル(コライダー), 球(着地判定用トリガー), メッシュトリガー
	void MakeCollider(const float capsule_radius, const float sphere_radius);

	/// @brief カプセルの長さを計算
	void CalcCapsuleLength();

private:
	/// @brief カプセルコライダーを作成
	void MakeCapsuleCollider(const float capsule_radius);

	/// @brief 着地トリガーを作成
	void MakeLandingTrigger(const float sphere_radius);

protected:
	std::shared_ptr<Animator> m_animator;
	std::unordered_map<GunKind, std::shared_ptr<GunBase>> m_guns;	// 登録(所持)している銃
	std::shared_ptr<GunBase>  m_current_attach_gun;					// 現在アタッチ(装備)している銃

	std::shared_ptr<Capsule> m_capsule_collider;
	float m_capsule_length;
	float m_capsule_radius;
};
