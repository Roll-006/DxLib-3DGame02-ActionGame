#pragma once
#include <nlohmann/json.hpp>

struct RifleCartridgeBoxData
{
	std::string model_path			= "";
	int			max_have_num		= 0;
	VECTOR		basic_angle			= {};
	float		basic_scale			= 1.0f;
	float		project_ray_length	= 0.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, RifleCartridgeBoxData& rifle_cartridge_box_data)
{
	j_data.at("model_path")			.get_to(rifle_cartridge_box_data.model_path);
	j_data.at("max_have_num")			.get_to(rifle_cartridge_box_data.max_have_num);
	j_data.at("basic_angle")			.get_to(rifle_cartridge_box_data.basic_angle);
	j_data.at("basic_scale")			.get_to(rifle_cartridge_box_data.basic_scale);
	j_data.at("project_ray_length")	.get_to(rifle_cartridge_box_data.project_ray_length);
}

inline void to_json(nlohmann::json& j_data, const RifleCartridgeBoxData& rifle_cartridge_box_data)
{
	j_data = nlohmann::json
	{
		{ "model_path",			rifle_cartridge_box_data.model_path },
		{ "max_have_num",		rifle_cartridge_box_data.max_have_num },
		{ "basic_angle",		rifle_cartridge_box_data.basic_angle },
		{ "basic_scale",		rifle_cartridge_box_data.basic_scale },
		{ "project_ray_length",	rifle_cartridge_box_data.project_ray_length },
	};
}
#pragma endregion
