#pragma once
#include <string>

static struct AnimationPath
{
	#pragma region IDLE

	#pragma region 通常
	const std::string IDLE										= "data/animation/idle.mv1";
	const std::string IDLE_KNIFE_EQUIPPED						= "data/animation/.mv1";
	const std::string IDLE_AIM_KNIFE							= "data/animation/.mv1";
	const std::string IDLE_STAB_KNIFE							= "data/animation/.mv1";
	const std::string IDLE_SIDE_SLASH_KNIFE						= "data/animation/.mv1";
	const std::string IDLE_PARRY								= "data/animation/.mv1";
	const std::string IDLE_GUN_EQUIPPED							= "data/animation/.mv1";
	const std::string IDLE_AIM_GUN								= "data/animation/.mv1";
	const std::string IDLE_SHOT									= "data/animation/.mv1";
	const std::string IDLE_RELOAD								= "data/animation/.mv1";
	#pragma endregion

	#pragma region しゃがみ
	const std::string IDLE_CROUCH								= "data/animation/.mv1";
	const std::string IDLE_CROUCH_KNIFE_EQUIPPED				= "data/animation/.mv1";
	const std::string IDLE_RELOAD_GUN_EQUIPEED					= "data/animation/.mv1";
	const std::string IDLE_RELOAD_RELOAD						= "data/animation/.mv1";
	#pragma endregion

	#pragma region 振り向き
	const std::string IDLE_TURN_AROUND							= "data/animation/.mv1";
	const std::string IDLE_TURN_AROUND_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string IDLE_TURN_AROUND_GUN_EQUIPEED				= "data/animation/.mv1";
	#pragma endregion

	#pragma region しゃがみ振り向き
	const std::string IDLE_CROUCH_TURN_AROUND					= "data/animation/.mv1";
	const std::string IDLE_CROUCH_TURN_AROUND_KNIFE_EQUIPPED	= "data/animation/.mv1";
	const std::string IDLE_CROUCH_TURN_AROUND_GUN_EQUIPEED		= "data/animation/.mv1";
	#pragma endregion

	#pragma endregion



	#pragma region 移動

	#pragma region 通常
	const std::string MOVE_FORWARD								= "data/animation/.mv1";
	const std::string MOVE_BACKWARD								= "data/animation/.mv1";
	const std::string MOVE_LEFT									= "data/animation/.mv1";
	const std::string MOVE_RIGHT								= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT							= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT						= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT						= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT						= "data/animation/.mv1";
	#pragma endregion

	#pragma region 移動(ナイフ装備状態)
	const std::string MOVE_FORWARD_KNIFE_EQUIPPED				= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_KNIFE_EQUIPPED				= "data/animation/.mv1";
	const std::string MOVE_LEFT_KNIFE_EQUIPPED					= "data/animation/.mv1";
	const std::string MOVE_RIGHT_KNIFE_EQUIPPED					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_KNIFE_EQUIPPED		= "data/animation/.mv1";
	#pragma endregion

	#pragma region ナイフを構えながら移動
	const std::string MOVE_FORWARD_AIM_KNIFE					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_AIM_KNIFE					= "data/animation/.mv1";
	const std::string MOVE_LEFT_AIM_KNIFE						= "data/animation/.mv1";
	const std::string MOVE_RIGHT_AIM_KNIFE						= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_AIM_KNIFE				= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_AIM_KNIFE				= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_AIM_KNIFE				= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_AIM_KNIFE				= "data/animation/.mv1";
	#pragma endregion

	#pragma region 移動(銃装備状態)
	const std::string MOVE_FORWARD_GUN_EQUIPPED					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_GUN_EQUIPPED				= "data/animation/.mv1";
	const std::string MOVE_LEFT_GUN_EQUIPPED					= "data/animation/.mv1";
	const std::string MOVE_RIGHT_GUN_EQUIPPED					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_GUN_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_GUN_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_GUN_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_GUN_EQUIPPED			= "data/animation/.mv1";
	#pragma endregion

