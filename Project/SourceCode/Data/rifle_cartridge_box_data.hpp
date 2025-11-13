#pragma once
#include <nlohmann/json.hpp>

struct RifleCartridgeBoxData
{
	int max_have_num;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, RifleCartridgeBoxData& rifle_cartridge_box_data)
{
	data.at("max_have_num").get_to(rifle_cartridge_box_data.max_have_num);
}

inline void to_json(nlohmann::json& data, const RifleCartridgeBoxData& rifle_cartridge_box_data)
{
	data = nlohmann::json
	{
		{ "max_have_num", rifle_cartridge_box_data.max_have_num },
	};
}
#pragma endregion
