#pragma once

enum class ZombieAnimKind
{
	kNone = -1,


	#pragma region 汎用
	kIdle,

	kMoveForwardWalk,
	kMoveForwardRun,

	kEnterNeckBite,
	kExitNeckBite,

	kDead,
	kCrouchDead,
	kFlyingKnockbackUp,
	kFlyingKnockbackDown,
	kStandUp,
	#pragma endregion


	#pragma region 上半身用
	kGrab,
	kStandStun,
	#pragma endregion


	#pragma region 下半身用
	kCrouchLeftStun,
	kCrouchRightStun,
	#pragma endregion
};
