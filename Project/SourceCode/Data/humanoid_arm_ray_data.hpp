#pragma once
#include <optional>
#include <nlohmann/json.hpp>
#include "../Shape/2DShape/triangle.hpp"

struct HumanoidArmRayData
{
	std::optional<VECTOR>	left_hand_cast_pos		= std::nullopt;
	std::optional<VECTOR>	right_hand_cast_pos		= std::nullopt;
	std::optional<Triangle> left_hand_hit_triangle	= std::nullopt;
	std::optional<Triangle> right_hand_hit_triangle = std::nullopt;

	float					hand_ray_length			= 0.0f;

	float					left_hand_ray_offset	= 0.0f;
	float					right_hand_ray_offset	= 0.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, HumanoidArmRayData& humanoid_arm_ray_data)
{
	data.at("hand_ray_length")			.get_to(humanoid_arm_ray_data.hand_ray_length);

	data.at("left_hand_ray_offset")		.get_to(humanoid_arm_ray_data.left_hand_ray_offset);
	data.at("right_hand_ray_offset")	.get_to(humanoid_arm_ray_data.right_hand_ray_offset);
}

inline void to_json(nlohmann::json& data, const HumanoidArmRayData& humanoid_arm_ray_data)
{
	data = nlohmann::json
	{
		{ "hand_ray_length",		humanoid_arm_ray_data.hand_ray_length },

		{ "left_hand_ray_offset",	humanoid_arm_ray_data.left_hand_ray_offset },
		{ "right_hand_ray_offset",	humanoid_arm_ray_data.right_hand_ray_offset },
	};
}
#pragma endregion
