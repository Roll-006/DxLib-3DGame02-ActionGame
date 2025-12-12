#pragma once
#include <DxLib.h>

struct HumanoidEnemyColliderData
{
	float  visible_fov				= 0.0f;
	float  visible_distance			= 0.0f;
	float  capsule_radius			= 0.0f;
	float  collision_area_radius	= 0.0f;
	VECTOR collision_area_offset	= {};
	float  landing_trigger_radius	= 0.0f;
	float  project_ray_length		= 0.0f;

	float  head_trigger_radius		= 0.0f;
	float  up_body_trigger_radius	= 0.0f;
	float  down_body_trigger_radius = 0.0f;
	float  upper_arm_trigger_radius = 0.0f;
	float  forearm_trigger_radius	= 0.0f;
	float  hand_trigger_radius		= 0.0f;
	float  up_leg_trigger_radius	= 0.0f;
	float  down_leg_trigger_radius	= 0.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, HumanoidEnemyColliderData& zombie_collider_data)
{
	j_data.at("visible_fov")				.get_to(zombie_collider_data.visible_fov);
	j_data.at("visible_distance")			.get_to(zombie_collider_data.visible_distance);
	j_data.at("capsule_radius")			.get_to(zombie_collider_data.capsule_radius);
	j_data.at("collision_area_radius")	.get_to(zombie_collider_data.collision_area_radius);
	j_data.at("collision_area_offset")	.get_to(zombie_collider_data.collision_area_offset);
	j_data.at("landing_trigger_radius")	.get_to(zombie_collider_data.landing_trigger_radius);
	j_data.at("project_ray_length")		.get_to(zombie_collider_data.project_ray_length);
	
	j_data.at("head_trigger_radius")		.get_to(zombie_collider_data.head_trigger_radius);
	j_data.at("up_body_trigger_radius")	.get_to(zombie_collider_data.up_body_trigger_radius);
	j_data.at("down_body_trigger_radius")	.get_to(zombie_collider_data.down_body_trigger_radius);
	j_data.at("upper_arm_trigger_radius")	.get_to(zombie_collider_data.upper_arm_trigger_radius);
	j_data.at("forearm_trigger_radius")	.get_to(zombie_collider_data.forearm_trigger_radius);
	j_data.at("hand_trigger_radius")		.get_to(zombie_collider_data.hand_trigger_radius);
	j_data.at("up_leg_trigger_radius")	.get_to(zombie_collider_data.up_leg_trigger_radius);
	j_data.at("down_leg_trigger_radius")	.get_to(zombie_collider_data.down_leg_trigger_radius);
}

inline void to_json(nlohmann::json& j_data, const HumanoidEnemyColliderData& zombie_collider_data)
{
	j_data = nlohmann::json
	{
		{ "visible_fov",				zombie_collider_data.visible_fov },
		{ "visible_distance",			zombie_collider_data.visible_distance },
		{ "capsule_radius",				zombie_collider_data.capsule_radius },
		{ "collision_area_radius",		zombie_collider_data.collision_area_radius },
		{ "collision_area_offset",		zombie_collider_data.collision_area_offset },
		{ "landing_trigger_radius",		zombie_collider_data.landing_trigger_radius },
		{ "project_ray_length",			zombie_collider_data.project_ray_length },
		
		{ "head_trigger_radius",		zombie_collider_data.head_trigger_radius },
		{ "up_body_trigger_radius",		zombie_collider_data.up_body_trigger_radius },
		{ "down_body_trigger_radius",	zombie_collider_data.down_body_trigger_radius },
		{ "upper_arm_trigger_radius",	zombie_collider_data.upper_arm_trigger_radius },
		{ "forearm_trigger_radius",		zombie_collider_data.forearm_trigger_radius },
		{ "hand_trigger_radius",		zombie_collider_data.hand_trigger_radius },
		{ "up_leg_trigger_radius",		zombie_collider_data.up_leg_trigger_radius },
		{ "down_leg_trigger_radius",	zombie_collider_data.down_leg_trigger_radius },
	};
}
#pragma endregion
