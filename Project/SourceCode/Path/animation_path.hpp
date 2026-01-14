#pragma once
#include <string>

static struct SWATAnimationPath
{
	#pragma region 汎用
	const std::string IDLE								= "Data/Animation/SWAT/idle.mv1";
	const std::string IDLE_INJURED						= "Data/Animation/SWAT/idle_injured.mv1";
	const std::string STAND_TO_CROUCH					= "Data/Animation/SWAT/stand_to_crouch.mv1";
	const std::string MOVE_FORWARD_RUN_01				= "Data/Animation/SWAT/move_forward_run_01.mv1";
	const std::string MOVE_FORWARD_RUN_INJURED			= "Data/Animation/SWAT/move_forward_run_injured.mv1";

	const std::string STAB_KNIFE						= "Data/Animation/SWAT/stab_knife.mv1";
	const std::string FIRST_SIDE_SLASH_KNIFE			= "Data/Animation/SWAT/side_slash_knife_left_to_right.mv1";
	const std::string SECOND_SIDE_SLASH_KNIFE			= "Data/Animation/SWAT/side_slash_knife_right_to_left.mv1";
	const std::string SPINNING_SLASH_KNIFE				= "Data/Animation/SWAT/spinning_slash_knife.mv1";
	const std::string STEALTH_KILL						= "Data/Animation/SWAT/stealth_kill.mv1";

	const std::string BLOW_AWAY							= "Data/Animation/SWAT/blow_away.mv1";
	const std::string GRABBED							= "Data/Animation/SWAT/grabbed.mv1";
	const std::string FRONT_KICK						= "Data/Animation/SWAT/front_kick_01.mv1";
	const std::string ROUNDHOUSE_KICK					= "Data/Animation/SWAT/roundhouse_kick.mv1";
	const std::string ESCAPE_KICK						= "Data/Animation/SWAT/front_kick_02.mv1";
	
	const std::string DEAD_02							= "Data/Animation/SWAT/death_02.mv1";
	#pragma endregion


	#pragma region 上半身用
	const std::string ATTACH_RIFLE						= "Data/Animation/SWAT/attach_rifle.mv1";
	const std::string DETACH_RIFLE						= "Data/Animation/SWAT/detach_rifle.mv1";
	const std::string ATTACH_HANDGUN					= "Data/Animation/SWAT/attach_handgun.mv1";
	const std::string DETACH_HANDGUN					= "Data/Animation/SWAT/detach_handgun.mv1";
	const std::string EQUIP_KNIFE						= "Data/Animation/SWAT/idle_equip_knife.mv1";
	const std::string AIM_KNIFE							= "Data/Animation/SWAT/aim_knife.mv1";
	const std::string PARRY								= "Data/Animation/SWAT/.mv1";
	const std::string EQUIP_GUN							= "Data/Animation/SWAT/idle_equip_gun.mv1";
	const std::string AIM_GUN							= "Data/Animation/SWAT/idle_aim_gun.mv1";
	const std::string RELOAD							= "Data/Animation/SWAT/crouch_reload.mv1";
	const std::string GRAB								= "Data/Animation/SWAT/move_forward_walk_zombie.mv1";
	const std::string TALKING_ON_PHONE					= "Data/Animation/SWAT/talking_on_phone.mv1";
	#pragma endregion


	#pragma region 下半身用
	const std::string CROUCH							= "Data/Animation/SWAT/idle_crouch_aim_gun.mv1";
	const std::string TALKING_CROUCH					= "Data/Animation/SWAT/crouch.mv1";

	const std::string MOVE_FORWARD						= "Data/Animation/SWAT/move_forward_aim_gun.mv1";
	const std::string MOVE_BACKWARD						= "Data/Animation/SWAT/move_backward_aim_gun.mv1";
	const std::string MOVE_LEFT							= "Data/Animation/SWAT/move_left_aim_gun.mv1";
	const std::string MOVE_RIGHT						= "Data/Animation/SWAT/move_right_aim_gun.mv1";
	const std::string MOVE_FORWARD_LEFT					= "Data/Animation/SWAT/move_forward_left_aim_gun.mv1";
	const std::string MOVE_FORWARD_RIGHT				= "Data/Animation/SWAT/move_forward_right_aim_gun.mv1";
	const std::string MOVE_BACKWARD_LEFT				= "Data/Animation/SWAT/move_backward_left_aim_gun.mv1";
	const std::string MOVE_BACKWARD_RIGHT				= "Data/Animation/SWAT/move_backward_right_aim_gun.mv1";

