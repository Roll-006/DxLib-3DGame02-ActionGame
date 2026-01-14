#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "../Calculation/vector_3d.hpp"
#include "humanoid_enemy_collider_data.hpp"

struct ZombieData
{
	std::string					model_path;
	VECTOR						basic_angle;
	float						basic_scale;
	float						walk_speed;
	float						run_speed;
	float						run_grab_speed;
	float						move_dir_offset_speed;
	float						look_dir_offset_speed;
	float						damage_over_time_start_time;
	HumanoidEnemyColliderData	collider_data;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, ZombieData& zombie_data)
{
	j_data.at("model_path")					.get_to(zombie_data.model_path);
	j_data.at("basic_angle")				.get_to(zombie_data.basic_angle);
	j_data.at("basic_scale")				.get_to(zombie_data.basic_scale);
	j_data.at("walk_speed")					.get_to(zombie_data.walk_speed);
	j_data.at("run_speed")					.get_to(zombie_data.run_speed);
	j_data.at("run_grab_speed")				.get_to(zombie_data.run_grab_speed);
	j_data.at("move_dir_offset_speed")		.get_to(zombie_data.move_dir_offset_speed);
	j_data.at("look_dir_offset_speed")		.get_to(zombie_data.look_dir_offset_speed);
	j_data.at("damage_over_time_start_time").get_to(zombie_data.damage_over_time_start_time);
	j_data.at("collider_data")				.get_to(zombie_data.collider_data);
}

inline void to_json(nlohmann::json& j_data, const ZombieData& zombie_data)
{
	j_data = nlohmann::json
	{
		{ "model_path",						zombie_data.model_path },
		{ "basic_angle",					zombie_data.basic_angle },
		{ "basic_scale",					zombie_data.basic_scale },
		{ "walk_speed",						zombie_data.walk_speed },
		{ "run_speed",						zombie_data.run_speed },
		{ "run_grab_speed",					zombie_data.run_grab_speed },
		{ "move_dir_offset_speed",			zombie_data.move_dir_offset_speed },
		{ "look_dir_offset_speed",			zombie_data.look_dir_offset_speed },
		{ "damage_over_time_start_time",	zombie_data.damage_over_time_start_time },
		{ "collider_data",					zombie_data.collider_data },
	};
}
#pragma endregion
