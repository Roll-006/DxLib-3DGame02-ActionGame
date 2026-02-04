#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "../Calculation/vector_3d.hpp"
#include "humanoid_enemy_collider_data.hpp"
#include "../Data/humanoid_arm_ray_data.hpp"
#include "../Data/humanoid_leg_ray_data.hpp"

struct ZombieData
{
	VECTOR				basic_angle					= {};
	float				basic_scale					= 1.0f;
	float				move_dir_offset_speed		= 0.0f;
	float				look_dir_offset_speed		= 0.0f;
	float				damage_over_time_start_time = 0.0f;
	float				disperse_distance			= 0.0f;
	HumanoidArmRayData	arm_ray_data				= {};
	HumanoidLegRayData	leg_ray_data				= {};
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, ZombieData& zombie_data)
{
	j_data.at("basic_angle")				.get_to(zombie_data.basic_angle);
	j_data.at("basic_scale")				.get_to(zombie_data.basic_scale);
	j_data.at("move_dir_offset_speed")		.get_to(zombie_data.move_dir_offset_speed);
	j_data.at("look_dir_offset_speed")		.get_to(zombie_data.look_dir_offset_speed);
	j_data.at("damage_over_time_start_time").get_to(zombie_data.damage_over_time_start_time);
	j_data.at("disperse_distance")			.get_to(zombie_data.disperse_distance);
	j_data.at("arm_ray_data")				.get_to(zombie_data.arm_ray_data);
	j_data.at("leg_ray_data")				.get_to(zombie_data.leg_ray_data);
}

inline void to_json(nlohmann::json& j_data, const ZombieData& zombie_data)
{
	j_data = nlohmann::json
	{
		{ "basic_angle",					zombie_data.basic_angle },
		{ "basic_scale",					zombie_data.basic_scale },
		{ "move_dir_offset_speed",			zombie_data.move_dir_offset_speed },
		{ "look_dir_offset_speed",			zombie_data.look_dir_offset_speed },
		{ "damage_over_time_start_time",	zombie_data.damage_over_time_start_time },
		{ "disperse_distance",				zombie_data.disperse_distance },
		{ "arm_ray_data",					zombie_data.arm_ray_data },
		{ "leg_ray_data",					zombie_data.leg_ray_data },
	};
}
#pragma endregion
