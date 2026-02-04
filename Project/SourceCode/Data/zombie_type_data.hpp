#pragma once
#include <nlohmann/json.hpp>
#include "humanoid_enemy_collider_data.hpp"

struct ZombieTypeData
{
	std::string					model_path		= "";
	float						attack			= 0.0f;
	float						walk_speed		= 0.0f;
	float						run_speed		= 0.0f;
	float						run_grab_speed	= 0.0f;
	HumanoidEnemyColliderData	collider_data	= {};
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, ZombieTypeData& zombie_type_data)
{
	j_data.at("model_path")		.get_to(zombie_type_data.model_path);
	j_data.at("attack")			.get_to(zombie_type_data.attack);
	j_data.at("walk_speed")		.get_to(zombie_type_data.walk_speed);
	j_data.at("run_speed")		.get_to(zombie_type_data.run_speed);
	j_data.at("run_grab_speed")	.get_to(zombie_type_data.run_grab_speed);
	j_data.at("collider_data")	.get_to(zombie_type_data.collider_data);
}

inline void to_json(nlohmann::json& j_data, const ZombieTypeData& zombie_type_data)
{
	j_data = nlohmann::json
	{
		{ "model_path",		zombie_type_data.model_path },
		{ "attack",			zombie_type_data.attack },
		{ "walk_speed",		zombie_type_data.walk_speed },
		{ "run_speed",		zombie_type_data.run_speed },
		{ "run_grab_speed", zombie_type_data.run_grab_speed },
		{ "collider_data",	zombie_type_data.collider_data },
	};
}
#pragma endregion
