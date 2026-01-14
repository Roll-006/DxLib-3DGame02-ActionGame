#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_item.hpp"
#include "../Interface/i_ammo_box.hpp"

#include "../Data/rifle_cartridge_box_data.hpp"
#include "../JSON/json_loader.hpp"
#include "../Part/modeler.hpp"

#include "../Accessor/obj_accessor.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class AssaultRifleAmmoBox final : public PhysicalObjBase, public IItem, public IAmmoBox
{
public:
	AssaultRifleAmmoBox();
	AssaultRifleAmmoBox(const int ammo_num);
	~AssaultRifleAmmoBox() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw()	const	override;

	/// @brief 各マネージャーに自身を登録する
	void AddToObjManager()		override;
	void RemoveToObjManager()	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;

	void Synthesize(const std::shared_ptr<IAmmoBox> ammo_box) override;

	void SetHaveNum(const int set_num) override;
	void AddHaveNum(const int add_num) override;

	[[nodiscard]] const float		GetDeltaTime()		const override;
	[[nodiscard]] const ItemKind	GetItemKind()		const override { return ItemKind::kAmmoBox; }
	[[nodiscard]] const int			GetMaxHaveNum()		const override { return rifle_cartridge_box_data.max_have_num; }
	[[nodiscard]] const int			GetCurrentHaveNum()	const override { return m_have_num; }
	[[nodiscard]] const bool		IsMax()				const override { return rifle_cartridge_box_data.max_have_num >= m_have_num; }
	[[nodiscard]] const std::shared_ptr<Transform> GetItemTransform() const override { return m_item_effect_transform; }

private:
	RifleCartridgeBoxData		rifle_cartridge_box_data;

	int							m_have_num; // 所有数
	std::shared_ptr<Transform>	m_item_effect_transform;
	std::shared_ptr<Modeler>	m_modeler;

	friend void from_json(const nlohmann::json& j_data, AssaultRifleAmmoBox& assault_rifle_ammo_box);
	friend void to_json  (nlohmann::json& j_data, const AssaultRifleAmmoBox& assault_rifle_ammo_box);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, AssaultRifleAmmoBox& assault_rifle_ammo_box)
{
	j_data.at("rifle_cartridge_box_data").get_to(assault_rifle_ammo_box.rifle_cartridge_box_data);
}

inline void to_json(nlohmann::json& j_data, const AssaultRifleAmmoBox& assault_rifle_ammo_box)
{
	j_data = nlohmann::json
	{
		{ "rifle_cartridge_box_data", assault_rifle_ammo_box.rifle_cartridge_box_data },
	};
}
#pragma endregion
