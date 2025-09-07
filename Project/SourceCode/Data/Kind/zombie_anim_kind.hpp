#pragma once

enum class ZombieAnimKind
{
	kNone = -1,


	#pragma region 汎用
	kIdle,

	kMoveForwardWalk,
	kMoveForwardRun,

	kNeckBite,
	#pragma endregion


	#pragma region 上半身用
	kGrab,
	#pragma endregion


	#pragma region 下半身用
	kCrouchStunLeft,
	kCrouchStunRight,
	#pragma endregion
};
