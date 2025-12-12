#pragma once
#include <optional>
#include <nlohmann/json.hpp>

struct HumanoidLegRayData
{
	std::optional<VECTOR>	left_leg_cast_pos			= std::nullopt;
	std::optional<VECTOR>	left_foot_cast_pos			= std::nullopt;
	std::optional<VECTOR>	left_toe_base_cast_pos		= std::nullopt;
	std::optional<VECTOR>	right_leg_cast_pos			= std::nullopt;
	std::optional<VECTOR>	right_foot_cast_pos			= std::nullopt;
	std::optional<VECTOR>	right_toe_base_cast_pos		= std::nullopt;

	float					leg_ray_length				= 0.0f;
	float					foot_ray_length				= 0.0f;
	float					toe_base_ray_length			= 0.0f;

	float					left_leg_ray_offset			= 0.0f;
	float					left_foot_ray_offset		= 0.0f;
	float					left_toe_base_ray_offset	= 0.0f;
	float					right_leg_ray_offset		= 0.0f;
	float					right_foot_ray_offset		= 0.0f;
	float					right_toe_base_ray_offset	= 0.0f;

	float					left_leg_offset				= 0.0f;
	float					left_heels_offset			= 0.0f;
	float					left_toe_offset				= 0.0f;
	float					right_leg_offset			= 0.0f;
	float					right_heels_offset			= 0.0f;
	float					right_toe_offset			= 0.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, HumanoidLegRayData& humanoid_leg_ray_data)
{
	j_data.at("leg_ray_length")			.get_to(humanoid_leg_ray_data.leg_ray_length);
	j_data.at("foot_ray_length")			.get_to(humanoid_leg_ray_data.foot_ray_length);
	j_data.at("toe_base_ray_length")		.get_to(humanoid_leg_ray_data.toe_base_ray_length);

	j_data.at("left_leg_ray_offset")		.get_to(humanoid_leg_ray_data.left_leg_ray_offset);
	j_data.at("left_foot_ray_offset")		.get_to(humanoid_leg_ray_data.left_foot_ray_offset);
	j_data.at("left_toe_base_ray_offset")	.get_to(humanoid_leg_ray_data.left_toe_base_ray_offset);
	j_data.at("right_leg_ray_offset")		.get_to(humanoid_leg_ray_data.right_leg_ray_offset);
	j_data.at("right_foot_ray_offset")	.get_to(humanoid_leg_ray_data.right_foot_ray_offset);
	j_data.at("right_toe_base_ray_offset").get_to(humanoid_leg_ray_data.right_toe_base_ray_offset);

	j_data.at("left_heels_offset")		.get_to(humanoid_leg_ray_data.left_heels_offset);
	j_data.at("left_toe_offset")			.get_to(humanoid_leg_ray_data.left_toe_offset);
	j_data.at("right_heels_offset")		.get_to(humanoid_leg_ray_data.right_heels_offset);
	j_data.at("right_toe_offset")			.get_to(humanoid_leg_ray_data.right_toe_offset);
}

inline void to_json(nlohmann::json& j_data, const HumanoidLegRayData& humanoid_leg_ray_data)
{
	j_data = nlohmann::json
	{
		{ "leg_ray_length",				humanoid_leg_ray_data.leg_ray_length },
		{ "foot_ray_length",			humanoid_leg_ray_data.foot_ray_length },
		{ "toe_base_ray_length",		humanoid_leg_ray_data.toe_base_ray_length },

		{ "left_leg_ray_offset",		humanoid_leg_ray_data.left_leg_ray_offset },
		{ "left_foot_ray_offset",		humanoid_leg_ray_data.left_foot_ray_offset },
		{ "left_toe_base_ray_offset",	humanoid_leg_ray_data.left_toe_base_ray_offset },
		{ "right_leg_ray_offset",		humanoid_leg_ray_data.right_leg_ray_offset },
		{ "right_foot_ray_offset",		humanoid_leg_ray_data.right_foot_ray_offset },
		{ "right_toe_base_ray_offset",	humanoid_leg_ray_data.right_toe_base_ray_offset },

		{ "left_heels_offset",			humanoid_leg_ray_data.left_heels_offset },
		{ "left_toe_offset",			humanoid_leg_ray_data.left_toe_offset },
		{ "right_heels_offset",			humanoid_leg_ray_data.right_heels_offset },
		{ "right_toe_offset",			humanoid_leg_ray_data.right_toe_offset },
	};
}
#pragma endregion
