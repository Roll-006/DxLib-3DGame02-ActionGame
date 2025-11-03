#pragma once
#include <DxLib.h>

struct HumanoidEnemyColliderData
{
	float  visible_fov;
	float  visible_distance;
	float  capsule_radius;
	float  collision_area_radius;
	VECTOR collision_area_offset;
	float  landing_trigger_radius;

	float  head_trigger_radius;
	float  up_body_trigger_radius;
	float  down_body_trigger_radius;
	float  upper_arm_trigger_radius;
	float  forearm_trigger_radius;
	float  hand_trigger_radius;
	float  up_leg_trigger_radius;
	float  down_leg_trigger_radius;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, HumanoidEnemyColliderData& zombie_collider_data)
{
	data.at("visible_fov")				.get_to(zombie_collider_data.visible_fov);
	data.at("visible_distance")			.get_to(zombie_collider_data.visible_distance);
	data.at("capsule_radius")			.get_to(zombie_collider_data.capsule_radius);
	data.at("collision_area_radius")	.get_to(zombie_collider_data.collision_area_radius);
	data.at("collision_area_offset")	.get_to(zombie_collider_data.collision_area_offset);
	data.at("landing_trigger_radius")	.get_to(zombie_collider_data.landing_trigger_radius);
	
	data.at("head_trigger_radius")		.get_to(zombie_collider_data.head_trigger_radius);
	data.at("up_body_trigger_radius")	.get_to(zombie_collider_data.up_body_trigger_radius);
	data.at("down_body_trigger_radius")	.get_to(zombie_collider_data.down_body_trigger_radius);
	data.at("upper_arm_trigger_radius")	.get_to(zombie_collider_data.upper_arm_trigger_radius);
	data.at("forearm_trigger_radius")	.get_to(zombie_collider_data.forearm_trigger_radius);
	data.at("hand_trigger_radius")		.get_to(zombie_collider_data.hand_trigger_radius);
	data.at("up_leg_trigger_radius")	.get_to(zombie_collider_data.up_leg_trigger_radius);
	data.at("down_leg_trigger_radius")	.get_to(zombie_collider_data.down_leg_trigger_radius);
}

inline void to_json(nlohmann::json& data, const HumanoidEnemyColliderData& zombie_collider_data)
{
	data = nlohmann::json
	{
		{ "visible_fov",				zombie_collider_data.visible_fov },
		{ "visible_distance",			zombie_collider_data.visible_distance },
		{ "capsule_radius",				zombie_collider_data.capsule_radius },
		{ "collision_area_radius",		zombie_collider_data.collision_area_radius },
		{ "collision_area_offset",		zombie_collider_data.collision_area_offset },
		{ "landing_trigger_radius",		zombie_collider_data.landing_trigger_radius },
		
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
