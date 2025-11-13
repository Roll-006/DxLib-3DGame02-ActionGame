#pragma once
#include "../Interface/i_item.hpp"
#include "../Interface/i_ammo_box.hpp"

#include "../Data/rifle_cartridge_box_data.hpp"
#include "../JSON/json_loader.hpp"

class AssaultRifleAmmoBox final : public IItem, public IAmmoBox
{
public:
	AssaultRifleAmmoBox();
	~AssaultRifleAmmoBox() override;

	void Synthesize(const std::shared_ptr<IAmmoBox> ammo_box) override;

	void SetHaveNum(const int set_num) override;
	void AddHaveNum(const int add_num) override;

	[[nodiscard]] int	GetMaxHaveNum()		const override { return rifle_cartridge_box_data.max_have_num; }
	[[nodiscard]] int	GetCurrentHaveNum()	const override { return m_have_num; }
	[[nodiscard]] bool	IsMax()				const override { return rifle_cartridge_box_data.max_have_num >= m_have_num; }

private:
	RifleCartridgeBoxData rifle_cartridge_box_data;

	int	m_have_num; // èäóLêî

	friend void from_json(const nlohmann::json& data, AssaultRifleAmmoBox& assault_rifle_ammo_box);
	friend void to_json  (nlohmann::json& data, const AssaultRifleAmmoBox& assault_rifle_ammo_box);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, AssaultRifleAmmoBox& assault_rifle_ammo_box)
{
	data.at("rifle_cartridge_box_data").get_to(assault_rifle_ammo_box.rifle_cartridge_box_data);
}

inline void to_json(nlohmann::json& data, const AssaultRifleAmmoBox& assault_rifle_ammo_box)
{
	data = nlohmann::json
	{
		{ "rifle_cartridge_box_data", assault_rifle_ammo_box.rifle_cartridge_box_data },
	};
}
#pragma endregion
