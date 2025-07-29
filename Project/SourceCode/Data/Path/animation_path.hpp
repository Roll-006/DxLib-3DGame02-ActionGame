#pragma once
#include <string>

static struct AnimationPath
{
	#pragma region UPPER BODY
	const std::string EQUIP_KNIFE					= "data/animation/.mv1";
	const std::string AIM_KNIFE						= "data/animation/.mv1";
	const std::string STAB_KNIFE					= "data/animation/.mv1";
	const std::string SIDE_SLASH_KNIFE				= "data/animation/.mv1";
	const std::string PARRY							= "data/animation/.mv1";
	const std::string EQUIP_GUN						= "data/animation/idle_equip_gun.mv1";
	const std::string AIM_GUN						= "data/animation/idle_aim_gun.mv1";
	const std::string RELOAD						= "data/animation/.mv1";
	#pragma endregion


	#pragma region LOWER BODY
	const std::string IDLE							= "data/animation/idle.mv1";
	const std::string CROUCH						= "data/animation/idle_crouch_aim_gun.mv1";

	const std::string MOVE_FORWARD					= "data/animation/move_forward_aim_gun.mv1";
	const std::string MOVE_BACKWARD					= "data/animation/move_backward_aim_gun.mv1";
	const std::string MOVE_LEFT						= "data/animation/move_left_aim_gun.mv1";
	const std::string MOVE_RIGHT					= "data/animation/move_right_aim_gun.mv1";
	const std::string MOVE_FORWARD_LEFT				= "data/animation/move_forward_left_aim_gun.mv1";
	const std::string MOVE_FORWARD_RIGHT			= "data/animation/move_forward_right_aim_gun.mv1";
	const std::string MOVE_BACKWARD_LEFT			= "data/animation/move_backward_left_aim_gun.mv1";
	const std::string MOVE_BACKWARD_RIGHT			= "data/animation/move_backward_right_aim_gun.mv1";

	const std::string MOVE_FORWARD_CROUCH			= "data/animation/move_forward_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_CROUCH			= "data/animation/move_backward_crouch_aim_gun.mv1";
	const std::string MOVE_LEFT_CROUCH				= "data/animation/move_left_crouch_aim_gun.mv1";
	const std::string MOVE_RIGHT_CROUCH				= "data/animation/move_right_crouch_aim_gun.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH		= "data/animation/move_forward_left_crouch_aim_gun.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH		= "data/animation/move_forward_right_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH		= "data/animation/move_backward_left_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH	= "data/animation/move_backward_right_crouch_aim_gun.mv1";

	const std::string MOVE_FORWARD_RUN				= "data/animation/move_forward_run_equip_gun.mv1";
	#pragma endregion

}AnimPath;
