#pragma once
#include <string>

static struct AnimationPath
{
	#pragma region UPPER BODY
	const std::string EQUIP_KNIFE						= "Data/Animation/idle_equip_knife.mv1";
	const std::string AIM_KNIFE							= "Data/Animation/aim_knife.mv1";
	const std::string STAB_KNIFE						= "Data/Animation/stab_knife.mv1";
	const std::string FIRST_SIDE_SLASH_KNIFE			= "Data/Animation/side_slash_knife_left_to_right.mv1";
	const std::string SECOND_SIDE_SLASH_KNIFE			= "Data/Animation/side_slash_knife_right_to_left.mv1";
	const std::string PARRY								= "Data/Animation/.mv1";
	const std::string EQUIP_GUN							= "Data/Animation/idle_equip_gun.mv1";
	const std::string AIM_GUN							= "Data/Animation/idle_aim_gun.mv1";
	const std::string RELOAD							= "Data/Animation/.mv1";
	#pragma endregion


	#pragma region LOWER BODY
	const std::string IDLE								= "Data/Animation/idle.mv1";
	const std::string CROUCH							= "Data/Animation/idle_crouch_aim_gun.mv1";

	const std::string MOVE_FORWARD						= "Data/Animation/move_forward_aim_gun.mv1";
	const std::string MOVE_BACKWARD						= "Data/Animation/move_backward_aim_gun.mv1";
	const std::string MOVE_LEFT							= "Data/Animation/move_left_aim_gun.mv1";
	const std::string MOVE_RIGHT						= "Data/Animation/move_right_aim_gun.mv1";
	const std::string MOVE_FORWARD_LEFT					= "Data/Animation/move_forward_left_aim_gun.mv1";
	const std::string MOVE_FORWARD_RIGHT				= "Data/Animation/move_forward_right_aim_gun.mv1";
	const std::string MOVE_BACKWARD_LEFT				= "Data/Animation/move_backward_left_aim_gun.mv1";
	const std::string MOVE_BACKWARD_RIGHT				= "Data/Animation/move_backward_right_aim_gun.mv1";

	const std::string MOVE_FORWARD_CROUCH				= "Data/Animation/move_forward_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_CROUCH				= "Data/Animation/move_backward_crouch_aim_gun.mv1";
	const std::string MOVE_LEFT_CROUCH					= "Data/Animation/move_left_crouch_aim_gun.mv1";
	const std::string MOVE_RIGHT_CROUCH					= "Data/Animation/move_right_crouch_aim_gun.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH			= "Data/Animation/move_forward_left_crouch_aim_gun.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH			= "Data/Animation/move_forward_right_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH			= "Data/Animation/move_backward_left_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH		= "Data/Animation/move_backward_right_crouch_aim_gun.mv1";

	const std::string MOVE_FORWARD_RUN					= "Data/Animation/move_forward_run.mv1";
	#pragma endregion

}AnimPath;
