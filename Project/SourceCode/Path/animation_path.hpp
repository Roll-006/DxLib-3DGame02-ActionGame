#pragma once
#include <string>

static struct AnimationPath
{
	#pragma region 汎用
	const std::string IDLE								= "Data/Animation/idle.mv1";
	const std::string ZOMBIE_IDLE_01					= "Data/Animation/zombie_idle_01.mv1";
	const std::string MOVE_FORWARD_RUN_01				= "Data/Animation/move_forward_run_01.mv1";
	const std::string MOVE_FORWARD_RUN_02				= "Data/Animation/move_forward_run_02.mv1";
	const std::string MOVE_FORWARD_WALK					= "Data/Animation/move_forward_walk.mv1";

	const std::string STAB_KNIFE						= "Data/Animation/stab_knife.mv1";
	const std::string FIRST_SIDE_SLASH_KNIFE			= "Data/Animation/side_slash_knife_left_to_right.mv1";
	const std::string SECOND_SIDE_SLASH_KNIFE			= "Data/Animation/side_slash_knife_right_to_left.mv1";
	const std::string SPINNING_SLASH_KNIFE				= "Data/Animation/spinning_slash_knife.mv1";

	const std::string BLOW_AWAY							= "Data/Animation/blow_away.mv1";
	const std::string ENTER_NECK_BITE					= "Data/Animation/enter_neck_bite.mv1";
	const std::string EXIT_NECK_BITE					= "Data/Animation/exit_neck_bite.mv1";
	const std::string GRABBED							= "Data/Animation/grabbed.mv1";
	const std::string FRONT_KICK						= "Data/Animation/front_kick_01.mv1";
	const std::string ROUNDHOUSE_KICK					= "Data/Animation/roundhouse_kick.mv1";
	const std::string ESCAPE_KICK						= "Data/Animation/front_kick_02.mv1";
	
	const std::string DEAD_01							= "Data/Animation/death_01.mv1";
	const std::string DEAD_02							= "Data/Animation/death_02.mv1";
	const std::string CROUCH_DEAD						= "Data/Animation/crouch_death.mv1";
	const std::string FLYING_KNOCKBACK_UP				= "Data/Animation/flying_knockback_up.mv1";
	const std::string FLYING_KNOCKBACK_DOWN				= "Data/Animation/flying_knockback_down.mv1";
	const std::string BACKWARD_KNOCKBACK				= "Data/Animation/backward_knockback.mv1";
	const std::string STAND_UP_ZOMBIE					= "Data/Animation/stand_up_zombie.mv1";
	#pragma endregion


	#pragma region 上半身用
	const std::string ATTACH_RIFLE						= "Data/Animation/attach_rifle.mv1";
	const std::string DETACH_RIFLE						= "Data/Animation/detach_rifle.mv1";
	const std::string ATTACH_HANDGUN					= "Data/Animation/attach_handgun.mv1";
	const std::string DETACH_HANDGUN					= "Data/Animation/detach_handgun.mv1";
	const std::string EQUIP_KNIFE						= "Data/Animation/idle_equip_knife.mv1";
	const std::string AIM_KNIFE							= "Data/Animation/aim_knife.mv1";
	const std::string PARRY								= "Data/Animation/.mv1";
	const std::string EQUIP_GUN							= "Data/Animation/idle_equip_gun.mv1";
	const std::string AIM_GUN							= "Data/Animation/idle_aim_gun.mv1";
	const std::string RELOAD							= "Data/Animation/crouch_reload.mv1";
	const std::string GRAB								= "Data/Animation/move_forward_walk_zombie.mv1";
	const std::string STAND_STUN						= "Data/Animation/move_backward_run_dizzy.mv1";
	#pragma endregion


	#pragma region 下半身用
	const std::string CROUCH							= "Data/Animation/idle_crouch_aim_gun.mv1";
	const std::string LEANING_FORWARD_CROUCH_LEFT		= "Data/Animation/idle_leaning_forward_crouch_left.mv1";
	const std::string LEANING_FORWARD_CROUCH_RIGHT		= "Data/Animation/idle_leaning_forward_crouch_right.mv1";

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
	#pragma endregion

}AnimPath;
