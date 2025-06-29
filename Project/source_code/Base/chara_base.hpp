#pragma once
#include "physical_obj_base.hpp"
#include "gun_base.hpp"

#include "../Part/animator.hpp"
#include "../Part/bone_pos_corrector.hpp"

class CharaBase abstract : public PhysicalObjBase
{
public:
	CharaBase(const std::string& name, const std::string& tag, const std::string& file_path, MassKind mass_level_kind);
	virtual ~CharaBase() = default;

	[[nodiscard]] const std::shared_ptr<Modeler> GetModeler()const { return m_modeler; }

protected:
	#pragma region アニメーション
	virtual void LoadAnim() abstract;
	virtual void ChangeAnimState() abstract;
	#pragma endregion


	#pragma region 武器
	/// @brief 武器の所持登録 
	template<obj_concepts::WeaponObjT WeaponObjT>
	void AddWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		// 上書き不可
		for (const auto& itr : m_weapons) { if (itr == weapon) { return; } }

		m_weapons.emplace_back(weapon);
	}

	/// @brief 武器の所持登録を解除
	/// @param weapon 武器
	template<obj_concepts::WeaponObjT WeaponObjT>
	void RemoveWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		remove(m_weapons.begin(), m_weapons.end(), weapon);
	}
	/// @brief 武器の所持登録を解除
	/// @param obj_name オブジェクト名(同じ武器を所持している場合はオブジェクトハンドルでの取得を推奨)
	void RemoveWeapon(const std::string& obj_name);
	/// @brief 武器の所持登録を解除
	/// @param obj_handle オブジェクトハンドル
	void RemoveWeapon(const int obj_handle);

	/// @brief 所持している武器の中からアタッチ(装備)する
	/// @param weapon 武器
	template<obj_concepts::WeaponObjT WeaponObjT>
	void AttachWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		if (std::find(m_weapons.begin(), m_weapons.end(), weapon) != m_weapons.end())
		{
			m_current_attach_weapon = weapon;
			m_current_attach_weapon->AttachOwner(m_modeler);
		}	
	}
	/// @brief 所持している武器の中からアタッチ(装備)する
	/// @brief 現在武器を装備している場合でもデタッチする必要はない
	/// @param obj_name オブジェクト名(同じ武器を所持している場合はオブジェクトハンドルでの取得を推奨)
	void AttachWeapon(const std::string& obj_name);
	/// @brief 所持している武器の中からアタッチ(装備)する
	/// @brief 現在武器を装備している場合でもデタッチする必要はない
	/// @param obj_handle オブジェクトハンドル
	void AttachWeapon(const int obj_handle);

	/// @brief 武器をデタッチ(装備解除)する
	void DetachWeapon();
	#pragma endregion


	#pragma region コライダー
	/// @brief キャラクターが標準的に持つコライダーを一括で作成する
	/// @brief カプセル(コライダー), 球(着地判定用トリガー), メッシュトリガー
	void CreateCollider(const float capsule_radius, const float sphere_radius);

	/// @brief カプセルの長さを計算
	void CalcCapsuleLength();
	#pragma endregion

private:
	#pragma region コライダー
	/// @brief カプセルコライダーを作成
	void CreateCapsuleCollider(const float capsule_radius);

	/// @brief 着地トリガーを作成
	void CreateLandingTrigger(const float sphere_radius);
	#pragma endregion

protected:
	std::shared_ptr<Animator> m_animator;

	std::vector<std::shared_ptr<WeaponBase>> m_weapons;		// 登録(所持)している武器
	std::shared_ptr<WeaponBase> m_current_attach_weapon;	// 現在アタッチ(装備)している武器

	std::shared_ptr<Capsule> m_capsule_collider;
	float m_capsule_length;
	float m_capsule_radius;
};