	const std::string MOVE_FORWARD_INJURED				= "Data/Animation/SWAT/move_forward_injured.mv1";
	const std::string MOVE_BACKWARD_INJURED				= "Data/Animation/SWAT/move_backward_injured.mv1";
	const std::string MOVE_LEFT_INJURED					= "Data/Animation/SWAT/move_left_injured.mv1";
	const std::string MOVE_RIGHT_INJURED				= "Data/Animation/SWAT/move_right_injured.mv1";
	const std::string MOVE_FORWARD_LEFT_INJURED			= "Data/Animation/SWAT/move_forward_left_injured.mv1";
	const std::string MOVE_FORWARD_RIGHT_INJURED		= "Data/Animation/SWAT/move_forward_right_injured.mv1";
	const std::string MOVE_BACKWARD_LEFT_INJURED		= "Data/Animation/SWAT/move_backward_left_injured.mv1";
	const std::string MOVE_BACKWARD_RIGHT_INJURED		= "Data/Animation/SWAT/move_backward_right_injured.mv1";

	const std::string MOVE_FORWARD_CROUCH				= "Data/Animation/SWAT/move_forward_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_CROUCH				= "Data/Animation/SWAT/move_backward_crouch_aim_gun.mv1";
	const std::string MOVE_LEFT_CROUCH					= "Data/Animation/SWAT/move_left_crouch_aim_gun.mv1";
	const std::string MOVE_RIGHT_CROUCH					= "Data/Animation/SWAT/move_right_crouch_aim_gun.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH			= "Data/Animation/SWAT/move_forward_left_crouch_aim_gun.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH			= "Data/Animation/SWAT/move_forward_right_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH			= "Data/Animation/SWAT/move_backward_left_crouch_aim_gun.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH		= "Data/Animation/SWAT/move_backward_right_crouch_aim_gun.mv1";
	#pragma endregion

}SWATAnimPath;


static struct ZombieAnimationPath
{
	#pragma region 汎用
	const std::string ZOMBIE_IDLE_01					= "Data/Animation/Zombie/zombie_idle_01.mv1";
	const std::string SHOUT_ZOMBIE						= "Data/Animation/Zombie/shout_zombie.mv1";
	const std::string MOVE_FORWARD_RUN_02				= "Data/Animation/Zombie/move_forward_run_02.mv1";
	const std::string MOVE_FORWARD_WALK					= "Data/Animation/Zombie/move_forward_walk.mv1";

	const std::string ENTER_NECK_BITE					= "Data/Animation/Zombie/enter_neck_bite.mv1";
	const std::string EXIT_NECK_BITE					= "Data/Animation/Zombie/exit_neck_bite.mv1";

	const std::string DEAD_01							= "Data/Animation/Zombie/death_01.mv1";
	const std::string DEAD_03							= "Data/Animation/Zombie/death_03.mv1";
	const std::string CROUCH_DEAD						= "Data/Animation/Zombie/crouch_death.mv1";
	const std::string FLYING_KNOCKBACK_UP				= "Data/Animation/Zombie/flying_knockback_up.mv1";
	const std::string FLYING_KNOCKBACK_DOWN				= "Data/Animation/Zombie/flying_knockback_down.mv1";
	const std::string BACKWARD_KNOCKBACK				= "Data/Animation/Zombie/backward_knockback.mv1";
	const std::string STAND_UP_ZOMBIE_01				= "Data/Animation/Zombie/stand_up_zombie_01.mv1";
	const std::string STAND_UP_ZOMBIE_02				= "Data/Animation/Zombie/stand_up_zombie_02.mv1";

	const std::string LEFT_CROUCH						= "Data/Animation/Zombie/left_crouch.mv1";
	const std::string RIGHT_CROUCH						= "Data/Animation/Zombie/right_crouch.mv1";
	#pragma endregion


	#pragma region 上半身用
	const std::string GRAB								= "Data/Animation/Zombie/move_forward_walk_zombie.mv1";
	const std::string STAND_STUN						= "Data/Animation/Zombie/move_backward_run_dizzy.mv1";
	#pragma endregion

}ZombieAnimPath;