	#pragma region 銃を構えながら移動
	const std::string MOVE_FORWARD_AIM_GUN						= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_AIM_GUN						= "data/animation/.mv1";
	const std::string MOVE_LEFT_AIM_GUN							= "data/animation/.mv1";
	const std::string MOVE_RIGHT_AIM_GUNE						= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_AIM_GUN					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_AIM_GUN				= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_AIM_GUN				= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_AIM_GUN				= "data/animation/.mv1";
	#pragma endregion

	#pragma region 銃を撃ちながら移動
	const std::string MOVE_FORWARD_SHOT							= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_SHOT						= "data/animation/.mv1";
	const std::string MOVE_LEFT_SHOT							= "data/animation/.mv1";
	const std::string MOVE_RIGHT_SHOT							= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_SHOT					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_SHOT					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_SHOT					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_SHOT					= "data/animation/.mv1";
	#pragma endregion

	#pragma region リロードしながら移動
	const std::string MOVE_FORWARD_RELOAD						= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RELOAD						= "data/animation/.mv1";
	const std::string MOVE_LEFT_RELOAD							= "data/animation/.mv1";
	const std::string MOVE_RIGHT_RELOAD							= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_RELOAD					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_RELOAD					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_RELOAD					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_RELOAD				= "data/animation/.mv1";
	#pragma endregion

	#pragma region しゃがみ移動
	const std::string MOVE_FORWARD_CROUCH						= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_CROUCH						= "data/animation/.mv1";
	const std::string MOVE_LEFT_CROUCH							= "data/animation/.mv1";
	const std::string MOVE_RIGHT_CROUCH							= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH					= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH				= "data/animation/.mv1";
	#pragma endregion

	#pragma region しゃがみ移動(ナイフ装備状態)
	const std::string MOVE_FORWARD_CROUCH_KNIFE_EQUIPPED		= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_CROUCH_KNIFE_EQUIPPED		= "data/animation/.mv1";
	const std::string MOVE_LEFT_CROUCH_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_RIGHT_CROUCH_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH_KNIFE_EQUIPPED	= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH_KNIFE_EQUIPPED	= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH_KNIFE_EQUIPPED	= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH_KNIFE_EQUIPPED = "data/animation/.mv1";
	#pragma endregion

	#pragma region しゃがみ移動(銃装備状態)
	const std::string MOVE_FORWARD_CROUCH_GUN_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_CROUCH_GUN_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_LEFT_CROUCH_GUN_EQUIPPED				= "data/animation/.mv1";
	const std::string MOVE_RIGHT_CROUCH_GUN_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH_GUN_EQUIPPED		= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH_GUN_EQUIPPED	= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH_GUN_EQUIPPED	= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH_GUN_EQUIPPED	= "data/animation/.mv1";
	#pragma endregion

	#pragma region リロードしながら移動
	const std::string MOVE_FORWARD_CROUCH_RELOAD				= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_CROUCH_RELOAD				= "data/animation/.mv1";
	const std::string MOVE_LEFT_CROUCH_RELOAD					= "data/animation/.mv1";
	const std::string MOVE_RIGHT_CROUCH_RELOAD					= "data/animation/.mv1";
	const std::string MOVE_FORWARD_LEFT_CROUCH_RELOAD			= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RIGHT_CROUCH_RELOAD			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_LEFT_CROUCH_RELOAD			= "data/animation/.mv1";
	const std::string MOVE_BACKWARD_RIGHT_CROUCH_RELOAD			= "data/animation/.mv1";
	#pragma endregion

	#pragma region ダッシュ移動
	const std::string MOVE_FORWARD_RUN							= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RUN_KNIFE_EQUIPPED			= "data/animation/.mv1";
	const std::string MOVE_FORWARD_RUN_GUN_EQUIPPED				= "data/animation/move_forward_run.mv1";
	const std::string MOVE_FORWARD_RUN_RELOAD					= "data/animation/.mv1";
	#pragma endregion

	#pragma endregion

}AnimPath;
